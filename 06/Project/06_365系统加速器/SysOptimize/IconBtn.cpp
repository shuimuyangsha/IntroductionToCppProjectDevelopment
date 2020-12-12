// IconBtn.cpp : implementation file
//

#include "stdafx.h"
#include "SysOptimize.h"
#include "IconBtn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIconBtn

CIconBtn::CIconBtn()
{
	m_ImageList.Create(32,32,ILC_COLOR24|ILC_MASK,1,0);			// ����ͼ���б�
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICONINETCPL));				// ����ͼ��
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICONMMSYS)); 				// ����ͼ��
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICONTIMEDATE));				// ����ͼ��
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICONDESK)); 				// ����ͼ��
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICONACCESS));				// ����ͼ��
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICONMOUSE)); 				// ����ͼ��
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICONKEYBOARD));				// ����ͼ��
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICONINTL)); 				// ����ͼ��
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICONAPPWIZ));				// ����ͼ��
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICONHDWWIZ)); 				// ����ͼ��
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICONSYSDM));				// ����ͼ��
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICONCOMPUTER)); 				// ����ͼ��

	IsPressed = FALSE;
}

CIconBtn::~CIconBtn()
{
}


BEGIN_MESSAGE_MAP(CIconBtn, CButton)
	//{{AFX_MSG_MAP(CIconBtn)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIconBtn message handlers

void CIconBtn::SetImageIndex(UINT Index)
{
	m_ImageIndex = Index;
}

void CIconBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);	
	if(m_pImageList)
	{
		UINT state  = lpDrawItemStruct->itemState; // ��ȡ״̬
		// ��ȡͼ������ͼ���С
		IMAGEINFO imageinfo;
		m_ImageList.GetImageInfo(m_ImageIndex,&imageinfo);
		CSize imagesize;
		imagesize.cx = imageinfo.rcImage.right - imageinfo.rcImage.left;
		imagesize.cy = imageinfo.rcImage.bottom - imageinfo.rcImage.top;
		// �ڰ�ť��ֱ���������ʾλͼ
		CRect rect;
		GetClientRect(rect);
		CPoint point;
		point.x = 5;
		point.y = (rect.Height() - imagesize.cy)/2;
		m_ImageList.Draw(&dc,m_ImageIndex,point,ILD_NORMAL|ILD_TRANSPARENT);
		CRect focusRect(rect);
		focusRect.DeflateRect(2,2,2,2);	
		// ��ť��ѡ�л��߻�ý���ʱ
		if((state&ODS_SELECTED)||(state&ODS_FOCUS))
		{
			CPen pen(PS_DASHDOTDOT,1,RGB(0,0,0));
			CBrush brush;
			brush.CreateStockObject(NULL_BRUSH);
			dc.SelectObject(&brush);
			dc.SelectObject(&pen);
			// ���ƽ������
			dc.DrawFocusRect(focusRect);
			// ��������Ч��
			dc.DrawEdge(rect,BDR_RAISEDINNER|BDR_RAISEDOUTER,BF_BOTTOMLEFT|BF_TOPRIGHT);			
			// ��ý���ʱ���ƺ�ɫ�߿�
			dc.Draw3dRect(rect,RGB(0,0,0),RGB(0,0,0));
		}
		else  // Ĭ�������
		{		
			CPen pen(PS_DOT,1,RGB(192,192,192));
			CBrush brush;
			brush.CreateStockObject(NULL_BRUSH);
			dc.SelectObject(&brush);
			dc.SelectObject(&pen);
			dc.Rectangle(focusRect);			
			dc.DrawEdge(rect,BDR_RAISEDINNER|BDR_RAISEDOUTER,BF_BOTTOMLEFT|BF_TOPRIGHT);	
		}
		if(IsPressed) // �ڰ�ť������ʱ���ư���Ч��
		{
			dc.DrawFocusRect(focusRect);			
			dc.DrawEdge(rect,BDR_SUNKENINNER |BDR_SUNKENOUTER,BF_BOTTOMLEFT|BF_TOPRIGHT);
			dc.Draw3dRect(rect,RGB(0,0,0),RGB(0,0,0));	
		}
		// ���ư�ť�ı�
		CString text;
		GetWindowText(text);
		rect.DeflateRect(point.x+imagesize.cx,0,0,0);
		dc.SetBkMode(TRANSPARENT);
		dc.DrawText(text,rect,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	}
}

void CIconBtn::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	IsPressed = TRUE;
	CButton::OnLButtonDown(nFlags, point);
}

void CIconBtn::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	IsPressed = FALSE;
	CButton::OnLButtonUp(nFlags, point);
}


