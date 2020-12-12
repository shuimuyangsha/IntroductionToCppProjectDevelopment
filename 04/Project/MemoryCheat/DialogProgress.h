#pragma once
#include "afxwin.h"


// CDialogProgress �Ի���

class CDialogProgress : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogProgress)

public:
    CDialogProgress(CWnd *pParent = NULL);   // ��׼���캯��
    virtual ~CDialogProgress();

// �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_PROGRESS_LIST };
#endif

protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV ֧��

    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();

    // 32λ�����б�
    CListCtrl m_lst;
    // �����б��ͼ��
    CImageList m_imgList;
    // ��ȡ32λ����,�����µ���������
    BOOL GetProcessList();

public:
    // ��ѡѡ�еĽ��̵�ID
    static DWORD m_dwProcessId;
    // ¼��ѡ�еĽ�����
    static CString m_strProcessName;

    // ˫�������б��¼�(��ѡ����һ��ʱ, ���� m_dwProcessId �� m_strProcessName,���رձ��Ի���)
    afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
};
