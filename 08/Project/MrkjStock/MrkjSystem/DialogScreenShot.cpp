// DialogScreenShot.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MrkjSystem.h"
#include "DialogScreenShot.h"
#include "afxdialogex.h"
#include "ScreenTools.h"

// CDialogScreenShot �Ի���

IMPLEMENT_DYNAMIC(CDialogScreenShot, CDialogEx)

CDialogScreenShot::CDialogScreenShot(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_DIALOG_SCREEN_SHOT, pParent)
{
}

CDialogScreenShot::~CDialogScreenShot()
{
}

void CDialogScreenShot::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogScreenShot, CDialogEx)
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_CTLCOLOR()
    ON_WM_SETCURSOR()
END_MESSAGE_MAP()



// CDialogScreenShot ��Ϣ�������

static CPoint ptStart{ -1, -1}; //��һ����
static CPoint ptLast{ -1, -1}; //�ڶ�����(��������ƶ�֮ǰ��)

BOOL CDialogScreenShot::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    // �����Ļ�ֱ���
    int w = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    // ���ñ����ڴ�С, �ڵ�������Ļ
    MoveWindow(0, 0, w, y);
    // ������ˢ
    m_bkBrush.CreateSolidBrush(RGB(0, 100, 100));
    // ����Ϊ͸������
    ModifyStyleEx(0, WS_EX_LAYERED);
    SetLayeredWindowAttributes(0, 100, LWA_ALPHA/*����͸��*/);
    // �������
    m_hCursorOld = SetCursor(hCursor);
    // ��ʼֵ
    ptLast.x = ptLast.y = -1;
    ptStart.x = ptStart.y = -1;
    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}

void CDialogScreenShot::OnLButtonDown(UINT nFlags, CPoint point)
{
    //��¼��һ����
    ptStart = point;
    CDialogEx::OnLButtonDown(nFlags, point);
}

void CDialogScreenShot::AdjustRect(CRect& rc)
{
    // ϵͳ ��Ļ�ֱ���
    int m_iXScreen{ GetSystemMetrics(SM_CXSCREEN)};
    int m_iYScreen{ GetSystemMetrics(SM_CYSCREEN)};
#define ADJ(a, _min, _max) do { if (a<_min) {a = _min; } if (a>_max) {a=_max;}} while(0)
    ADJ(rc.left, 0, m_iXScreen);
    ADJ(rc.top, 0, m_iYScreen);
    ADJ(rc.right, 0, m_iXScreen);
    ADJ(rc.bottom, 0, m_iYScreen);
}

void CDialogScreenShot::OnLButtonUp(UINT nFlags, CPoint point)
{
    TRACE("%d,%d\r\n", point.x, point.y);
    //��ȡ��������,��ͼ
    CRect rc = { ptStart.x, ptStart.y, point.x, point.y };
    // �ж� ѡȡ�����Ƿ���ȷ
    rc.NormalizeRect();
    if(!(rc.IsRectEmpty() && rc.IsRectNull())) {
        m_RectSelect = rc;
        AdjustRect(m_RectSelect);
    }
    //�ָ����
    SetCursor(m_hCursorOld);
    // �ָ�Ĭ��ֵ
    ptLast.x = ptLast.y = -1;
    ptStart.x = ptStart.y = -1;
    CDialogEx::OnLButtonUp(nFlags, point);
    // ����ѡȡֵ,ȷ�����ؽ��(IDOK or IDCANCEL)
    m_RectSelect.NormalizeRect();
    if(m_RectSelect.IsRectEmpty() || m_RectSelect.IsRectNull()) {
        OnCancel();
    }
    else {
        OnOK();
    }
}

void CDialogScreenShot::OnMouseMove(UINT nFlags, CPoint point)
{
    // ÿ���ƶ���Ҫ�������ù��
    SetCursor(hCursor);
    // ��� ������ǰ���״̬
    if(nFlags == MK_LBUTTON) {
        CClientDC aDc(this);
        // ��ɫģʽ
        aDc.SetROP2(R2_NOTXORPEN);
        // �ػ��ͼ��
        if(ptLast.x > 0 && ptLast.y > 0) {
            RECT rc = {ptStart.x, ptStart.y, ptLast.x, ptLast.y};
            // �����ϴεľ���
            aDc.Rectangle(&rc);
            // �Ա���ɫ�ػ�
            aDc.FillRect(&rc, &m_bkBrush);
        }
        //���Ʊ��ε�
        {
            RECT rc = {ptStart.x, ptStart.y, point.x, point.y};
            aDc.Rectangle(&rc);
            aDc.FillSolidRect(&rc, RGB(255, 0, 0));
        }
        //���汾�ε�
        ptLast = point;
    }
    CDialogEx::OnMouseMove(nFlags, point);
}

HBRUSH CDialogScreenShot::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
    if(nCtlColor == CTLCOLOR_DLG) {
        return (HBRUSH)m_bkBrush; //�����»���,�ػ汳��ɫ
    }
    return hbr;
}

BOOL CDialogScreenShot::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    return 0; //�����겻ͣ����������
    //return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}

// ����ѡȡ������
CRect CDialogScreenShot::GetSelectRect() const
{
    return m_RectSelect;
}


void CDialogScreenShot::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	CDialogEx::OnOK();
}


void CDialogScreenShot::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	CDialogEx::OnCancel();
}
