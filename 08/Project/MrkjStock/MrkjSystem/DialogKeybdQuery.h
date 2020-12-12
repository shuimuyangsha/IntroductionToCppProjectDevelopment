#pragma once
#include "afxwin.h"


// CDialogKeybdQuery 对话框

class CDialogKeybdQuery : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogKeybdQuery)

public:
    CDialogKeybdQuery(const CString& strInput, int x, int y, CWnd* pParent = NULL);   // 标准构造函数
    virtual ~CDialogKeybdQuery();

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_KEYBD_QUERY };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
private:
    /*初始化时设置*/
    // 第一次输入的第一个字母
    CString m_strQueryInput;
    // 即将显示的右下角位置
    int m_x, m_y;

    /*对话框退出时返回选中的内容*/
    CString m_strQueryResultSel;

public:
    // 获得用户的选择:是一个股票代码
    CString GetResult()
    {
        return m_strQueryResultSel;
    }

    // 查询的输入
    CEdit m_edtQueryInput;
    // 查询的结果
    CListBox m_lstBoxQueryResult;

    // 对话框初始化
    virtual BOOL OnInitDialog();
    // 输入内容发生变化
    afx_msg void OnEnChangeEditQueryInput();
    // 双击了查询结果
    afx_msg void OnLbnDblclkListQueryResult();
    afx_msg void OnEnSetfocusEditQueryInput();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnOK();
	virtual void OnCancel();
};
