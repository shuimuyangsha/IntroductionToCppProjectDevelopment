#pragma once
#include "afxcmn.h"


// CDialogDataImpExp �Ի���

class CDialogDataImpExp : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogDataImpExp)

public:
    CDialogDataImpExp(CWnd* pParent = NULL);   // ��׼���캯��
    virtual ~CDialogDataImpExp();

// �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_DATA_IMPEXP };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    DECLARE_MESSAGE_MAP()
    virtual void OnOK();
    virtual void OnCancel();
public:
    virtual BOOL OnInitDialog();
    // �����ļ�·��
    CString m_strImportFilePath;
    // �����ļ�·��
    CString m_strExportFilePath;
    // ��Ʊ����
    CString m_strCode;
    // ��Ʊ����ʷ����
    CListCtrl m_lst;
    enum {
        EListIndexCode = 0,
        EListIndexName,
        EListIndexUpPercent,
        EListIndexPrice,
        EListIndexOpen,
        EListIndexClose,
        EListIndexMax,
        EListIndexMin,
        EListIndexTurnover,
        EListIndexTradingVolume,
        EListIndexMaxLimit,
    };

    // ����m_lst�е�����
    void UpdateListData(const VDayDataEntry& v, const CMRKJDatabase::TupleStockInfo& info);
    // ���ĳ������
    void GetListData(std::vector<CString>& data, int index);

    afx_msg void OnBnClickedButtonLookupImport();
    afx_msg void OnBnClickedButtonLookupExport();
    afx_msg void OnBnClickedButtonQuery();
    afx_msg void OnBnClickedButtonImport();
    afx_msg void OnBnClickedButtonExport();
    afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonImportTdx();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
