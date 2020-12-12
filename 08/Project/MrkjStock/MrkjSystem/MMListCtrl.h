#pragma once


// CMMListCtrl

class CMMListCtrl : public CMFCListCtrl
{
    DECLARE_DYNAMIC(CMMListCtrl)

public:
    CMMListCtrl();
    virtual ~CMMListCtrl();

protected:
    DECLARE_MESSAGE_MAP()
public:
    int InsertItem(int nItem, LPCTSTR szText, COLORREF txtClr, COLORREF txtBk = RGB(0x00, 0x00, 0x00));
    BOOL SetItemText(int nItem, int iSubItem, LPCTSTR szText, COLORREF txtClr, COLORREF txtBk = RGB(0x00, 0x00, 0x00));
    virtual COLORREF OnGetCellBkColor(int nRow, int nColumn);
    virtual COLORREF OnGetCellTextColor(int nRow, int nColumn);
    virtual HFONT OnGetCellFont(int nRow, int nColum, DWORD dwData = 0);
    typedef struct {
        COLORREF clrTxt;
        COLORREF clrBk;
        CString text;
    } SItemContent; //一项数据

    typedef std::map<int, SItemContent> MItem; // 一行数据
    typedef std::map<int, MItem> MLine;  // 全部数据

    MLine m_datas;
    afx_msg BOOL OnEraseBkgnd(CDC *pDC);

private:
    int m_iPage;// 当前是第几页
    int m_iCount; // 每页的行数
public:
    int GetPage() const
    {
        return m_iPage;
    }
    void SetPage(int iPage)
    {
        m_iPage = iPage;
    }

    void SetOnePageCount(int count)
    {
        m_iCount = count;
    }
    int GetOnePageCount()
    {
        return m_iCount;
    }
};


