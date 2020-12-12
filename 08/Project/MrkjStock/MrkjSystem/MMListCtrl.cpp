// MMListCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MrkjSystem.h"
#include "MMListCtrl.h"


// CMMListCtrl

IMPLEMENT_DYNAMIC(CMMListCtrl, CMFCListCtrl)

CMMListCtrl::CMMListCtrl()
{

}

CMMListCtrl::~CMMListCtrl()
{
}


BEGIN_MESSAGE_MAP(CMMListCtrl, CMFCListCtrl)
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CMMListCtrl ��Ϣ�������




int CMMListCtrl::InsertItem(int nItem, LPCTSTR szText, COLORREF txtClr, COLORREF txtBk)
{
    int index =  CMFCListCtrl::InsertItem(nItem, szText);
    m_datas[nItem][0].clrTxt = txtClr;
    m_datas[nItem][0].clrBk = txtBk;
    return index;
}

BOOL CMMListCtrl::SetItemText(int nItem, int iSubItem, LPCTSTR szText, COLORREF txtClr, COLORREF txtBk)
{
    BOOL b = CMFCListCtrl::SetItemText(nItem, iSubItem, szText);
    m_datas[nItem][iSubItem].clrTxt = txtClr;
    m_datas[nItem][iSubItem].clrBk = txtBk;
    return b;
}

COLORREF CMMListCtrl::OnGetCellBkColor(int nRow, int nColumn)
{
    try {
        // �ҵ�����ɫ������
        if(m_datas.find(nRow) != m_datas.end()) {
            if(m_datas.at(nRow).find(nColumn) != m_datas.at(nRow).end()) {
                return m_datas.at(nRow).at(nColumn).clrBk;
            }
        }
        // ���� ���غ�ɫ
        return RGB(0, 0, 0);
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return CMFCListCtrl::OnGetCellTextColor(nRow, nColumn);
    }
}

COLORREF CMMListCtrl::OnGetCellTextColor(int nRow, int nColumn)
{
    try {
        // �ҵ�����ɫ������
        if(m_datas.find(nRow) != m_datas.end()) {
            if(m_datas.at(nRow).find(nColumn) != m_datas.at(nRow).end()) {
                return m_datas.at(nRow).at(nColumn).clrTxt;
            }
        }
        // ���� ���غ�ɫ
        return RGB(0, 0, 0);
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return CMFCListCtrl::OnGetCellTextColor(nRow, nColumn);
    }
}

HFONT CMMListCtrl::OnGetCellFont(int nRow, int nColum, DWORD dwData/* = 0*/)
{
    return NULL;
}

BOOL CMMListCtrl::OnEraseBkgnd(CDC *pDC)
{
    //// ��ֹ��˸
    //return FALSE;
    return CMFCListCtrl::OnEraseBkgnd(pDC);
}
