#pragma once
#include "afxcmn.h"

#include "MMListCtrl.h"

// CDialogHuShen 对话框

class CDialogHuShen : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogHuShen)

public:
    CDialogHuShen(CWnd *pParent = NULL);   // 标准构造函数
    virtual ~CDialogHuShen();

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_HUSHEN };
#endif

protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnSize(UINT nType, int cx, int cy);

    CMMListCtrl m_lst;
    enum {
        EListIndexNo = 0, // 序列
        EListIndexCode, // 代码
        EListIndexName, // 名称
        EListIndexZhangFu, // 涨幅
        EListIndexXianJian, // 现价
        EListIndexZhangDie,  // 涨跌
        EListIndexBuy, // 买价
        EListIndexSell, // 卖价
        EListIndexZongLiang,   // 总量
        EListIndexOpen, // 今开
        EListIndexHigh, // 最高
        EListIndexLow, // 最低
        EListIndexZuoShou, // 昨收
        EListIndexZongJinE, // 总金额
        EListIndexMaxLimit, // 列数
    };

private:
    enum {MaxCountOnePage = 28}; //一页显示的行数

    // 载入数据
    void LoadData(int iPage);
public:
    virtual BOOL PreTranslateMessage(MSG *pMsg);
    // 启动更新实时数据的线程
    void StartUpdate();
    // 停止更新实时数据的线程
    void StopUpdate();
    // 线程句柄
    HANDLE m_hThread{NULL};
    // 线程ID
    unsigned m_threadID{0};
    // 线程函数
    static unsigned __stdcall ThreadRoutine(PVOID pArgs);
    afx_msg void OnBnClickedButtonFilter();
    afx_msg void OnBnClickedButtonAll();

    // 显示的类别
    enum EShowStaus {EAll, // 全部
                     EFilter, // 条件选股
                    };
    // 当前显示的类别
    EShowStaus m_status{EAll};
    afx_msg void OnBnClickedButtonShowFilter();

    // 条件选股选择出来的股票
    std::vector<std::wstring> m_vFilterStocks;
};
