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
	CString m_ItemText;  // �˵����ı�
	int     m_ItemIndex; // �˵�������
	int     m_ItemID;    // �˵���� 0�ָ���,������ͨ�˵�
};

class CCustomMenu  :  public CMenu
{
public:
	CCustomMenu();
	virtual ~CCustomMenu();

	CMenuItemInfo m_ItemLists[10]; // �˵�����Ϣ
	int			  m_ImageIndex;    // ͼ������
	void DrawItemText(CDC* pDC,LPSTR str,CRect rect);      // ���Ʋ˵����ı�
	void DrawComMenu(CDC* pDC,CRect rect, BOOL select);    // ���Ʋ˵����ͼƬ
	void DrawSeparater(CDC* pDC,CRect rect);               // ���Ʒָ���
	BOOL ChangeMenuItem(CMenu* menu);                      // �޸Ĳ˵�����Ϣ
	virtual void DrawItem(LPDRAWITEMSTRUCT lpStruct);		// �ػ�˵���
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpStruct); // ���ò˵����С
};

#endif // !defined(AFX_CUSTOMMENU_H__5E0963CD_D37A_4BEA_B1E7_F092682907BA__INCLUDED_)
