#pragma once
#include "afxcmn.h"

#include "MMListCtrl.h"

// CDialogHuShen �Ի���

class CDialogHuShen : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogHuShen)

public:
    CDialogHuShen(CWnd *pParent = NULL);   // ��׼���캯��
    virtual ~CDialogHuShen();

// �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_HUSHEN };
#endif

protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV ֧��

    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnSize(UINT nType, int cx, int cy);

    CMMListCtrl m_lst;
    enum {
        EListIndexNo = 0, // ����
        EListIndexCode, // ����
        EListIndexName, // ����
        EListIndexZhangFu, // �Ƿ�
        EListIndexXianJian, // �ּ�
        EListIndexZhangDie,  // �ǵ�
        EListIndexBuy, // ���
        EListIndexSell, // ����
        EListIndexZongLiang,   // ����
        EListIndexOpen, // ��
        EListIndexHigh, // ���
        EListIndexLow, // ���
        EListIndexZuoShou, // ����
        EListIndexZongJinE, // �ܽ��
        EListIndexMaxLimit, // ����
    };

private:
    enum {MaxCountOnePage = 28}; //һҳ��ʾ������

    // ��������
    void LoadData(int iPage);
public:
    virtual BOOL PreTranslateMessage(MSG *pMsg);
    // ��������ʵʱ���ݵ��߳�
    void StartUpdate();
    // ֹͣ����ʵʱ���ݵ��߳�
    void StopUpdate();
    // �߳̾��
    HANDLE m_hThread{NULL};
    // �߳�ID
    unsigned m_threadID{0};
    // �̺߳���
    static unsigned __stdcall ThreadRoutine(PVOID pArgs);
    afx_msg void OnBnClickedButtonFilter();
    afx_msg void OnBnClickedButtonAll();

    // ��ʾ�����
    enum EShowStaus {EAll, // ȫ��
                     EFilter, // ����ѡ��
                    };
    // ��ǰ��ʾ�����
    EShowStaus m_status{EAll};
    afx_msg void OnBnClickedButtonShowFilter();

    // ����ѡ��ѡ������Ĺ�Ʊ
    std::vector<std::wstring> m_vFilterStocks;
};
