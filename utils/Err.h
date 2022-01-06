#pragma once
#define	ERR_BASE				0x00000000
#define ERR_OK					0x00000000
#define ERR_DATA				(ERR_BASE+1)			//���ݴ���	
#define ERR_VERIFY_TYPE			(ERR_BASE+2)			//���ͻ�ṹ��֤ʧ��	
#define ERR_ARGS				(ERR_BASE+3)			//�����������Ч	
#define ERR_MEMBER				(ERR_BASE+4)			//��Ա������Ч��û�н�������	
#define ERR_INIT_TIME			(ERR_BASE+5)			//��ȡʱ��ʧ��
#define ERR_INVALID_TIME		(ERR_BASE+6)			//��Ч������
#define ERR_INVALID_USAGE		(ERR_BASE+7)			//֤����;�����û�ƥ��
#define ERR_UNFOUND				(ERR_BASE+8)			//δ�ҵ������	
#define ERR_PARSE_URL			(ERR_BASE+9)			//����url����
#define ERR_REQUEST_CRL			(ERR_BASE+10)			//����crlʧ��
#define ERR_RECV_CRL			(ERR_BASE+11)			//����crl����ʧ��
#define ERR_JE_MALLOC			(ERR_BASE+12)			//���뻺��ʧ��
#define ERR_REVOKED				(ERR_BASE+13)			//�Ѿ�ע��
#define ERR_ROOTCERT_DATA		(ERR_BASE+14)			//��֤�����ݴ������ǩʧ��
#define ERR_CACERT_DATA			(ERR_BASE+15)			//��֤�����ݴ���
#define ERR_SIGNCERT_DISMATCH	(ERR_BASE+16)			//ǩ��֤�鲻ƥ��
#define ERR_SRCHASH_DISMATCH	(ERR_BASE+17)			//ԭ��hash��ƥ��
#define ERR_PDF_FILEPATH		(ERR_BASE+18)			//pdf�ļ�����
#define ERR_PDF_FD				(ERR_BASE+19)			//pdf�ļ�����������
#define ERR_PDF_CONTENT			(ERR_BASE+20)			//pdf���ݴ���
#define ERR_PDF_PARSE			(ERR_BASE+21)			//pdf���ݽ�������