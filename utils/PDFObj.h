#pragma once
/**
author:			paulson
createTime:		2021-12-15
Tel or qq:		911477285
*/
#include <memory>
#define TAG_STARTXREF				"startxref"
#define TAG_STARTXREF_LEN			9	
#define TAG_XREF					"xref"
#define TAG_XREF_LEN				4	
#define TAG_TRAILER					"trailer"
#define TAG_TRAILER_LEN				7	
#define TAG_PREV					"/Prev"
#define TAG_PREV_LEN				5
#define TAG_ENDOBJ					"endobj"
#define TAG_ENDOBJ_LEN				6
#define SPLIT_LEN					128
class CPDFObj
{
public:
	typedef std::shared_ptr<CPDFObj> Ptr;
	CPDFObj(unsigned int nIndex,unsigned int nLocation,char* ver,char* valid);
	CPDFObj(unsigned int nIndex, const char* xref);
	~CPDFObj();
protected:
	unsigned int	m_nIndex;
	unsigned int	m_nLocation;
	char			m_szVer[16];
	char			m_szValid[4];
	char*			m_pHeader;
	char*			m_pContent;
public:
	unsigned int	GetContent(FILE* fp);
	void			PrintObjDesc();
	void			PrintObjContent();
	bool			IsValid() { return 0 == strncmp(m_szValid, "n", 1); }
};

