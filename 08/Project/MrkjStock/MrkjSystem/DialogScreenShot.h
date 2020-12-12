#pragma once


// CDialogScreenShot 对话框

class CDialogScreenShot : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogScreenShot)

public:
    CDialogScreenShot(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~CDialogScreenShot();

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_SCREEN_SHOT };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
private:
    // 记录最后选择的区域
    CRect m_RectSelect{ -1, 1, -1, -1};
    // 调整选择区域
    void AdjustRect(CRect& rc);
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);

    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    // 背景画刷
    CBrush m_bkBrush{};
    // 光标
    HCURSOR hCursor{ LoadCursor(NULL, IDC_CROSS) };
    // 原来的光标
    HCURSOR m_hCursorOld{};
    // 返回选取的区域
    CRect GetSelectRect() const;
	virtual void OnOK();
	virtual void OnCancel();
};
