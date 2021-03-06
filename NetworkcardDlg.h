
// MFCListMacDlg.h : 头文件
//

#pragma once


// CMFCListMacDlg 对话框
class CMFCListMacDlg : public CDialogEx
{
// 构造
public:
	CMFCListMacDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFCLISTMAC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


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
	afx_msg void OnEnChangeBlank_char();
	afx_msg void OnBnClickedButtonCurrentPath();

	void SendDlgListboxMessage(int listboxID, char* szString);
	void SeparateMacWithRule(char* szString, char* szMacString);
	void WriteMacToFile(char* szMacString, char* szfileName);
#if 0
	static DWORD WINAPI FindFileThread(LPVOID lpParameter);
#endif

public:
	char szPath[255];
	char szSaveFileName[255];
	char szSuffix[255];
	char szBlankChar[255];
	unsigned char u8checkBoxStatus;
	afx_msg void OnBnClickedButtonOtherPath();
	afx_msg void OnEnChangeEditSuffix();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnEnChangeEditSaveFilename();
	afx_msg void OnBnClickedOk();
};
