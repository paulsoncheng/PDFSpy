/**
author:			paulson
createTime:		2021-12-15
Tel or qq:		911477285
*/
#include "stdafx.h"
#include "PDFObj.h"
#include "Tools.h"
#include "Err.h"
CPDFObj::CPDFObj(unsigned int nIndex, unsigned int nLocation, char* ver, char* valid)
	:m_nIndex(nIndex)
	,m_nLocation(nLocation)
	, m_pHeader(NULL)
	,m_pContent(NULL)
	
{
	if (strlen(ver) >= 16 || strlen(valid)>=4)return;
	memset(m_szVer, 0, 16);
	memset(m_szValid, 0, 4);
	strcpy_s(m_szVer,16, ver);
	strcpy_s(m_szValid,4, valid);
}
CPDFObj::CPDFObj(unsigned int nIndex, const char* xref)
	:m_nIndex(nIndex)
	,m_nLocation(0)
	, m_pHeader(NULL)
	, m_pContent(NULL)
{
	memset(m_szVer, 0, 16);
	memset(m_szValid, 0, 4);
	int			nLen = 0;
	char		pNum[24] = {0};
	if (NULL == xref)return;
	char* pTmp = TrimChar(xref, ' ');
	if (0 >= (nLen = GetNextString(pTmp, 0, ' ')))
		return;
	memcpy(pNum, pTmp, nLen);
	m_nLocation = strtoul(pNum, NULL, 10);
	pTmp = TrimChar(pTmp, ' ');
	if (0 >= (nLen = GetNextString(pTmp, 0, ' ')) || nLen>=16)
		return;
	memcpy(m_szVer, pTmp, nLen);
	pTmp = TrimChar(pTmp, ' ');
	if (0 >= (nLen = GetNextString(pTmp, 0, ' ')) || nLen >= 46)
		return;
	memcpy(m_szValid, pTmp, nLen);
}

CPDFObj::~CPDFObj()
{
	if (NULL == m_pHeader)
	{
		je_free(m_pHeader);
		m_pHeader = NULL;
	}
	/*if (NULL == m_szContent)
	{
		je_free(m_szContent);
		m_szContent = NULL;
	}*/
}
unsigned int CPDFObj::GetContent(FILE* fp)
{
	char				*pSrc = NULL, *pTmp = NULL, *pEnd = NULL;
	unsigned int		 nAdd = 0;
	if (NULL == fp)
		return ERR_PDF_FD;
	fseek(fp, m_nLocation, SEEK_SET);
	
	pSrc = (char*)je_calloc(SPLIT_LEN + 1, 1);
	while (1)
	{
		if (SPLIT_LEN != fread(pSrc + nAdd, 1, SPLIT_LEN, fp))
			return ERR_PDF_CONTENT;
		pTmp = pSrc + (nAdd > TAG_ENDOBJ_LEN ? (nAdd - TAG_ENDOBJ_LEN + 1) : 0);
		if (NULL == (pEnd = strstr(pTmp, TAG_ENDOBJ)))
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
		pEnd -= 1;
		while(*pEnd!='>' && pEnd!=pSrc)
			pEnd -= 1;
		//memset(pEnd, 0, nAdd + SPLIT_LEN + 1-(pEnd-pSrc));
		break;
	}
	m_pHeader = pSrc;
	if(NULL==(pTmp=strstr(pSrc,"obj")))
		return ERR_PDF_CONTENT;
	pTmp += 3;
	while (*pTmp != '<'&& *pTmp!='\0')
	{
		*pTmp = '\0';
		pTmp += 1;
	}
	m_pContent = pTmp;
	return 0;
}
void	CPDFObj::PrintObjDesc()
{
	printf("%04d\t%016d\t%s\t%s\n", m_nIndex, m_nLocation, m_szVer, m_szValid);
}
void	CPDFObj::PrintObjContent()
{
	if (NULL == m_pHeader)
		return;
	printf("%s  %s\n", m_pHeader, m_pContent);
}