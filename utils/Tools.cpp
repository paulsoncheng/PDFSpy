/**
author:			paulson
createTime:		2021-12-15
Tel or qq:		911477285
*/
#include "stdafx.h"
#include "Tools.h"
#include <time.h>
#include <string.h>
unsigned int	 GetFileLength(FILE* fp)
{
	if (!fp)return -1;
	fseek(fp, 0, SEEK_SET);
	unsigned int offset = ftell(fp);
	fseek(fp, 0, SEEK_END);
	offset = ftell(fp) - offset;
	fseek(fp, 0, SEEK_SET);
	//fclose(fp);
	return offset;
}
unsigned char* GetFileContent(unsigned char** pBuf, const char* path, unsigned int& offset)
{
	FILE* fp = NULL;
	errno_t err = 0;
	if(0!=(err=fopen_s(&fp,path, "rb")) || NULL == fp)
		return NULL;
	unsigned int len = GetFileLength(fp);
	offset = 0;
	unsigned char* buf = (unsigned char*)je_calloc(len+1,1);
	if (NULL == buf)
	{
		fclose(fp);
		return NULL;
	}
	fread(buf, 1, len, fp);
	fclose(fp);
	offset = len;
	*pBuf = buf;
	return buf;
}
unsigned int GetCurrentGMTTime(char* gmttime,unsigned int nGmt)
{
	
	if (nGmt <= 15)
		return 1;
	time_t timep;
	struct tm p ;
	time(&timep);
	if (0 != gmtime_s(&p, &timep))
		return 2;

	memset(gmttime, 0, nGmt);
	sprintf_s(gmttime, nGmt, "%04d%02d%02d%02d%02d%02dZ", (1900 + p.tm_year), p.tm_mon + 1, p.tm_mday, p.tm_hour, p.tm_min, p.tm_sec);
	return 0;
}
unsigned int CompareTime(const char* src, const char* dst, unsigned long nDst,int& result)
{
	unsigned int err = 0;
	char tmp[20] = { 0 };
	if (nDst == 13)
		result= strncmp(src + 2, dst, nDst);
	else if (nDst == 15)
		result = strncmp(src, dst, nDst);
	else if (nDst == 20)
	{
		int				bRet = 0;
		unsigned long	nLen = 0;

		for (int n = 0; n < nDst; ++n)
		{
			if ((*(dst + n) >= '0' && *(dst + n) <= '9') || *(dst + n) == 'Z')
			{
				*(tmp + nLen) = *(dst + n);
				++nLen;
			}
		}
		result  = nLen == 13 ? strncmp(src + 2, tmp, nLen) : strncmp(src, tmp, nLen);
	}
	else
		err = 1;
	return err;
}
char* TrimChar(const char* pSrc, char ch)
{
	char* pTmp = (char*)pSrc;
	while (*pTmp == ch)
	{
		++pTmp;
	}
	return pTmp;
}
int	GetNextString(const char* pSrc, unsigned int start, char ch)
{
	
	if (start >= strlen(pSrc))
		return -1;
	char* pTmp = (char*)pSrc+start;
	while (*pTmp != '\0' && *pTmp != ch)
	{
		++pTmp;
	}	
	return pTmp-pSrc;
}