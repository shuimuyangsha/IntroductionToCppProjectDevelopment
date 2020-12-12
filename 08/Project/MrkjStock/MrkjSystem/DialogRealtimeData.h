#pragma once
#include "afxwin.h"

#include "HttpDataEntry.h"

// CDialogRealtimeData �Ի���

class CDialogRealtimeData : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogRealtimeData)

public:
    CDialogRealtimeData(CWnd* pParent = NULL);   // ��׼���캯��
    virtual ~CDialogRealtimeData();

// �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_REALTIME_DATA };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    DECLARE_MESSAGE_MAP()

private:
    // ����ѯ�̲߳�ѯ������.�ᴥ������Ϣ
    afx_msg LRESULT OnMsgHttpQueryEnd(WPARAM wParam, LPARAM lParam);
    // ������ �������ͷ�����
    typedef CMRKJDatabase::TupleStockInfo INFO, *PINFO;
    // ��� Ŀ�� ��Ʊ����
    bool GetSelStockCode(CString& strCode);
	// ����CComboBox��������
	void ClearList();
    // �洢 ʵʱ����
    data_http::CHttpDataEntry m_httpData{};
    // ���ص�K��ͼ·��
    std::wstring m_strFilePath;
    // ��Ʊ����
    CComboBox m_cbbStockCode;
    CString m_strCode;
public:
    enum WM_REAL_TIME { sc_MSG_MIN = (WM_USER + 101),
                        sc_MSG_HTTP_QUERY_END, //���ݲ�ѯ���
                      };
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual void OnCancel();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnPaint();
    afx_msg void OnBnClickedButtonQuery();
	afx_msg void OnDestroy();

    // �����б������(��Ʊ��Ϣ)
    void UpdateStockInfoComboBox();
};
