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
	if(nChar == 8)			//���������˸�������š�С����
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);			//���û���ķ���
		return;
	}
	if(nChar<65 || nChar>90) 							//������������
		nChar = 0;								//���ü�ֵΪ0
	else
		CEdit::OnChar(nChar, nRepCnt, nFlags);
}
