
// DesktopToysDlg.h : ͷ�ļ�
//

#pragma once


// CDesktopToysDlg �Ի���
class CDesktopToysDlg : public CDialogEx
{
// ����
public:
    CDesktopToysDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DESKTOPTOYS_DIALOG };
#endif

    protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
    HICON m_hIcon;

    // ���ɵ���Ϣӳ�亯��
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()

    // ��������ͼƬ
    Bitmap* bmp;
};
