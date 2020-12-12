// CustomMenu.h: interface for the CCustomMenu class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUSTOMMENU_H__5E0963CD_D37A_4BEA_B1E7_F092682907BA__INCLUDED_)
#define AFX_CUSTOMMENU_H__5E0963CD_D37A_4BEA_B1E7_F092682907BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct CMenuItemInfo
{
	CString m_ItemText;  // 菜单项文本
	int     m_ItemIndex; // 菜单项索引
	int     m_ItemID;    // 菜单标记 0分隔条,其他普通菜单
};

class CCustomMenu  :  public CMenu
{
public:
	CCustomMenu();
	virtual ~CCustomMenu();

	CMenuItemInfo m_ItemLists[10]; // 菜单项信息
	int			  m_ImageIndex;    // 图像索引
	void DrawItemText(CDC* pDC,LPSTR str,CRect rect);      // 绘制菜单项文本
	void DrawComMenu(CDC* pDC,CRect rect, BOOL select);    // 绘制菜单项背景图片
	void DrawSeparater(CDC* pDC,CRect rect);               // 绘制分隔条
	BOOL ChangeMenuItem(CMenu* menu);                      // 修改菜单项信息
	virtual void DrawItem(LPDRAWITEMSTRUCT lpStruct);		// 重绘菜单项
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpStruct); // 设置菜单项大小
};

#endif // !defined(AFX_CUSTOMMENU_H__5E0963CD_D37A_4BEA_B1E7_F092682907BA__INCLUDED_)
