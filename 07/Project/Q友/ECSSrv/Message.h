// Message.h: interface for the CMessage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESSAGE_H__5E759F12_353D_469A_8605_6A281948328D__INCLUDED_)
#define AFX_MESSAGE_H__5E759F12_353D_469A_8605_6A281948328D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define ACCOUNTLEN 5				// �û��˻�����
#define PASSWRODLEN 20				// ���볤��
#define CONTENTLEN 1024				// �ı�����
#define MAXACCOUNT	100				// ���������������������

// ������Ϣ����
#define MT_UNKNOWN	0x00000			// δ֪
#define MT_FIRSTLOG 0X00001			// �״ε�¼
#define MT_SENDDATA 0x00002			// ��������
#define MT_CONFIRM	0x00003			// ȷ����Ϣ
#define MT_DISCONNECT 0x00004		// �Ͽ�����
#define MT_NETTEST	0x00005			// ��·״̬����
#define MT_NETCONNECT 0x00006		// ����״̬����	 

// ������Ϣ������
#define ST_UNKNOWN	0x00000			// δ֪
#define ST_TEXT		0x00020			// �����ı�����
#define ST_IMAGE	0x00021			// ����ͼ������
#define ST_FILE		0x00022			// �����ļ�����
#define ST_FILEREQUEST 0x00023		// �����ļ�������Ϣ
#define ST_FILERESPONSE	0x00024		// ���շ����ļ�����
#define ST_FILEDENY		0x00025		// �ܾ������ļ�����
#define ST_FILECANCEL	0x00026		// �����ļ�ȡ��
#define ST_LOGINCONFIRM  0x00027	// ���͵�¼ȷ����Ϣ
#define ST_IMAGEREQUEST  0x00028	// ͼ���ļ���������
#define ST_IMAGERESPONSE 0x00029	// ͼ���ļ���������

class CMessage  
{
public:
	int m_nMsgType;					// ��Ϣ����
	int m_nSubType;					// ��Ϣ������
	char m_chAccount[ACCOUNTLEN];	// �˻�ID
	char m_chPassword[PASSWRODLEN];	// ��¼����
	char m_chContent[CONTENTLEN];	// ��Ϣ����
	int m_nAccountID[MAXACCOUNT];	// �Է��˻�ID
	int m_nSrcID;					// ������Ϣ��Դ�˻�ID
	int m_nFlag;					// ���ӱ�ʶ�����ڷ����ļ���˵��Ϊ0��ʾ�ļ�����δ��ɣ�Ϊ1�ļ��������
	DWORD m_dwFileLen;				// ��¼�ļ��ĳ���
	DWORD dwDatalen;				// ���ݱ��ĳ���
	char* m_pchData;				// ����,����δʹ��
	CMessage();
	virtual ~CMessage();
};

#endif // !defined(AFX_MESSAGE_H__5E759F12_353D_469A_8605_6A281948328D__INCLUDED_)
