#pragma once
#define	ERR_BASE				0x00000000
#define ERR_OK					0x00000000
#define ERR_DATA				(ERR_BASE+1)			//数据错误	
#define ERR_VERIFY_TYPE			(ERR_BASE+2)			//类型或结构验证失败	
#define ERR_ARGS				(ERR_BASE+3)			//参数错误或无效	
#define ERR_MEMBER				(ERR_BASE+4)			//成员数据无效或没有解析数据	
#define ERR_INIT_TIME			(ERR_BASE+5)			//获取时间失败
#define ERR_INVALID_TIME		(ERR_BASE+6)			//无效的日期
#define ERR_INVALID_USAGE		(ERR_BASE+7)			//证书用途不可用或不匹配
#define ERR_UNFOUND				(ERR_BASE+8)			//未找到相关项	
#define ERR_PARSE_URL			(ERR_BASE+9)			//解析url错误
#define ERR_REQUEST_CRL			(ERR_BASE+10)			//请求crl失败
#define ERR_RECV_CRL			(ERR_BASE+11)			//接收crl数据失败
#define ERR_JE_MALLOC			(ERR_BASE+12)			//申请缓存失败
#define ERR_REVOKED				(ERR_BASE+13)			//已经注销
#define ERR_ROOTCERT_DATA		(ERR_BASE+14)			//根证书数据错误或验签失败
#define ERR_CACERT_DATA			(ERR_BASE+15)			//根证书数据错误
#define ERR_SIGNCERT_DISMATCH	(ERR_BASE+16)			//签名证书不匹配
#define ERR_SRCHASH_DISMATCH	(ERR_BASE+17)			//原文hash不匹配
#define ERR_PDF_FILEPATH		(ERR_BASE+18)			//pdf文件错误
#define ERR_PDF_FD				(ERR_BASE+19)			//pdf文件描述符错误
#define ERR_PDF_CONTENT			(ERR_BASE+20)			//pdf内容错误
#define ERR_PDF_PARSE			(ERR_BASE+21)			//pdf内容解析错误