#pragma once
/**
author:			paulson
createTime:		2021-12-15
Tel or qq:		911477285
*/
#include <stdio.h>
#include <memory.h>
#include <stdlib.h >
#ifdef JEMALLOC
#include <jemalloc.h>
#else
#define je_malloc	malloc
#define je_calloc	calloc
#define je_free		free
#define je_realloc	realloc
#endif
#ifdef __cplusplus
extern "C" {
#endif
	unsigned char* 	GetFileContent(unsigned char** pBuf,const char* path, unsigned int& offset);
	unsigned int	GetFileLength(FILE* fp);
	unsigned int	GetCurrentGMTTime(char* gmttime, unsigned int nGmt);
	unsigned int	CompareTime(const char* src, const  char* dst, unsigned long nDst, int& result);
	int				GetNextString(const char* pSrc, unsigned int start, char ch);
	char*			TrimChar(const char* pSrc, char ch);
#ifdef __cplusplus
}
#endif
