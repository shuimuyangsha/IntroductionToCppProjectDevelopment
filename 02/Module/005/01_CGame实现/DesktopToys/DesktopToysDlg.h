
// DesktopToysDlg.h : ͷ�ļ�
//

#pragma once


// CDesktopToysDlg �Ի���
class CDesktopToysDlg : public CDialogEx
{
// ����
public:
    CDesktopToysDlg(CWnd *pParent = NULL);  // ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DESKTOPTOYS_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV ֧��


// ʵ��
protected:
    HICON m_hIcon;

    // ���ɵ���Ϣӳ�亯��
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();

    virtual BOOL PreTranslateMessage(MSG *pMsg);


    DECLARE_MESSAGE_MAP()

    // ��������ͼƬ
    Bitmap *bmp;
public:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
    // ������Ϸ����ת���� CGame�������
    std::shared_ptr<CGame> m_pGame;
};
