// SelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SysOptimize.h"
#include "SelectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectDlg dialog


CSelectDlg::CSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectDlg)
	ON_BN_CLICKED(IDC_BUTOK, OnButok)
	ON_BN_CLICKED(IDC_BUTCANCEL, OnButcancel)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectDlg message handlers

BOOL CSelectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString num;
	for(int i=0;i<25;i++)
	{
		num.Format("%d",i+1);
		CButton* check = (CButton*)GetDlgItem(IDC_CHECK1+i);// ��ø�ѡ��ָ��
		char ischeck[2];
		GetPrivateProfileString("�����ļ�����", num, "", ischeck,2,"./litterfile.ini");
		num = ischeck;
		check->SetCheck(atoi(num));// ���ø�ѡ��״̬
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectDlg::OnButok() 
{
	// TODO: Add your control notification handler code here
	CString num,ischeck;
	for(int i=0;i<25;i++)
	{
		num.Format("%d",i+1);
		CButton* check = (CButton*)GetDlgItem(IDC_CHECK1+i);// ��ø�ѡ��ָ��
		ischeck.Format("%d",check->GetCheck());// ���ø�ѡ��״̬	
		WritePrivateProfileString("�����ļ�����", num, ischeck,"./litterfile.ini");	
	}
	CDialog::OnCancel();
}

void CSelectDlg::OnButcancel() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

HBRUSH CSelectDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	CBitmap m_BKGround;
	m_BKGround.LoadBitmap(IDB_BITBLANK);
	if (nCtlColor==CTLCOLOR_DLG)
	{
		// ����һ��λͼ��ˢ
		CBrush m_Brush(&m_BKGround);
		CRect rect;
		GetClientRect(rect);
		// ѡ�л�ˢ
		pDC->SelectObject(&m_Brush);
		// ���ͻ�����
		pDC->FillRect(rect,&m_Brush);
		return m_Brush;
	}
	else
		hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if (nCtlColor==CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
	}
	return hbr;
}
