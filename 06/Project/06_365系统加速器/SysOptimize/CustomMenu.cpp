// CustomMenu.cpp: implementation of the CCustomMenu class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SysOptimize.h"
#include "CustomMenu.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomMenu::CCustomMenu()
{
	m_ImageIndex = 0;
}

CCustomMenu::~CCustomMenu()
{

}
// 绘制菜单项文本
void CCustomMenu::DrawItemText(CDC *pDC, LPSTR str, CRect rect)
{
	pDC->DrawText(str,rect,DT_SINGLELINE|DT_VCENTER|DT_CENTER);
}
// 绘制菜单项背景图片
void CCustomMenu::DrawComMenu(CDC *pDC, CRect rect, BOOL select)
{
	if(select)
	{
		pDC->SelectStockObject(BLACK_PEN);
// 		rect.DeflateRect(0,1,0,2);
		pDC->Rectangle(rect);
		CBitmap m_bitmap;
		m_bitmap.LoadBitmap(IDB_ITEMBITMAP);
		BITMAP m_size;
		m_bitmap.GetBitmap(&m_size);
		CDC m_memdc;
		m_memdc.CreateCompatibleDC(pDC);
		CGdiObject* m_oldobject;
		m_oldobject = m_memdc.SelectObject(&m_bitmap);
		pDC->StretchBlt(rect.left+1,rect.top+1,rect.Width()-2,rect.Height()-2,
			&m_memdc,0,0,m_size.bmWidth,m_size.bmHeight,SRCCOPY);
		m_bitmap.DeleteObject();
	}
	else
	{
		pDC->FillSolidRect(rect,RGB(255,255,255));
	}
}
// 绘制分隔条
void CCustomMenu::DrawSeparater(CDC *pDC, CRect rect)
{
	if(pDC != NULL)
	{
		pDC->Draw3dRect(rect,RGB(0,0,255),RGB(0,0,255));
	}
}
// 修改菜单项信息
BOOL CCustomMenu::ChangeMenuItem(CMenu *menu)
{
	int m_index=0;
	if(menu != NULL)
	{
		int m_itemcount = menu->GetMenuItemCount(); // 获得菜单项个数
		for(int i=0;i<m_itemcount;i++)
		{
			// 获得菜单项文本
			menu->GetMenuString(i,m_ItemLists[m_index].m_ItemText,MF_BYPOSITION);
			int m_itemID = menu->GetMenuItemID(i); // 获得菜单项ID
			m_ItemLists[m_index].m_ItemID = m_itemID;
			if(m_itemID>0)
			{
				m_ItemLists[m_index].m_ItemIndex= m_ImageIndex;
				m_ImageIndex++;
			}
			// 修改菜单项信息
			menu->ModifyMenu(i,MF_OWNERDRAW | MF_BYPOSITION | MF_STRING,
				m_ItemLists[m_index].m_ItemID,(LPSTR)&(m_ItemLists[m_index]));
			m_index++;
			CMenu* m_subMenu = menu->GetSubMenu(i);
			if(m_subMenu)
			{
				ChangeMenuItem(m_subMenu);
			}
		}
	}
	return TRUE	;
}
// 设置菜单项大小
void CCustomMenu::MeasureItem(LPMEASUREITEMSTRUCT lpStruct)
{
	if(lpStruct->CtlType==ODT_MENU)
	{
		lpStruct->itemHeight = 22; // 设置菜单项的高
		lpStruct->itemWidth = 70; // 设置菜单项的宽
		CMenuItemInfo* m_iteminfo;
		m_iteminfo = (CMenuItemInfo*)lpStruct->itemData;
		if(m_iteminfo->m_ItemID == 0)
		{
			lpStruct->itemHeight = 1; // 设置分隔条
		}
	}
}
// 重绘菜单项
void CCustomMenu::DrawItem(LPDRAWITEMSTRUCT lpStruct)
{
	if(lpStruct->CtlType==ODT_MENU)
	{
		if(lpStruct->itemData == NULL)	return;
		unsigned int m_state = lpStruct->itemState; // 获得菜单项状态
		CDC* pDC = CDC::FromHandle(lpStruct->hDC);
		CString str = ((CMenuItemInfo*)(lpStruct->itemData))->m_ItemText;
		LPSTR m_str = str.GetBuffer(str.GetLength());
		int m_itemID = ((CMenuItemInfo*)(lpStruct->itemData))->m_ItemID;	
		int m_itemicon = ((CMenuItemInfo*)(lpStruct->itemData))->m_ItemIndex;
		CRect m_rect = lpStruct->rcItem;	
		pDC->SetBkMode(TRANSPARENT); // 设置菜单项文本背景透明	
		switch(m_itemID)
		{
		case 0: // 绘制分隔条
				DrawSeparater(pDC,m_rect);
				break;
		default: // 绘制菜单项和左侧标题
				DrawComMenu(pDC,m_rect,m_state&ODS_SELECTED);
				DrawItemText(pDC,m_str,m_rect);			
				break;
		}			
	}
}

