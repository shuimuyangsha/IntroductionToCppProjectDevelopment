
// MemoryCheatDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include "MemFinder.h"


// CMemoryCheatDlg �Ի���
class CMemoryCheatDlg : public CDialogEx
{
    // ����
public:
    CMemoryCheatDlg(CWnd *pParent = NULL);  // ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_MEMORYCHEAT_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV ֧��


// ʵ��
protected:
    HICON m_hIcon;

    // ���ɵ���Ϣӳ�亯��
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
public:
    /*******************************************************************************/
    // ��ʱ��ַ�б��
    enum {
        EListTempIndexAddress = 0,  // ��ַ
    };
    typedef std::tuple<DWORD> TupleTempLineData;
    // ��������
    void AddTempListData(const TupleTempLineData &info);
    // ���ĳ������
    TupleTempLineData CMemoryCheatDlg::GetTempListData(int index);

    /*******************************************************************************/
    // Ŀ���ֹ�б����
    enum {
        EListTargetIndexDescription = 0, // ����
        EListTargetIndexAddress, // �ڴ��ַ
        EListTargetIndexType,  // ��������
        EListTargetIndexValue, // ����ֵ : ��������� �������ͱ仯
    };
    // Ŀ���ַ�б�� ÿ������
    typedef std::tuple<CString, DWORD, CString, CString> TupleTargetLineData;
    // ��������
    void AddTargetListData(const TupleTargetLineData &info);
    // ��������
    void UpdTargetListData(int index, const TupleTargetLineData &info);
    // ���ĳ������
    TupleTargetLineData GetListTargetData(int index);

    /*******************************************************************************/
    // ��� dword ���͵�ֵ
    DWORD GetListItemValueDWORD(CListCtrl &lst, int nItem, int nSubItem, int _Radix = 0x10);
    // ����ֵ
    void SetListItemValueFormat(CListCtrl &lst, int nItem, int nSubItem, PCTSTR szFormat, ...);

    virtual BOOL PreTranslateMessage(MSG *pMsg);
    afx_msg void OnBnClickedButtonProgress();
    afx_msg void OnBnClickedButtonFirst();
    afx_msg void OnBnClickedButtonNext();
    afx_msg void OnBnClickedButtonStop();
    // ��ʱ��ַ�б�
    CListCtrl m_lstAddressTemp;

    // Ŀ���ַ�б�
    CListCtrl m_lstAddressTarget;

    // Ҫ������ֵ
    CString m_strSearchValue;

    // Ҫ������ֵ������
    CComboBox m_cbbValueType;
    CString m_strValueType;

    // ����������ָ��
    std::shared_ptr<CMemFinder>  m_pFinder{ std::make_shared<CMemFinder>() };

public:
    // ��ǰ���� �����Ľ���ID
    DWORD m_dwProcessId{ 0 };

    // �����ڴ����ʼ��ַ,������ַ
    CString m_strLimitBegin;
    CString m_strLimitEnd;
    void GetLimit(DWORD &dwLimitBegin, DWORD &dwLimitEnd);

    // ���������б�
    CProgressCtrl m_pProcess;

    // �Ƿ��������
    bool m_bGoon{ false };

    // ���ݱ༭�ؼ���������
    // ����
    CString m_strDesEdit;
    // ��ַ
    CString m_strAddressEdit;
    // ֵ����
    CString m_strValueTypeEdit;
    CComboBox m_cbbValueTypeEdit;
    // ֵ
    CString m_strValueEdit;

    /******************** ���� ********************/
    struct {
        int index;
        CString strValueType;
    } m_indexValueTypeArray[5];

    // ����CComboBox��ͬѡ����, ���в�ͬ�� ��ȡ/д������
    CString ReadValue(int index, DWORD dwAddr);
    CString ReadValue(const CString &strValueType, DWORD dwAddr);
    CString ReadValue(CComboBox &cbb, DWORD dwAddr);
    void WriteValue(CComboBox &cbb, DWORD dwAddr, const CString &value);
    CString ReadValue(CComboBox &cbb, const CString &strAddr);
    void WriteValue(CComboBox &cbb, const CString &strAddr, const CString &value);

    // �����б���е�ֵ
    void UpdateTargetListValueByAddress();

    // ���Ӱ�ť : ���� �༭�������ݵ� Ŀ���ַ�б����
    afx_msg void OnBnClickedButtonAdd();
    // ɾ����ť : ɾ�� Ŀ���ַ�б����ѡ�е���
    afx_msg void OnBnClickedButtonDel();
    // ���水ť : ���� �༭�������ݵ� Ŀ���ַ�б����ѡ�е���
    afx_msg void OnBnClickedButtonSave();
    // ˫����ʱ��ַ�б�: ��ѡ���е����ļ��뵽 Ŀ���ַ�б�
    afx_msg void OnNMDblclkListAddressTemp(NMHDR *pNMHDR, LRESULT *pResult);
    // ���� Ŀ���ַ�б�
    afx_msg void OnNMClickListAddressTarget(NMHDR *pNMHDR, LRESULT *pResult);
    // �༭�� �������� �����仯: �ض���ַ����ֵ
    afx_msg void OnCbnSelchangeComboValueTypeEdit();
    // ��ʱ��, ��ʱ���� Ŀ���б�����г����ڴ��ַ����ֵ
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    // ע����Ϸ���̵���
    afx_msg void OnBnClickedButtonPlantInject();
};