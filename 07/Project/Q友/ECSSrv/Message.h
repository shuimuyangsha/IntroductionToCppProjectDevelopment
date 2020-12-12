// Message.h: interface for the CMessage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESSAGE_H__5E759F12_353D_469A_8605_6A281948328D__INCLUDED_)
#define AFX_MESSAGE_H__5E759F12_353D_469A_8605_6A281948328D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define ACCOUNTLEN 5				// 用户账户长度
#define PASSWRODLEN 20				// 密码长度
#define CONTENTLEN 1024				// 文本长度
#define MAXACCOUNT	100				// 服务器承受最大连接数量

// 定义消息类型
#define MT_UNKNOWN	0x00000			// 未知
#define MT_FIRSTLOG 0X00001			// 首次登录
#define MT_SENDDATA 0x00002			// 发送数据
#define MT_CONFIRM	0x00003			// 确认信息
#define MT_DISCONNECT 0x00004		// 断开连接
#define MT_NETTEST	0x00005			// 网路状态测试
#define MT_NETCONNECT 0x00006		// 网络状态良好	 

// 定义消息子类型
#define ST_UNKNOWN	0x00000			// 未知
#define ST_TEXT		0x00020			// 发送文本类型
#define ST_IMAGE	0x00021			// 发送图像类型
#define ST_FILE		0x00022			// 发送文件类型
#define ST_FILEREQUEST 0x00023		// 发送文件请求信息
#define ST_FILERESPONSE	0x00024		// 接收发送文件请求
#define ST_FILEDENY		0x00025		// 拒绝接收文件请求
#define ST_FILECANCEL	0x00026		// 发送文件取消
#define ST_LOGINCONFIRM  0x00027	// 发送登录确认信息
#define ST_IMAGEREQUEST  0x00028	// 图像文件连接请求
#define ST_IMAGERESPONSE 0x00029	// 图像文件接收请求

class CMessage  
{
public:
	int m_nMsgType;					// 消息类型
	int m_nSubType;					// 消息子类型
	char m_chAccount[ACCOUNTLEN];	// 账户ID
	char m_chPassword[PASSWRODLEN];	// 登录密码
	char m_chContent[CONTENTLEN];	// 消息长度
	int m_nAccountID[MAXACCOUNT];	// 对方账户ID
	int m_nSrcID;					// 发送消息的源账户ID
	int m_nFlag;					// 附加标识，对于发送文件来说，为0表示文件发送未完成，为1文件发送完成
	DWORD m_dwFileLen;				// 记录文件的长度
	DWORD dwDatalen;				// 数据报的长度
	char* m_pchData;				// 数据,保留未使用
	CMessage();
	virtual ~CMessage();
};

#endif // !defined(AFX_MESSAGE_H__5E759F12_353D_469A_8605_6A281948328D__INCLUDED_)
