/**
author:			paulson
createTime:		2021-12-15
Tel or qq:		911477285
*/
#include "stdafx.h"
#include "PDF.h"
#include "Err.h"
#include "Tools.h"
#include <algorithm>
CPDF::CPDF(const std::string& pdfPath)
	:m_pdfPath(pdfPath)
	, m_pdfFD(NULL)
{
}


CPDF::~CPDF()
{
	if (NULL != m_pdfFD)
	{
		fclose(m_pdfFD);
		m_pdfFD = NULL;
	}
}
unsigned int CPDF::VerifySign()
{
	unsigned nStart = 0, nEnd = 0, nRet = 0;
	if (m_pdfPath.empty()
		|| 0 != (fopen_s(&m_pdfFD, m_pdfPath.c_str(), "rb")))
		return ERR_PDF_FILEPATH;
	if (0 != (nRet = GetStartXrefLocation(nStart, nEnd)))
		return nRet;
	if (0 != (nRet = GetObj(nStart, nEnd)))
		return nRet;
	PrintObj();
	return 0;
}
bool	CPDF::IsObjIndexInMap(unsigned int nIndex)
{
	auto pos = find_if(m_objMap.cbegin(), m_objMap.cend(),
		[=](const std::pair<unsigned int,CPDFObj::Ptr> & item) {
		return item.first==nIndex;
	});
	if (pos != m_objMap.end())
		 return true;
	return false;
}
unsigned int CPDF::GetStartXrefLocation(unsigned int & loc,unsigned int& endLoc)
{
	unsigned int	nLoc = 256,nLength=0,nLen=0,nOffset=0;
	char			pBuf[260] = { 0 }, *pTmp = NULL, pNum[24] = {0};
	if (NULL == m_pdfFD)
		return ERR_PDF_FD;
	nLength = GetFileLength(m_pdfFD);
	fseek(m_pdfFD, nLength-nLoc, SEEK_SET);
	nLen=fread(pBuf+1, 1, nLoc, m_pdfFD);
	if (nLen < TAG_STARTXREF_LEN)
		return ERR_PDF_CONTENT;
	pTmp = pBuf+1 + nLen - TAG_STARTXREF_LEN;
	while (strncmp(pTmp, TAG_STARTXREF, TAG_STARTXREF_LEN) != 0 && pTmp != pBuf)
	{
		--pTmp;
	}
	if (pTmp == pBuf)
		return ERR_PDF_CONTENT;
	endLoc = nLength - nLoc + (pTmp - pBuf - 1);
	pTmp += 10;
	if(0>=(nOffset=GetNextString(pTmp,0,0x0A)))
		return ERR_PDF_CONTENT;
	memcpy(pNum, pTmp, nOffset);
	loc= strtoul(pNum, NULL, 10);
	return 0;
}
unsigned int CPDF::GetObj(const char* pSrc, unsigned int& preLoc)
{
	char			*pTmp = NULL, *pTmp1 = NULL, pNum[64] = { 0 };
	unsigned int	 nLoc = 0, nStartSeq = 0, nNum = 0, n = 0, nRet = 0;
	pTmp = (char*)pSrc;
	if (0 != strncmp((char*)pTmp, TAG_XREF, TAG_XREF_LEN))
	{
		return ERR_PDF_PARSE;
	}

	pTmp += (TAG_XREF_LEN + 1);
	while (!(*(pTmp + 2)>='a' && *(pTmp + 2) <= 'z'))
	{
		pTmp = TrimChar(pTmp, 0x0D);
		pTmp = TrimChar(pTmp, 0x0A);
		if (0 >= (nLoc = GetNextString(pTmp, 0, 0x0A)) || nLoc >= 32)
		{
			return ERR_PDF_CONTENT;
		}
		memset(pNum, 0, 64);
		if(*(pTmp+nLoc-1)==0x0D)
			memcpy(pNum, pTmp, nLoc-1);
		else
			memcpy(pNum, pTmp, nLoc );
		if (0 != GetObjSeq(pNum, nStartSeq, nNum))
		{
			return  ERR_PDF_CONTENT;
		}
		pTmp += nLoc;

		for (n = 0; n < nNum; ++n)
		{
			pTmp = TrimChar(pTmp, 0x0D);
			pTmp = TrimChar(pTmp, 0x0A);
			if (0 >= (nLoc = GetNextString(pTmp, 0, 0x0A)) || nLoc >= 32)
			{
				return  ERR_PDF_CONTENT;
			}
			memset(pNum, 0, 64);
			if (*(pTmp + nLoc - 1) == 0x0D)
				memcpy(pNum, pTmp, nLoc - 1);
			else
				memcpy(pNum, pTmp, nLoc);
			strncpy_s(pNum,64, pTmp, nLoc);
			if (!IsObjIndexInMap(nStartSeq + n))
			{
				if (GetObjLocation(pNum, nStartSeq + n))
				{
					return  ERR_PDF_CONTENT;
				}
			}
			pTmp += nLoc;

		}
	}
	pTmp = TrimChar(pTmp, 0x0D);
	pTmp = TrimChar(pTmp, 0x0A);
	if (0 != strncmp(pTmp, TAG_TRAILER, TAG_TRAILER_LEN))
	{
		return  ERR_PDF_CONTENT;
	}
	pTmp += (TAG_TRAILER_LEN + 1);
	if (NULL == (pTmp1 = strstr(pTmp, TAG_PREV)))
	{
		return  0;
	}
	pTmp = pTmp1 + TAG_PREV_LEN;
	pTmp = TrimChar(pTmp, ' ');
	if (NULL == (pTmp1 = strstr(pTmp, ">>")))
	{
		return  0;
	}
	memset(pNum, 0, 64);
	strncpy_s(pNum,64, pTmp, pTmp1 - pTmp);
	preLoc = strtoul(pNum, NULL, 10);
	return 0;
}
unsigned int CPDF::GetObj(unsigned int loc, unsigned int endLoc)
{
	char	*pSrc = NULL;
	unsigned int	nSrc = endLoc - loc,nRet=0,preLoc=0;
	if (NULL == (pSrc = (char*)je_calloc(nSrc + 1, 1)))
	{
		nRet = ERR_JE_MALLOC;
		goto Exit_Point;
	}
	fseek(m_pdfFD, loc, SEEK_SET);
	fread(pSrc, 1, nSrc, m_pdfFD);
	//pTmp = pSrc;
	while (1)
	{
		preLoc = 0;
		if (0 != (nRet = GetObj(pSrc, preLoc)))
			goto Exit_Point;
		if (0 == preLoc)
			break;
		je_free(pSrc);
		pSrc = NULL;
		if(0!=(nRet=GetObjString(&pSrc,preLoc, TAG_STARTXREF, TAG_STARTXREF_LEN)))
			goto Exit_Point;
	}
	

Exit_Point:
	if (NULL != pSrc)
	{
		je_free(pSrc);
		pSrc = NULL;
	}
	return nRet;
}
unsigned int CPDF::GetObjLocation(const char* pSrc, unsigned int nStartSeq)
{
	unsigned int		nLen=0,nLoc=0;
	char				*pTmp = (char*)pSrc, pNum[24] = { 0 }, pVer[16] = {0},pValid[4]= { 0 };
	if (NULL == pSrc)
		return ERR_PDF_PARSE;
	if (0 >= (nLen = GetNextString(pTmp, 0, ' '))|| nLen >=24)
		return ERR_PDF_CONTENT;
	strncpy_s(pNum,24, pTmp, nLen);
	nLoc = strtoul(pNum, NULL, 10);
	pTmp += nLen;
	pTmp = TrimChar(pTmp, ' ');
	memset(pNum, 0, 24);
	if (0 >= (nLen = GetNextString(pTmp, 0, ' '))|| nLen>=16)
		return ERR_PDF_CONTENT;
	strncpy_s(pVer,16, pTmp, nLen);
	pTmp += nLen;
	pTmp = TrimChar(pTmp, ' ');
	if (0 >= (nLen = GetNextString(pTmp, 0, ' '))|| nLen>=4)
		return ERR_PDF_CONTENT;
	strncpy_s(pValid,4, pTmp, nLen);
	m_objMap[nStartSeq] = std::make_shared<CPDFObj>(nStartSeq, nLoc, pVer, pValid);
	return 0;
}
unsigned int CPDF::GetObjSeq(const char* pSrc, unsigned int& nSeq, unsigned int& num)
{
	unsigned int	nLoc = 0;
	char			*pTmp = (char*)pSrc, pNum[24] = {0};
	if (NULL == pSrc)
		return ERR_PDF_PARSE;
	if (0 >= (nLoc = GetNextString(pTmp, 0, ' ')))
		return ERR_PDF_CONTENT;
	strncpy_s(pNum,24, pTmp, nLoc);
	nSeq = strtoul(pNum, NULL, 10);
	pTmp += nLoc;
	pTmp = TrimChar(pTmp, ' ');
	memset(pNum, 0, 24);
	if (0 >= (nLoc = GetNextString(pTmp, 0, ' ')))
		return ERR_PDF_CONTENT;
	strncpy_s(pNum,24, pTmp, nLoc);
	num = strtoul(pNum, NULL, 10);
	return 0;
}
unsigned int CPDF::GetObjString(char** pDst, unsigned int start, const char* endFlag, unsigned int nEndFlag)
{
	char				*pSrc = NULL,*pTmp=NULL,*pEnd=NULL;
	unsigned int		nLoc = start,nAdd=0;

	fseek(m_pdfFD, nLoc, SEEK_SET);
	pSrc = (char*)je_calloc(SPLIT_LEN+1, 1);
	while (1)
	{
		if(SPLIT_LEN!=fread(pSrc+nAdd,1, SPLIT_LEN,m_pdfFD))
			return ERR_PDF_CONTENT;
		pTmp = pSrc + (nAdd > nEndFlag ? (nAdd - nEndFlag+1) : 0);
		if (NULL == (pEnd = strstr(pTmp, endFlag)))
		{
			nAdd += SPLIT_LEN;
			if (NULL == (pTmp = (char*)je_realloc(pSrc, nAdd + SPLIT_LEN + 1)))
			{
				return ERR_JE_MALLOC;
			}
			memset(pTmp + nAdd, 0, SPLIT_LEN + 1);
			pSrc = pTmp;
			pTmp = NULL;
			continue;
		}
		memset(pEnd, 0, nEndFlag);
		break;
	}
	*pDst = pSrc;
	return 0;
}
void CPDF::PrintObj()
{
	for (auto item : m_objMap)
	{
		if(item.second->IsValid())
			item.second->GetContent(m_pdfFD);
	}
	for (auto item : m_objMap)
	{
		item.second->PrintObjDesc();
	}
	for (auto item : m_objMap)
	{
		if (item.second->IsValid())
			item.second->PrintObjContent();
	}
}