// DataManage.h: interface for the CDataManage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAMANAGE_H__76E52576_E432_494A_A0BA_262CC2DDD9C9__INCLUDED_)
#define AFX_DATAMANAGE_H__76E52576_E432_494A_A0BA_262CC2DDD9C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDataManage  
{
public:
	// 初始化数据库
	BOOL InitDatabase();
	_ConnectionPtr m_pConnect;
	_CommandPtr    m_pCommand;
	_RecordsetPtr  m_pRecord;
	static CString m_ConnectStr;
	CDataManage();
	virtual ~CDataManage();

};

#endif // !defined(AFX_DATAMANAGE_H__76E52576_E432_494A_A0BA_262CC2DDD9C9__INCLUDED_)
