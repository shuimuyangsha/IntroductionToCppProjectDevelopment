#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "ATLComTime.h"
#include "afxcmn.h"


// CDialogDataMaintenance �Ի���

class CDialogDataMaintenance : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogDataMaintenance)

public:
    CDialogDataMaintenance(CWnd* pParent = NULL);   // ��׼���캯��
    virtual ~CDialogDataMaintenance();

// �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_DATA_MAINTENANCE };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    // ��Ʊ����
    CEdit m_edtCode;
    CString m_strCode;

    // ��Ʊ����
    CEdit m_edtName;
    CString m_strName;

    // ��ʼ����
    CDateTimeCtrl m_dtcDateStart;
    COleDateTime m_odtDateStart;

    // ��������
    CDateTimeCtrl m_dtcDateEnd;
    COleDateTime m_odtDateEnd;

    // ��ƱƷ��
    CComboBox m_cbbKind;
    CString m_strKind;

    // ���̼�
    CEdit m_edtOpen;
    CString m_strOpen;

    // ���̼�
    CEdit m_edtClose;
    CString m_strClose;

    // ��߼�
    CEdit m_edtHigh;
    CString m_strHigh;

    // ��ͼ�
    CEdit m_edtLow;
    CString m_strLow;

    // �ɽ���
    CEdit m_edtTurnover;
    CString m_strTurnover;

    // �ɽ���
    CEdit m_edtTradingvolume;
    CString m_strTradingvolume;
    // ��Ʊ����ʷ����
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

    // ����ɵ�����
    CString m_strOldDate{_T("")};
    // �޸İ�ť
    CButton m_btnUpdate;
    // ���水ť
    CButton m_btnSave;

	// ����������Ϣ
	void UpdateKindList();
    // ����m_lst�е�����
    void UpdateListData(const VDayDataEntry& v, const CMRKJDatabase::TupleStockInfo& info);
    // ���ĳ������
    void GetListData(CDayDataEntry& data, int index);
    // ���¹�Ʊ��Ϣ
    void UpdateStockInfo(const CMRKJDatabase::TupleStockInfo& info);
    // ��õ�ǰѡ��� ����id
    bool GetKindIdSel(int& id);
    // ���ؼ������ĺϷ��� : ��������
    bool IsInputValid();

    afx_msg void OnBnClickedButtonQuery();
    afx_msg void OnBnClickedButtonAdd();
    afx_msg void OnBnClickedButtonUpdate();
    afx_msg void OnBnClickedButtonSave();
    afx_msg void OnBnClickedButtonDel();
    afx_msg void OnBnClickedButtonQueryStockInfo();
    afx_msg void OnBnClickedButtonAddStockInfo();
    afx_msg void OnBnClickedButtonUpdateStockInfo();
    afx_msg void OnBnClickedButtonDelteStockInfo();
    afx_msg void OnNMClickListData(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
