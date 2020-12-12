// DialogProgress.cpp : 实现文件
//

#include "stdafx.h"
#include "MemoryCheat.h"
#include "DialogProgress.h"
#include "afxdialogex.h"


#include <TlHelp32.h>

namespace hh
{
	// 判断os是否是64位的
	BOOL IsOs64 ()
	{
		SYSTEM_INFO si;
		GetNativeSystemInfo (&si);
		if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64
			|| si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64) {
			return TRUE;
		}
		else {
			return FALSE;
		}

	}

	// 判断是否是32位进程
	BOOL Is32BitProcess (DWORD dwProcessId)
	{
		// 如果不是64位统，则肯定是32位进程
		if (!hh::IsOs64 ()) {
			return TRUE;
		}
		if (dwProcessId == 0 || dwProcessId == 4) {
			return FALSE;
		}
		// 判断是否是32位系统
		typedef BOOL (WINAPI * LPFN_ISWOW64PROCESS)(HANDLE, PBOOL);
		static LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress (GetModuleHandle (_T ("kernel32")), "IsWow64Process");
		// 是32位系统, 进程止定是 32位的
		if (!fnIsWow64Process) {
			return TRUE;
		}
		// 判断是否是 64位系统下面的 32位进程
		HANDLE h = OpenProcess (PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessId);
		if (!h || h == INVALID_HANDLE_VALUE) {
			return FALSE;
		}
		BOOL b32 = 0;
		BOOL b = fnIsWow64Process (h, &b32);
		CloseHandle (h);
		return b32;
	}

	// 获得进程完整的路径
	BOOL GetFullPathProcess (DWORD dwProcessId, TCHAR szBuffer[], DWORD dwMaxLen)
	{
		HANDLE h = OpenProcess (PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessId);
		if (h && h != INVALID_HANDLE_VALUE) {
			DWORD dwLen = dwMaxLen;
			BOOL b = ::QueryFullProcessImageName (h, 0, szBuffer, (PDWORD)&dwLen);
			if (b) {
				szBuffer[dwLen] = _T ('\0');
			}
			CloseHandle (h);
			return b;
		}
		return FALSE;
	}
	BOOL GetProcessIco (DWORD dwProcessId, HICON &hIco)
	{
		TCHAR szPath[MAX_PATH] = { 0 };
		if (hh::GetFullPathProcess (dwProcessId, szPath, sizeof (szPath))) {
			SHFILEINFO info = { 0 };
			DWORD_PTR dwRet = ::SHGetFileInfo (szPath, 0, &info, sizeof (info), SHGFI_ICON | SHGFI_LARGEICON);
			if (dwRet == 0) {
				hIco = 0;
			}
			else {
				hIco = info.hIcon;
			}
		}
		return hIco != 0;
	}


}




DWORD CDialogProgress::m_dwProcessId = 0;
CString CDialogProgress::m_strProcessName = _T ("");

// CDialogProgress 对话框

IMPLEMENT_DYNAMIC (CDialogProgress, CDialogEx)


CDialogProgress::CDialogProgress (CWnd *pParent /*=NULL*/)
	: CDialogEx (IDD_DIALOG_PROGRESS_LIST, pParent)
{
}

CDialogProgress::~CDialogProgress ()
{
}

void CDialogProgress::DoDataExchange (CDataExchange *pDX)
{
	CDialogEx::DoDataExchange (pDX);
	DDX_Control (pDX, IDC_LIST1, m_lst);
}


BEGIN_MESSAGE_MAP (CDialogProgress, CDialogEx)
	ON_NOTIFY (NM_DBLCLK, IDC_LIST1, &CDialogProgress::OnNMDblclkList1)
END_MESSAGE_MAP ()


// CDialogProgress 消息处理程序




// 获取32位进程,并更新到进程列中
BOOL CDialogProgress::GetProcessList ()
{
	// 通过进程快照,遍历进程
	// 并判断进程是否是32位进程
	// 如果是32位进程,则加入进程列表m_lst中
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;
	hProcessSnap = CreateToolhelp32Snapshot (TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		// 创建快照失败,返回
		return(FALSE);
	}
	// 获得第一个进程
	pe32.dwSize = sizeof (PROCESSENTRY32);
	if (!Process32First (hProcessSnap, &pe32)) {
		CloseHandle (hProcessSnap);
		return(FALSE);
	}
	do {
		// 判数是否是32位进程
		if (hh::Is32BitProcess (pe32.th32ProcessID)) {
			// 如果获取图标成功, 表示图标在m_imgList中的位置
			int indexIco = -1;
			// 获取进程图标,并加入到 m_imgList中
			{
				HICON hIco = 0;
				if (hh::GetProcessIco (pe32.th32ProcessID, hIco)) {
					// 如果有图标,加入列表
					indexIco = m_imgList.Add (hIco);
				}
			}

			// 插入新行
			{
				// 进程名
				int index = m_lst.InsertItem (m_lst.GetItemCount (),
					pe32.szExeFile, indexIco);
				//进程ID
				CString s;
				s.Format (_T ("%d"), pe32.th32ProcessID);
				m_lst.SetItemText (index, 1, s);
				// 设置该行数据
				m_lst.SetItemData (index, (DWORD_PTR)pe32.th32ProcessID);
			}
		}
		else {
			// 不是32位进程,什么也不做
		}
	}
	// 获得下一个进程
	while (Process32Next (hProcessSnap, &pe32));

	// 关闭 快照句柄
	CloseHandle (hProcessSnap);
	return(TRUE);
}

BOOL CDialogProgress::OnInitDialog ()
{
	CDialogEx::OnInitDialog ();
	// 控件
	{
		LONG lStyle = GetWindowLong (m_lst.m_hWnd, GWL_STYLE);
		lStyle &= ~LVS_TYPEMASK;
		lStyle |= LVS_REPORT;
		SetWindowLong (m_lst.GetSafeHwnd (), GWL_STYLE, lStyle);
		DWORD dwStyle = m_lst.GetExtendedStyle ();
		dwStyle |= LVS_EX_FULLROWSELECT; //选中行 整征高亮
		dwStyle |= LVS_EX_GRIDLINES; //网络线
		m_lst.SetExtendedStyle (dwStyle);
		// 设置列,并设置大小
		{
			CRect rc;
			m_lst.GetClientRect (rc);
			m_lst.InsertColumn (0, _T ("进程名"), LVCFMT_LEFT, rc.Width () / 2);
			m_lst.InsertColumn (1, _T ("进程ID"), LVCFMT_LEFT, rc.Width () / 2);
		}
		// 设置控件关联的图标列表,这样才可以在每行的开头显示图标
		m_imgList.Create (16, 16, ILC_COLOR32, 1, 1);
		m_lst.SetImageList (&m_imgList, LVSIL_SMALL);
	}
	//列出进程
	GetProcessList ();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDialogProgress::OnNMDblclkList1 (NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = pNMItemActivate->iItem;
	// 如果有选中的行,设置回传给父窗口的变量,使父窗口可以知道当前选择的进程信息
	if (nItem >= 0) {
		m_dwProcessId = (DWORD)m_lst.GetItemData (nItem);
		m_strProcessName = m_lst.GetItemText (nItem, 0);
		OnOK ();
	}
	*pResult = 0;
}
