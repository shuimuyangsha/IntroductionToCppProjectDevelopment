// DialogCalc.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MrkjSystem.h"
#include "DialogCalc.h"
#include "afxdialogex.h"
#include "DialogCalc01.h"
#include "DialogCalc02.h"
#include "DialogCalc03.h"
#include "DialogCalc04.h"
#include "DialogCalc05.h"


// CDialogCalc �Ի���

IMPLEMENT_DYNAMIC(CDialogCalc, CDialogEx)

CDialogCalc::CDialogCalc(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_DIALOG_CALC, pParent)
{
}

CDialogCalc::~CDialogCalc()
{
}

void CDialogCalc::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TREE1, m_tree);
}


BEGIN_MESSAGE_MAP(CDialogCalc, CDialogEx)
    ON_NOTIFY(NM_CLICK, IDC_TREE1, &CDialogCalc::OnNMClickTree1)
END_MESSAGE_MAP()


// CDialogCalc ��Ϣ�������


void CDialogCalc::OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
    DWORD dwPos = GetMessagePos();
    TVHITTESTINFO ht = {0};
    ht.pt.x = GET_X_LPARAM(dwPos);
    ht.pt.y = GET_Y_LPARAM(dwPos);
    ::MapWindowPoints(HWND_DESKTOP, pNMHDR->hwndFrom, &ht.pt, 1);
    TreeView_HitTest(pNMHDR->hwndFrom, &ht);
    HTREEITEM hItem = ht.hItem;
    if(hItem) {
        struct {
            CString str;
            CDialogEx* pDlg;
        } keymap[] = {
            {_T("��Ʊӯ��"), &dlg01},
            {_T("�ǵ�ͣ����"), &dlg02},
            {_T("��������"), &dlg03},
            {_T("��ǰ����"), &dlg04},
            {_T("����˰����"), &dlg05},
        };
        CString str = m_tree.GetItemText(hItem);
        for(auto pp : keymap) {
            if(pp.str == str) {
                pp.pDlg->ShowWindow(SW_SHOW);
            }
            else {
                pp.pDlg->ShowWindow(SW_HIDE);
            }
        }
    }
    *pResult = 0;
}


BOOL CDialogCalc::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    {
        {
            static CBitmap bmp;
            bmp.LoadBitmap(IDB_BITMAP_CALC);
            static CImageList imgList;
            imgList.Create(16, 16, ILC_COLOR32, 1, 1);
            imgList.Add(&bmp, RGB(0, 0, 0));
            m_tree.SetImageList(&imgList, TVSIL_NORMAL);
        }
        HTREEITEM hItem0 = m_tree.InsertItem(_T("��Ʊ��"));
        HTREEITEM hItem1 = m_tree.InsertItem(_T("������"));
        HTREEITEM hItem2 = m_tree.InsertItem(_T("����˰����"));
        HTREEITEM hItem00 = m_tree.InsertItem(_T("��Ʊӯ��"), hItem0);
        HTREEITEM hItem01 = m_tree.InsertItem(_T("�ǵ�ͣ����"), hItem0);
        HTREEITEM hItem10 = m_tree.InsertItem(_T("��������"), hItem1);
        HTREEITEM hItem11 = m_tree.InsertItem(_T("��ǰ����"), hItem1);
        m_tree.Expand(hItem0, TVE_EXPAND);
        m_tree.Expand(hItem1, TVE_EXPAND);
        m_tree.Expand(hItem2, TVE_EXPAND);
        m_tree.SetItemData(hItem00, (DWORD_PTR)&dlg01);
        m_tree.SetItemData(hItem01, (DWORD_PTR)&dlg02);
        m_tree.SetItemData(hItem10, (DWORD_PTR)&dlg03);
        m_tree.SetItemData(hItem11, (DWORD_PTR)&dlg04);
        m_tree.SetItemData(hItem2, (DWORD_PTR)&dlg05);
        dlg01.Create(IDD_DIALOG1, this);
        dlg02.Create(IDD_DIALOG2, this);
        dlg03.Create(IDD_DIALOG3, this);
        dlg04.Create(IDD_DIALOG4, this);
        dlg05.Create(IDD_DIALOG5, this);
        m_vDlgPtr.push_back(&dlg01);
        m_vDlgPtr.push_back(&dlg02);
        m_vDlgPtr.push_back(&dlg03);
        m_vDlgPtr.push_back(&dlg04);
        m_vDlgPtr.push_back(&dlg05);
        CRect rc;
        GetClientRect(rc);
        CRect rcTree;
        m_tree.GetClientRect(rcTree);
        for(auto ptr : m_vDlgPtr) {
            ptr->MoveWindow(rcTree.Width() + 10, rcTree.top, rc.Width() - rcTree.Width() - 40, rcTree.Height());
            ptr->ShowWindow(SW_HIDE);
        }
        m_vDlgPtr[0]->ShowWindow(SW_SHOW);
    }
    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}


void CDialogCalc::OnOK()
{
    // TODO: �ڴ����ר�ô����/����û���
    CDialogEx::OnOK();
}


void CDialogCalc::OnCancel()
{
    // TODO: �ڴ����ר�ô����/����û���
    CDialogEx::OnCancel();
}

BOOL CDialogCalc::PreTranslateMessage(MSG* pMsg)
{
    // ��ֹ ESC/�س��� �Զ��˳�
    if(pMsg->message == WM_KEYFIRST) {
        if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) {
            return TRUE; // ���ط�0,�������Ϣ���������зַ�
        }
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}
