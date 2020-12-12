
// MrkjSystemDlg.h : ͷ�ļ�
//

#pragma once


#include "DialogAbout.h"
#include "DialogCalc.h"
#include "DialogDataImpExp.h"
#include "DialogDataKind.h"
#include "DialogDataMaintenance.h"
#include "DialogHostoryData.h"
#include "DialogKindSelect.h"
#include "DialogRealtimeData.h"
#include "DialogSetting.h"
#include "DialogHuShen.h"


// CMrkjSystemDlg �Ի���
class CMrkjSystemDlg : public CDialogEx
{
    // ����
public:
    CMrkjSystemDlg(CWnd *pParent = NULL);   // ��׼���캯��
    ~CMrkjSystemDlg();
// �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_MRKJSYSTEM_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV ֧��


// ʵ��
protected:
    HICON m_hIcon;
    // �˵���
    CMenu m_menu;
    //״̬��
    CStatusBar m_statusBar;

    // ������ˢ
    CBrush m_bkBrush;

    // ���ɵ���Ϣӳ�亯��
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL PreTranslateMessage(MSG *pMsg);
    virtual void OnOK();
    virtual void OnCancel();

    // ������ : ���ı�����ˢ
    afx_msg HBRUSH OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor);

    //����ά��
    afx_msg void OnMenuDataMaintenance();

    // ���ݵ��뵼��
    afx_msg void OnMenuDataImpexp();

    // ����ά��
    afx_msg void OnMenuDataKind();
    // ��ӡ
    afx_msg void OnMenuPrint();

    // �˳�ϵͳ
    afx_msg void OnMenuExitSys();

    // ����ѡ��
    afx_msg void OnMenuKindSelect();

    // ��ʷ����
    afx_msg void OnMenuHostoryData();

    // ʵʱ����
    afx_msg void OnMenuRealtimeData();

    // ���ڼ�����
    afx_msg void OnMenuCalc();

    // ��Ļ��ͼ
    afx_msg void OnMenuScreenShot();

    // ϵͳ����
    afx_msg void OnMenuSetting();

    // ����
    afx_msg void OnMenuHelp();

    // ���ڱ�ϵͳ
    afx_msg void OnMenuAbout();

private:

    // ��ҳ ��������Ʊ
    CDialogHuShen *m_pDlgHuShen{ new CDialogHuShen()};

    // �ӶԻ��� : ����ά��
    CDialogDataMaintenance *m_pDlgDataMaintenance{new CDialogDataMaintenance()};

    // �ӶԻ��� : ���ݵ��뵼��
    CDialogDataImpExp *m_pDlgDataImpExp{new CDialogDataImpExp()};

    // �ӶԻ��� : ����ά��
    CDialogDataKind *m_pDlgDataKind{new CDialogDataKind()};

    // �ӶԻ��� : ����ѡ��
    CDialogKindSelect *m_pDlgKindSelect{new CDialogKindSelect()};

    // �ӶԻ��� : ��ʷ����
    CDialogHostoryData *m_pDlgHostoryData{new CDialogHostoryData()};

    // �ӶԻ��� : ʵʱ����
    CDialogRealtimeData *m_pDlgRealtimeData{new CDialogRealtimeData()};

    // �ӶԻ��� : ������
    CDialogCalc *m_pDlgCalc{new CDialogCalc()};

    // �ӶԻ��� : ��Ļ��ͼ
    // û�жԻ���

    // �ӶԻ��� : ϵͳ����
    CDialogSetting *m_pDlgSetting{new CDialogSetting()};

    // �ӶԻ��� : ����
    // û�жԻ���, ������ҳ

    // �ӶԻ��� : ����
    // ģ̬�Ի���, ���ض���ɳ�Ա����

    // �潲���жԻ����ָ��
    std::vector<CDialogEx *> m_vDlgPtr;

    //ֻ��ʾ�Լ��Ĵ���,���������,�������ϴ���ʾ�Ĵ���
    void ShowChange(CDialogEx *pDlg);

    //��¼��ǰ������ʾ�ĶԻ���
    CDialogEx *m_pDlgLastShow{nullptr};

public:
    afx_msg void OnMenuRealHushen();
};
