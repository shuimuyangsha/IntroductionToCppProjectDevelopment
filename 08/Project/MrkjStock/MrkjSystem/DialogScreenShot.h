#pragma once


// CDialogScreenShot �Ի���

class CDialogScreenShot : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogScreenShot)

public:
    CDialogScreenShot(CWnd* pParent = NULL);   // ��׼���캯��
    virtual ~CDialogScreenShot();

// �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_SCREEN_SHOT };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    DECLARE_MESSAGE_MAP()
private:
    // ��¼���ѡ�������
    CRect m_RectSelect{ -1, 1, -1, -1};
    // ����ѡ������
    void AdjustRect(CRect& rc);
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);

    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    // ������ˢ
    CBrush m_bkBrush{};
    // ���
    HCURSOR hCursor{ LoadCursor(NULL, IDC_CROSS) };
    // ԭ���Ĺ��
    HCURSOR m_hCursorOld{};
    // ����ѡȡ������
    CRect GetSelectRect() const;
	virtual void OnOK();
	virtual void OnCancel();
};
