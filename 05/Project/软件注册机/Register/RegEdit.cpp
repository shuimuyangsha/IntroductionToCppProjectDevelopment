// RegEdit.cpp : implementation file
//

#include "stdafx.h"
#include "Register.h"
#include "RegEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegEdit

CRegEdit::CRegEdit()
{
}

CRegEdit::~CRegEdit()
{
}


BEGIN_MESSAGE_MAP(CRegEdit, CEdit)
	//{{AFX_MSG_MAP(CRegEdit)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegEdit message handlers

void CRegEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if(nChar == 8)			//允许输入退格键、减号、小数点
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);			//调用基类的方法
		return;
	}
	if(nChar<65 || nChar>90) 							//允许输入数字
		nChar = 0;								//设置键值为0
	else
		CEdit::OnChar(nChar, nRepCnt, nFlags);
}
