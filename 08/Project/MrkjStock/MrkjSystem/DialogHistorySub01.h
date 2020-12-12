#pragma once
#include "afxcmn.h"


// CDialogHistorySub01 �Ի���

class CDialogHistorySub01 : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogHistorySub01)

public:
    CDialogHistorySub01(CWnd* pParent = NULL);   // ��׼���캯��
    virtual ~CDialogHistorySub01();

// �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_HISTORY_SUB_01 };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnSize(UINT nType, int cx, int cy);

    CListCtrl m_lst;
    enum {
        EListIndexCode = 0,
        EListIndexName,
        EListIndexUpPercent,
        EListIndexPrice,
        EListIndexMax,
        EListIndexOpen,
        EListIndexClose,
        EListIndexMin,
        EListIndexTurnover,
        EListIndexTradingVolume,
        EListIndexMaxLimit,
    };

    typedef tuple<CString, CString, CString, CString, CString, CString, CString, CString, CString, CString> TupleLineData;
    typedef vector<TupleLineData> VLineData;

    // ����m_lst�е�����
    void UpdateListData(const VLineData& v);

    // ���ĳ������
    void GetListData(TupleLineData& data, int index);

public:
    //�ṩ��������е��õĽӿ�
    void UpdateStockData(const CString& stockCode, const CString& strDateStart, const CString strDateEnd);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
