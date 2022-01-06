#pragma once
/**
	author:			paulson
	createTime:		2021-12-15
	Tel or qq:		911477285
*/
#include <string>
#include <map>
#include "PDFObj.h"
#include <stdlib.h>

class CPDF
{
public:
	CPDF(const std::string& pdfPath);
	virtual ~CPDF();
protected:
	std::string			m_pdfPath;
	FILE				*m_pdfFD;
	std::map<unsigned int,CPDFObj::Ptr> m_objMap;
protected:
	bool		 IsObjIndexInMap(unsigned int nIndex);
	unsigned int GetObjLocation(const char* pSrc,unsigned int nStartSeq);
	unsigned int GetObjSeq(const char* pSrc,unsigned int& nSeq, unsigned int& num);
	unsigned int GetObj(unsigned int loc, unsigned int endLoc);
	unsigned int GetObj(const char* pSrc,unsigned int& preLoc);
	unsigned int GetObjString(char** pDst, unsigned int start,const char* endFlag,unsigned int nEndFlag);
	unsigned int GetStartXrefLocation(unsigned int & loc, unsigned int& endLoc);
public:
	unsigned int VerifySign();
	void PrintObj();
};

