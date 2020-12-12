
// MemoryCheatDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include "MemFinder.h"


// CMemoryCheatDlg 对话框
class CMemoryCheatDlg : public CDialogEx
{
    // 构造
public:
    CMemoryCheatDlg(CWnd *pParent = NULL);  // 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_MEMORYCHEAT_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV 支持


// 实现
protected:
    HICON m_hIcon;

    // 生成的消息映射函数
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
public:
    /*******************************************************************************/
    // 临时地址列表框
    enum {
        EListTempIndexAddress = 0,  // 地址
    };
    typedef std::tuple<DWORD> TupleTempLineData;
    // 增加数据
    void AddTempListData(const TupleTempLineData &info);
    // 获得某行数据
    TupleTempLineData CMemoryCheatDlg::GetTempListData(int index);

    /*******************************************************************************/
    // 目标地止列表框列
    enum {
        EListTargetIndexDescription = 0, // 描述
        EListTargetIndexAddress, // 内存地址
        EListTargetIndexType,  // 数据类型
        EListTargetIndexValue, // 数据值 : 根据上面的 数据类型变化
    };
    // 目标地址列表框 每行数据
    typedef std::tuple<CString, DWORD, CString, CString> TupleTargetLineData;
    // 增加数据
    void AddTargetListData(const TupleTargetLineData &info);
    // 更新数据
    void UpdTargetListData(int index, const TupleTargetLineData &info);
    // 获得某行数据
    TupleTargetLineData GetListTargetData(int index);

    /*******************************************************************************/
    // 获得 dword 类型的值
    DWORD GetListItemValueDWORD(CListCtrl &lst, int nItem, int nSubItem, int _Radix = 0x10);
    // 设置值
    void SetListItemValueFormat(CListCtrl &lst, int nItem, int nSubItem, PCTSTR szFormat, ...);

    virtual BOOL PreTranslateMessage(MSG *pMsg);
    afx_msg void OnBnClickedButtonProgress();
    afx_msg void OnBnClickedButtonFirst();
    afx_msg void OnBnClickedButtonNext();
    afx_msg void OnBnClickedButtonStop();
    // 临时地址列表
    CListCtrl m_lstAddressTemp;

    // 目标地址列表
    CListCtrl m_lstAddressTarget;

    // 要搜索的值
    CString m_strSearchValue;

    // 要搜索的值的类型
    CComboBox m_cbbValueType;
    CString m_strValueType;

    // 搜索工具类指针
    std::shared_ptr<CMemFinder>  m_pFinder{ std::make_shared<CMemFinder>() };

public:
    // 当前正在 操作的进程ID
    DWORD m_dwProcessId{ 0 };

    // 搜索内存的起始地址,结束地址
    CString m_strLimitBegin;
    CString m_strLimitEnd;
    void GetLimit(DWORD &dwLimitBegin, DWORD &dwLimitEnd);

    // 搜索进度列表
    CProgressCtrl m_pProcess;

    // 是否继续搜索
    bool m_bGoon{ false };

    // 数据编辑控件关联变量
    // 描述
    CString m_strDesEdit;
    // 地址
    CString m_strAddressEdit;
    // 值类型
    CString m_strValueTypeEdit;
    CComboBox m_cbbValueTypeEdit;
    // 值
    CString m_strValueEdit;

    /******************** 辅助 ********************/
    struct {
        int index;
        CString strValueType;
    } m_indexValueTypeArray[5];

    // 根据CComboBox不同选择项, 进行不同的 读取/写入内容
    CString ReadValue(int index, DWORD dwAddr);
    CString ReadValue(const CString &strValueType, DWORD dwAddr);
    CString ReadValue(CComboBox &cbb, DWORD dwAddr);
    void WriteValue(CComboBox &cbb, DWORD dwAddr, const CString &value);
    CString ReadValue(CComboBox &cbb, const CString &strAddr);
    void WriteValue(CComboBox &cbb, const CString &strAddr, const CString &value);

    // 更新列表框中的值
    void UpdateTargetListValueByAddress();

    // 增加按钮 : 增加 编辑区的数据到 目标地址列表框中
    afx_msg void OnBnClickedButtonAdd();
    // 删除按钮 : 删除 目标地址列表框中选中的行
    afx_msg void OnBnClickedButtonDel();
    // 保存按钮 : 更新 编辑区的数据到 目标地址列表框中选中的行
    afx_msg void OnBnClickedButtonSave();
    // 双击临时地址列表: 把选中行的数的加入到 目标地址列表
    afx_msg void OnNMDblclkListAddressTemp(NMHDR *pNMHDR, LRESULT *pResult);
    // 单击 目标地址列表
    afx_msg void OnNMClickListAddressTarget(NMHDR *pNMHDR, LRESULT *pResult);
    // 编辑区 数据类型 发生变化: 重读地址处的值
    afx_msg void OnCbnSelchangeComboValueTypeEdit();
    // 定时器, 定时更新 目标列表框中列出的内存地址处的值
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    // 注入游戏进程当中
    afx_msg void OnBnClickedButtonPlantInject();
};