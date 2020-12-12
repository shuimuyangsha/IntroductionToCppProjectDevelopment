#pragma once
#include "afxcmn.h"


// CDialogKindSelect �Ի���

class CDialogKindSelect : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogKindSelect)

public:
    CDialogKindSelect(CWnd* pParent = NULL);   // ��׼���캯��
    virtual ~CDialogKindSelect();

// �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_KIND_SELECT };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    DECLARE_MESSAGE_MAP()

private:
    CBitmap m_bmpTree;
    CImageList m_imgListTree;
    CImageList m_imgListList;
    CBitmap m_bmpList;
    CString GetSelectStockCode() const;

    // ȷ��ʱ ָ��ѡ�еĹ�Ʊ
    static CString m_strSelectCode;
public:
    CTreeCtrl m_treeKind;
    CListCtrl m_lst;
    afx_msg void OnBnClickedButtonOk();
    afx_msg void OnBnClickedButtonCancel();
    virtual void OnOK();
    virtual void OnCancel();
    virtual BOOL OnInitDialog();

    void UpdateTree();

    afx_msg void OnNMClickTreeKind(NMHDR *pNMHDR, LRESULT *pResult);

    // ����
    void UpdateList(const vector<CMRKJDatabase::TupleStockInfo>& v);
    // ����б��е�����
    void ClearList();
    // ���ѡ�е�,Ҫ��ѯ�Ĺ�Ʊ����
    CString GetTargetStockCode() const
    {
        return m_strSelectCode;
    };
    afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
