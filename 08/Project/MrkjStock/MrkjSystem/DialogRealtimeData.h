#pragma once
#include "afxwin.h"

#include "HttpDataEntry.h"

// CDialogRealtimeData 对话框

class CDialogRealtimeData : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogRealtimeData)

public:
    CDialogRealtimeData(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~CDialogRealtimeData();

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_REALTIME_DATA };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()

private:
    // 当查询线程查询结束后.会触发此消息
    afx_msg LRESULT OnMsgHttpQueryEnd(WPARAM wParam, LPARAM lParam);
    // 重命名 数据类型方便用
    typedef CMRKJDatabase::TupleStockInfo INFO, *PINFO;
    // 获得 目标 股票代码
    bool GetSelStockCode(CString& strCode);
	// 清理CComboBox各项数据
	void ClearList();
    // 存储 实时数据
    data_http::CHttpDataEntry m_httpData{};
    // 下载的K线图路径
    std::wstring m_strFilePath;
    // 股票代码
    CComboBox m_cbbStockCode;
    CString m_strCode;
public:
    enum WM_REAL_TIME { sc_MSG_MIN = (WM_USER + 101),
                        sc_MSG_HTTP_QUERY_END, //数据查询完毕
                      };
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual void OnCancel();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnPaint();
    afx_msg void OnBnClickedButtonQuery();
	afx_msg void OnDestroy();

    // 更新列表框内容(股票信息)
    void UpdateStockInfoComboBox();
};
