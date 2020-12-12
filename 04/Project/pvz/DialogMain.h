#pragma once
#include "afxwin.h"


// CDialogMain �Ի���

class CDialogMain : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogMain)

public:
    CDialogMain(CWnd *pParent = NULL);   // ��׼���캯��
    virtual ~CDialogMain();

// �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG1 };
#endif

protected:
    HICON m_hIcon;

    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV ֧��

    DECLARE_MESSAGE_MAP()


public:
    virtual BOOL OnInitDialog();

    // ���� ��ֲ��û��CD
    afx_msg void OnBnClickedButtonNocd();
    // ���� �����ص���ֲ
    afx_msg void OnBnClickedButtonAnywhere();
    // ���� �����Զ��ռ����⣬��Ǯ
    afx_msg void OnBnClickedButtonAutoCollect();
    // ���� ��ʬȫ������
    afx_msg void OnBnClickedButtonAllZombies();
    // ��������
    DWORD m_dwSunlight;
    // ���� ��������
    afx_msg void OnBnClickedButtonSunlight();
    // ��Ǯ
    DWORD m_dwGold;
    // ���� ��������
    afx_msg void OnBnClickedButtonGold();
    // Ҫ��ֲ��ֲ��
    CComboBox m_comboPlants;
    // ���� x
    CComboBox m_comboCropX;
    // ���� y
    CComboBox m_comboCropY;
    // ��ֲ
    afx_msg void OnBnClickedButtonCrop();
    // ��ֲһ��
    afx_msg void OnBnClickedButtonCropRow();
    // ��ֲһ��
    afx_msg void OnBnClickedButtonColumn();
    // �Ƴ�ֲ��
    afx_msg void OnBnClickedButtonRemoveplant();
    // ��ģ�õ�ֲ��
    CComboBox m_comboPlants2;
    afx_msg void OnCbnSelchangeComboPlants();
    // ��ñ�ģ��ֲ���ID
    DWORD GetPlants2Id();
};
