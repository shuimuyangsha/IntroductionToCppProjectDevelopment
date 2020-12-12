#pragma once


// CMyPic

class CMyPic : public CStatic 
{
	DECLARE_DYNAMIC(CMyPic)

public:
	CMyPic();
	virtual ~CMyPic();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


