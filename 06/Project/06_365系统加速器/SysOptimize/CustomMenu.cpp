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
// ���Ʋ˵����ı�
void CCustomMenu::DrawItemText(CDC *pDC, LPSTR str, CRect rect)
{
	pDC->DrawText(str,rect,DT_SINGLELINE|DT_VCENTER|DT_CENTER);
}
// ���Ʋ˵����ͼƬ
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
// ���Ʒָ���
void CCustomMenu::DrawSeparater(CDC *pDC, CRect rect)
{
	if(pDC != NULL)
	{
		pDC->Draw3dRect(rect,RGB(0,0,255),RGB(0,0,255));
	}
}
// �޸Ĳ˵�����Ϣ
BOOL CCustomMenu::ChangeMenuItem(CMenu *menu)
{
	int m_index=0;
	if(menu != NULL)
	{
		int m_itemcount = menu->GetMenuItemCount(); // ��ò˵������
		for(int i=0;i<m_itemcount;i++)
		{
			// ��ò˵����ı�
			menu->GetMenuString(i,m_ItemLists[m_index].m_ItemText,MF_BYPOSITION);
			int m_itemID = menu->GetMenuItemID(i); // ��ò˵���ID
			m_ItemLists[m_index].m_ItemID = m_itemID;
			if(m_itemID>0)
			{
				m_ItemLists[m_index].m_ItemIndex= m_ImageIndex;
				m_ImageIndex++;
			}
			// �޸Ĳ˵�����Ϣ
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
// ���ò˵����С
void CCustomMenu::MeasureItem(LPMEASUREITEMSTRUCT lpStruct)
{
	if(lpStruct->CtlType==ODT_MENU)
	{
		lpStruct->itemHeight = 22; // ���ò˵���ĸ�
		lpStruct->itemWidth = 70; // ���ò˵���Ŀ�
		CMenuItemInfo* m_iteminfo;
		m_iteminfo = (CMenuItemInfo*)lpStruct->itemData;
		if(m_iteminfo->m_ItemID == 0)
		{
			lpStruct->itemHeight = 1; // ���÷ָ���
		}
	}
}
// �ػ�˵���
void CCustomMenu::DrawItem(LPDRAWITEMSTRUCT lpStruct)
{
	if(lpStruct->CtlType==ODT_MENU)
	{
		if(lpStruct->itemData == NULL)	return;
		unsigned int m_state = lpStruct->itemState; // ��ò˵���״̬
		CDC* pDC = CDC::FromHandle(lpStruct->hDC);
		CString str = ((CMenuItemInfo*)(lpStruct->itemData))->m_ItemText;
		LPSTR m_str = str.GetBuffer(str.GetLength());
		int m_itemID = ((CMenuItemInfo*)(lpStruct->itemData))->m_ItemID;	
		int m_itemicon = ((CMenuItemInfo*)(lpStruct->itemData))->m_ItemIndex;
		CRect m_rect = lpStruct->rcItem;	
		pDC->SetBkMode(TRANSPARENT); // ���ò˵����ı�����͸��	
		switch(m_itemID)
		{
		case 0: // ���Ʒָ���
				DrawSeparater(pDC,m_rect);
				break;
		default: // ���Ʋ˵����������
				DrawComMenu(pDC,m_rect,m_state&ODS_SELECTED);
				DrawItemText(pDC,m_str,m_rect);			
				break;
		}			
	}
}

