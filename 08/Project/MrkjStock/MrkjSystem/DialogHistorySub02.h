#pragma once
#include "StockChart.h"


// CDialogHistorySub02 �Ի���

class CDialogHistorySub02 : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogHistorySub02)

public:
    CDialogHistorySub02(CWnd* pParent = NULL);   // ��׼���캯��
    virtual ~CDialogHistorySub02();

// �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_HISTORY_SUB_02 };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    DECLARE_MESSAGE_MAP()
public:
    CStockChart m_stockChart;
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnSize(UINT nType, int cx, int cy);

public:
    // �ṩ�������ڵ��õĽӿ�
    void UpdateKLine(const CString& strCode, const CString& strDateStart, const CString& strDateEnd);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};
