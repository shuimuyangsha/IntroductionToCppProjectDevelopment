#pragma once
#include "afxwin.h"


// CDialogProgress 对话框

class CDialogProgress : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogProgress)

public:
    CDialogProgress(CWnd *pParent = NULL);   // 标准构造函数
    virtual ~CDialogProgress();

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_PROGRESS_LIST };
#endif

protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();

    // 32位进程列表
    CListCtrl m_lst;
    // 进程列表的图标
    CImageList m_imgList;
    // 获取32位进程,并更新到进程列中
    BOOL GetProcessList();

public:
    // 当选选中的进程的ID
    static DWORD m_dwProcessId;
    // 录脱选中的进程名
    static CString m_strProcessName;

    // 双击进程列表事件(当选中了一行时, 设置 m_dwProcessId 和 m_strProcessName,并关闭本对话框)
    afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
};
