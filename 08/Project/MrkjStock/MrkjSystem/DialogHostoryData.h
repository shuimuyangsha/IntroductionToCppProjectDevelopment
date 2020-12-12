#pragma once
#include "afxcmn.h"
#include "ATLComTime.h"

#include "DialogHistorySub01.h"
#include "DialogHistorySub02.h"
#include "afxwin.h"


// CDialogHostoryData �Ի���

class CDialogHostoryData : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogHostoryData)

public:
    CDialogHostoryData(CWnd* pParent = NULL);   // ��׼���캯��
    virtual ~CDialogHostoryData();

// �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_HOSTORY_DATA };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    DECLARE_MESSAGE_MAP()

private:
    CDialogHistorySub01* m_pDlg01{new CDialogHistorySub01()};
    CDialogHistorySub02* m_pDlg02{new CDialogHistorySub02()};
public:
    //
    CTabCtrl m_tab;
    // ��ʼ����
    COleDateTime m_odtDateStart;
    // ��������
    COleDateTime m_odtDateEnd;

    virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnDestroy();
    afx_msg void OnBnClickedButtonQuery();
    afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);

    // ��Ʊ�����������
    CString m_strCode;
    // ��Ʊ����
    CComboBox m_cbbStockCode;
    // ��������
    void ClearComboBox();

    // ��������������,���㱾����ʹ��
    typedef CMRKJDatabase::TupleStockInfo INFO, *PINFO;
    // ���¹�Ʊ����
    void UpdateStockInfoComboBox();
    // ���ѡ�еĹ�Ʊ����
    bool GetSelStockCode(CString& strCode);
    // ����ѡ����
    void SetSelByStockCode(const CString& strCode);
    // ʹ������ʾ������
    void SetDataAndUpdate(const CString& stockCode, const COleDateTime& m_odtDateStart, const COleDateTime& m_odtDateEnd);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};
