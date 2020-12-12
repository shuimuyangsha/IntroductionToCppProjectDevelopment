#pragma once
#include "afxcmn.h"


#include "DialogCalc01.h"
#include "DialogCalc02.h"
#include "DialogCalc03.h"
#include "DialogCalc04.h"
#include "DialogCalc05.h"

// CDialogCalc �Ի���

class CDialogCalc : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogCalc)

public:
    CDialogCalc(CWnd* pParent = NULL);   // ��׼���캯��
    virtual ~CDialogCalc();

// �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_CALC };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    DECLARE_MESSAGE_MAP()
public:
    CTreeCtrl m_tree;
    afx_msg void OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
    virtual BOOL OnInitDialog();
    CDialogCalc01 dlg01; // ��Ʊ���������
    CDialogCalc02 dlg02; // ��Ʊ�ǵ�ͣ������
    CDialogCalc03 dlg03; // ������������
    CDialogCalc04 dlg04; // ������ǰ�������
    CDialogCalc05 dlg05; // ����˰����
    std::vector<CDialogEx*> m_vDlgPtr;
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
