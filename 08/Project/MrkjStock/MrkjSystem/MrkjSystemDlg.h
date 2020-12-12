
// MrkjSystemDlg.h : 头文件
//

#pragma once


#include "DialogAbout.h"
#include "DialogCalc.h"
#include "DialogDataImpExp.h"
#include "DialogDataKind.h"
#include "DialogDataMaintenance.h"
#include "DialogHostoryData.h"
#include "DialogKindSelect.h"
#include "DialogRealtimeData.h"
#include "DialogSetting.h"
#include "DialogHuShen.h"


// CMrkjSystemDlg 对话框
class CMrkjSystemDlg : public CDialogEx
{
    // 构造
public:
    CMrkjSystemDlg(CWnd *pParent = NULL);   // 标准构造函数
    ~CMrkjSystemDlg();
// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_MRKJSYSTEM_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV 支持


// 实现
protected:
    HICON m_hIcon;
    // 菜单栏
    CMenu m_menu;
    //状态栏
    CStatusBar m_statusBar;

    // 背景画刷
    CBrush m_bkBrush;

    // 生成的消息映射函数
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL PreTranslateMessage(MSG *pMsg);
    virtual void OnOK();
    virtual void OnCancel();

    // 画背景 : 更改背景画刷
    afx_msg HBRUSH OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor);

    //数据维护
    afx_msg void OnMenuDataMaintenance();

    // 数据导入导出
    afx_msg void OnMenuDataImpexp();

    // 种类维护
    afx_msg void OnMenuDataKind();
    // 打印
    afx_msg void OnMenuPrint();

    // 退出系统
    afx_msg void OnMenuExitSys();

    // 种类选择
    afx_msg void OnMenuKindSelect();

    // 历史数据
    afx_msg void OnMenuHostoryData();

    // 实时数据
    afx_msg void OnMenuRealtimeData();

    // 金融记算器
    afx_msg void OnMenuCalc();

    // 屏幕截图
    afx_msg void OnMenuScreenShot();

    // 系统设置
    afx_msg void OnMenuSetting();

    // 帮助
    afx_msg void OnMenuHelp();

    // 关于本系统
    afx_msg void OnMenuAbout();

private:

    // 主页 沪深分类股票
    CDialogHuShen *m_pDlgHuShen{ new CDialogHuShen()};

    // 子对话框 : 数据维护
    CDialogDataMaintenance *m_pDlgDataMaintenance{new CDialogDataMaintenance()};

    // 子对话框 : 数据导入导出
    CDialogDataImpExp *m_pDlgDataImpExp{new CDialogDataImpExp()};

    // 子对话框 : 类型维护
    CDialogDataKind *m_pDlgDataKind{new CDialogDataKind()};

    // 子对话框 : 类型选择
    CDialogKindSelect *m_pDlgKindSelect{new CDialogKindSelect()};

    // 子对话框 : 历史数据
    CDialogHostoryData *m_pDlgHostoryData{new CDialogHostoryData()};

    // 子对话框 : 实时数据
    CDialogRealtimeData *m_pDlgRealtimeData{new CDialogRealtimeData()};

    // 子对话框 : 计算器
    CDialogCalc *m_pDlgCalc{new CDialogCalc()};

    // 子对话框 : 屏幕截图
    // 没有对话框

    // 子对话框 : 系统设置
    CDialogSetting *m_pDlgSetting{new CDialogSetting()};

    // 子对话框 : 帮助
    // 没有对话框, 弹出网页

    // 子对话框 : 关于
    // 模态对话框, 不必定义成成员变量

    // 存讲所有对话框的指针
    std::vector<CDialogEx *> m_vDlgPtr;

    //只显示自己的窗口,其余的隐藏,并返回上次显示的窗口
    void ShowChange(CDialogEx *pDlg);

    //记录当前正在显示的对话框
    CDialogEx *m_pDlgLastShow{nullptr};

public:
    afx_msg void OnMenuRealHushen();
};
