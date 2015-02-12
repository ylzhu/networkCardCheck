
// MFCListMacDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Networkcard.h"
#include "NetworkcardDlg.h"
#include "afxdialogex.h"
#include "windows.h"
#include "ping.h"

//#param warning(disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


char *w2c(char *pcstr, const WCHAR *pwstr, size_t len)
{
	int nlength = wcslen(pwstr);

	//获取转换后的长度
	int nbytes = WideCharToMultiByte(0,	// specify the code page used to perform the conversion
		0,									// no special flags to handle unmapped characters
		pwstr,     // wide character string to convert
		nlength,   // the number of wide characters in that string
		NULL,      // no output buffer given, we just want to know how long it needs to be
		0,
		NULL,      // no replacement character given
		NULL);    // we don't want to know if a character didn't make it through the translation 

	// make sure the buffer is big enough for this, making it larger if necessary
	if (nbytes>(int)len)   nbytes = len;

	// 通过以上得到的结果，转换unicode 字符为ascii 字符
	WideCharToMultiByte(0, // specify the code page used to perform the conversion
		0,         // no special flags to handle unmapped characters
		pwstr,   // wide character string to convert
		nlength,   // the number of wide characters in that string
		pcstr, // put the output ascii characters at the end of the buffer
		nbytes,                           // there is at least this much space there
		NULL,      // no replacement character given
		NULL);
	pcstr[nbytes] = '\0';
	return pcstr;

}

//将char* 转成wchar_t*的实现函数如下： 

//这是把asii字符转换为unicode字符，和上面相同的原理
void c2w(wchar_t *pwstr, size_t len, const char *str)
{
	if (str)
	{
		size_t nu = strlen(str);
		size_t n = (size_t)MultiByteToWideChar(CP_ACP, 0, (const char *)str, (int)nu, NULL, 0);
		if (n >= len)n = len - 1;

		MultiByteToWideChar(CP_ACP, 0, (const char *)str, (int)nu, pwstr, (int)n);
		pwstr[n] = 0;
	}
}


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCListMacDlg 对话框



CMFCListMacDlg::CMFCListMacDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCListMacDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCListMacDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCListMacDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT_BLANK_CHAR, &CMFCListMacDlg::OnEnChangeBlank_char)
	ON_BN_CLICKED(IDC_BUTTON_CURRENT_PATH, &CMFCListMacDlg::OnBnClickedButtonCurrentPath)
	ON_BN_CLICKED(IDC_BUTTON_OTHER_PATH, &CMFCListMacDlg::OnBnClickedButtonOtherPath)
	ON_EN_CHANGE(IDC_EDIT_SUFFIX, &CMFCListMacDlg::OnEnChangeEditSuffix)
	ON_BN_CLICKED(IDC_CHECK1, &CMFCListMacDlg::OnBnClickedCheck1)
	ON_EN_CHANGE(IDC_EDIT_SAVE_FILENAME, &CMFCListMacDlg::OnEnChangeEditSaveFilename)
	ON_BN_CLICKED(IDOK, &CMFCListMacDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CMFCListMacDlg 消息处理程序

BOOL CMFCListMacDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	
	char m_strRemoteMachine[100] = {0}; // .Format("%s","192.168.3.10");  
	CPing myPing;  
	strcpy(m_strRemoteMachine, "192.168.3.28");
	bool blPing=myPing.Ping(5,m_strRemoteMachine,this->m_hWnd);  
	ZeroMemory(szSaveFileName, sizeof(szSaveFileName));
	ZeroMemory(szSuffix, sizeof(szSuffix));
	ZeroMemory(szBlankChar, sizeof(szBlankChar));
	ZeroMemory(szPath, sizeof(szPath));
	
	//strcpy(szSaveFileName, "MacAddressList");
	strcpy(szSaveFileName, "MacAddressList");
	strcpy(szSuffix, ("cer"));
	strcpy(szBlankChar, ("-"));
	SetDlgItemText(IDC_EDIT_SUFFIX, szSuffix);
	SetDlgItemText(IDC_EDIT_BLANK_CHAR, szBlankChar);
	SetDlgItemText(IDC_EDIT_SAVE_FILENAME, szSaveFileName);

	SendDlgListboxMessage(IDC_LISTBOX_MSG, ("默认后缀名:"));
	SendDlgListboxMessage(IDC_LISTBOX_MSG, szSuffix);
	SendDlgListboxMessage(IDC_LISTBOX_MSG, ("默认过滤间隔符:"));
	SendDlgListboxMessage(IDC_LISTBOX_MSG, szBlankChar);
	SendDlgListboxMessage(IDC_LISTBOX_MSG, ("默认保存文件名:"));
	SendDlgListboxMessage(IDC_LISTBOX_MSG, szSaveFileName);

	((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(1);
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK1))
	{
		// 勾选
		u8checkBoxStatus = 1;
		SendDlgListboxMessage(IDC_LISTBOX_MSG, ("列表信息去除文件后缀."));
	}
	else
	{
		u8checkBoxStatus = 0;
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCListMacDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCListMacDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCListMacDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCListMacDlg::OnEnChangeBlank_char()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT_BLANK_CHAR, szBlankChar, 20);
	//GetDlgItemText(IDC_EDIT_BLANK_CHAR, szBlankChar);
	//GetDlgItemText(IDC_EDIT_SAVE_FILENAME, szSaveFileName);
	//SendDlgListboxMessage(IDC_LISTBOX_MSG, ("去除:"));
	//SendDlgListboxMessage(IDC_LISTBOX_MSG, szBlankChar);

}


void CMFCListMacDlg::OnBnClickedButtonCurrentPath()
{
	// TODO:  在此添加控件通知处理程序代码
	GetCurrentDirectory(sizeof(szPath), szPath);
	//MessageBox(szPath);
	//SetWindowTextW(GetDlgItem(hDlg, IDC_BUTTON_CURRENT_PATH), _T(""));
	SetDlgItemText(IDC_STATIC_PATH, szPath);

	//SetDlgItemText(IDC_LISTBOX_MSG, ("Current path:"));
	//SetDlgItemText(IDC_LISTBOX_MSG, szPath);

	SendDlgListboxMessage(IDC_LISTBOX_MSG, ("Current path:"));
	SendDlgListboxMessage(IDC_LISTBOX_MSG, szPath);
}

void CMFCListMacDlg::SendDlgListboxMessage(int listboxID, char* szString)
{
#if 1
	long lNumber;
	//WCHAR strWtmp[256] = {0};
	//c2w(strWtmp, strlen(szString), szString);
	SendDlgItemMessage(listboxID, LB_ADDSTRING, NULL, reinterpret_cast<LPARAM>(szString));	//打印字串到listbox
	lNumber = SendDlgItemMessage(listboxID, LB_GETCOUNT, NULL, NULL);						//获取listbox中字串数目
	SendDlgItemMessage(listboxID, LB_SETCURSEL, lNumber - 1, 0);							//使本字串反选, 同时可以使字串在listbox的可见区域.
	SendDlgItemMessage(listboxID, LB_SETCURSEL, -1, 0);		
	//UpdateData(true);
#endif//取消所有反选.
	
}


void CMFCListMacDlg::OnBnClickedButtonOtherPath()
{
	// TODO:  在此添加控件通知处理程序代码
	char szPath_tmp[MAX_PATH];     //存放选择的目录路径 
	//CString str;

	ZeroMemory(szPath_tmp, sizeof(szPath_tmp));

	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPath_tmp;
	bi.lpszTitle = ("请选择需要打包的目录：");
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	//弹出选择目录对话框
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);

	if (lp && SHGetPathFromIDList(lp, szPath_tmp))
	{
		//str.Format(("选择的目录为 %s"), szPath);
		//MessageBox(str);
		strcpy(szPath, szPath_tmp);
		SetDlgItemText(IDC_STATIC_PATH, szPath);
		SendDlgListboxMessage(IDC_LISTBOX_MSG, ("Select path:"));
		SendDlgListboxMessage(IDC_LISTBOX_MSG, szPath);


	}
	else
	{
		MessageBox(("无效的目录，请重新选择"));
		SendDlgListboxMessage(IDC_LISTBOX_MSG, ("无效的目录，请重新选择"));
	}
		
}


void CMFCListMacDlg::OnEnChangeEditSuffix()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT_SUFFIX, szSuffix, 20);
	//GetDlgItemText(IDC_EDIT_BLANK_CHAR, szBlankChar);
	//GetDlgItemText(IDC_EDIT_SAVE_FILENAME, szSaveFileName);
	//SendDlgListboxMessage(IDC_LISTBOX_MSG, ("后缀名改为:"));
	//SendDlgListboxMessage(IDC_LISTBOX_MSG, szSuffix);
	
}


void CMFCListMacDlg::OnBnClickedCheck1()
{
	// TODO:  在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK1))
	{
		// 勾选
		u8checkBoxStatus = 1;
		//SendDlgListboxMessage(IDC_LISTBOX_MSG, ("列表信息去除文件后缀."));
	}
	else
	{
		u8checkBoxStatus = 0;
		//SendDlgListboxMessage(IDC_LISTBOX_MSG, ("列表信息保留文件后缀."));
	}
}


void CMFCListMacDlg::OnEnChangeEditSaveFilename()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT_SAVE_FILENAME, szSaveFileName, 50);
	
	//SendDlgListboxMessage(IDC_LISTBOX_MSG, ("文件名:"));
	//SendDlgListboxMessage(IDC_LISTBOX_MSG, szSaveFileName);
}

#if 0
 DWORD WINAPI CMFCListMacDlg::FindFileThread(LPVOID lpParameter)
{
	char szPath_tmp[500];
	char szMacstr[50];
	char szWildcard[20];
	char szFileName[MAX_PATH];
	char szFilename_c[MAX_PATH];

	char szRecordNum[60];
	char szRecordNum_c[60];

	WIN32_FIND_DATA p;
	HANDLE h = NULL;
	int record = 0;
	int len = 0;
	ZeroMemory(szPath_tmp, sizeof(szPath_tmp));
	ZeroMemory(szWildcard, sizeof(szWildcard));
	ZeroMemory(szMacstr, sizeof(szMacstr));
	ZeroMemory(szFileName, sizeof(szFileName));

	SYSTEMTIME sys;

	((CButton*)GetDlgItem(IDOK))->EnableWindow(FALSE);

	GetLocalTime(&sys);
	_tcscpy(szFileName, szSaveFileName);
	w2c(szFilename_c, szFileName, sizeof(szFileName));
	sprintf(szFilename_c, "%s%04d", szFilename_c, sys.wMilliseconds);
	c2w(szFileName, sizeof(szFilename_c), szFilename_c);
	SendDlgListboxMessage(IDC_LISTBOX_MSG, szFileName);

	len = wcslen(szPath);
	if (szPath[len - 1] == '\\')
	{
		_tcscpy(szWildcard, ("*."));
	}
	else
	{
		_tcscpy(szWildcard, ("\\*."));
	}
	_tcscpy(szPath_tmp, szPath);
	_tcscat(szPath_tmp, szWildcard);
	_tcscat(szPath_tmp, szSuffix);
	SendDlgListboxMessage(IDC_LISTBOX_MSG, szPath_tmp);
#if 1
	h = FindFirstFile(szPath_tmp, &p);
	if (h != INVALID_HANDLE_VALUE)
	{
		//SendDlgListboxMessage(IDC_LISTBOX_MSG, p.cFileName);
		SeparateMacWithRule(p.cFileName, szMacstr);
		SendDlgListboxMessage(IDC_LISTBOX_MSG, szMacstr);
		WriteMacToFile(szMacstr, szFileName);
		record++;

	}
	else
	{
		((CButton*)GetDlgItem(IDOK))->EnableWindow(TRUE);
		SendDlgListboxMessage(IDC_LISTBOX_MSG, ("No file found!"));
		return 0;
	}
	while (FindNextFile(h, &p))
	{
		//SendDlgListboxMessage(IDC_LISTBOX_MSG, p.cFileName);
		SeparateMacWithRule(p.cFileName, szMacstr);
		SendDlgListboxMessage(IDC_LISTBOX_MSG, szMacstr);
		WriteMacToFile(szMacstr, szFileName);
		record++;
	}
	FindClose(h);
	// print record number.
	_tcscpy(szRecordNum, ("record number: "));
	w2c(szRecordNum_c, szRecordNum, sizeof(szRecordNum));
	sprintf(szRecordNum_c, "%s%04d", szRecordNum_c, record);
	c2w(szRecordNum, sizeof(szRecordNum_c), szRecordNum_c);
	SendDlgListboxMessage(IDC_LISTBOX_MSG, szRecordNum);

	// print save file name.
	SendDlgListboxMessage(IDC_LISTBOX_MSG, ("Mac list save to file:"));
	SendDlgListboxMessage(IDC_LISTBOX_MSG, szFileName);
	((CButton*)GetDlgItem(IDOK))->EnableWindow(TRUE);
	return 1;
#endif
}
#endif


void CMFCListMacDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
#if 1
	char szPath_tmp[500];
	char szMacstr[50];
	char szWildcard[20];
	char szFileName[MAX_PATH];
	char szFilename_c[MAX_PATH];

	char szRecordNum[60];
	char szRecordNum_c[60];

	WIN32_FIND_DATA p;
	HANDLE h = NULL;
	int record = 0;
	int len = 0;
	ZeroMemory(szPath_tmp, sizeof(szPath_tmp));
	ZeroMemory(szWildcard, sizeof(szWildcard));
	ZeroMemory(szMacstr, sizeof(szMacstr));
	ZeroMemory(szFileName, sizeof(szFileName));

	SYSTEMTIME sys;
	
	((CButton*)GetDlgItem(IDOK))->EnableWindow(FALSE);

	GetLocalTime(&sys);
	strcpy(szFileName, szSaveFileName);
	strcpy(szFilename_c, szFileName);
	//w2c(szFilename_c, szFileName, sizeof(szFileName));
	sprintf(szFilename_c, "%s%04d", szFilename_c, sys.wMilliseconds);
	//c2w(szFileName, sizeof(szFilename_c), szFilename_c);
	SendDlgListboxMessage(IDC_LISTBOX_MSG, szFilename_c);

	len = strlen(szPath);
	if (szPath[len - 1] == '\\')
	{
		_tcscpy(szWildcard, ("*."));
	}
	else
	{
		_tcscpy(szWildcard, ("\\*."));
	}
	_tcscpy(szPath_tmp, szPath);
	_tcscat(szPath_tmp, szWildcard);
	_tcscat(szPath_tmp, szSuffix);
	SendDlgListboxMessage(IDC_LISTBOX_MSG, szPath_tmp);
#if 1
	h = FindFirstFile(szPath_tmp, &p);
	if (h != INVALID_HANDLE_VALUE)
	{
		//SendDlgListboxMessage(IDC_LISTBOX_MSG, p.cFileName);
		SeparateMacWithRule(p.cFileName, szMacstr);
		SendDlgListboxMessage(IDC_LISTBOX_MSG, szMacstr);
		WriteMacToFile(szMacstr, szFileName);
		record++;

	}
	else
	{
		((CButton*)GetDlgItem(IDOK))->EnableWindow(TRUE);
		SendDlgListboxMessage(IDC_LISTBOX_MSG, ("No file found!"));
		return;
	}
	while (FindNextFile(h, &p))
	{
		//SendDlgListboxMessage(IDC_LISTBOX_MSG, p.cFileName);
		SeparateMacWithRule(p.cFileName, szMacstr);
		SendDlgListboxMessage(IDC_LISTBOX_MSG, szMacstr);
		WriteMacToFile(szMacstr, szFileName);
		record++;
	}
	FindClose(h);
	// print record number.
	_tcscpy(szRecordNum, ("record number: "));
	//w2c(szRecordNum_c, szRecordNum, sizeof(szRecordNum));
	sprintf(szRecordNum, "%s%04d", szRecordNum, record);
	//c2w(szRecordNum, sizeof(szRecordNum_c), szRecordNum_c);
	SendDlgListboxMessage(IDC_LISTBOX_MSG, szRecordNum);

	// print save file name.
	SendDlgListboxMessage(IDC_LISTBOX_MSG, ("Mac list save to file:"));
	SendDlgListboxMessage(IDC_LISTBOX_MSG, szFileName);
	((CButton*)GetDlgItem(IDOK))->EnableWindow(TRUE);
#endif
#else
	CreateThread(NULL, 0, CMFCListMacDlg::FindFileThread, this, 0, NULL);
#endif
}

void CMFCListMacDlg::SeparateMacWithRule(char* szString, char* szMacString)
{
	char szInputStr[50] = ("");
	_tcscpy(szInputStr, szString);
	int lenInputStr = 0;
	int lenBlanChar = 0;
	int numi = 0, numj = 0, numk = 0;
	
	lenInputStr = strlen(szInputStr);
	lenBlanChar = strlen(szBlankChar);
	for (numi = 0; numi < lenInputStr; numi++)
	{
		if (szInputStr[numi] == ('.'))
		{
			szInputStr[numi] = ('\0');
			lenInputStr = numi + 1;
			break;
		}
	}
	for (numk = 0; numk < lenBlanChar; numk++)
	{
		for (numi = 0; numi<lenInputStr; numi++)
		{
			if (szBlankChar[numk] == szInputStr[numi])
			{
				for (numj = numi; numj < lenInputStr; numj++)
				{
					szInputStr[numj] = szInputStr[numj + 1];
				}
				numi -= 1;
				lenInputStr -= 1;
			}
		}
	}
	_tcscpy(szMacString, szInputStr);
}

void CMFCListMacDlg::WriteMacToFile(char* szMacString, char* szfileName)
{
	char szInputStr[50] = ("");
	char szFileName[MAX_PATH] = ("");
	FILE *pStream = NULL;

	ZeroMemory(szInputStr, sizeof(szInputStr));
	ZeroMemory(szFileName, sizeof(szFileName));

	_tcscpy(szInputStr, szMacString);

	_tcscpy(szFileName, szfileName);
	_tcscat(szFileName, (".csv"));

	pStream = fopen(szFileName, ("a+"));
	if (pStream)
	{
		fprintf(pStream, szInputStr);
		fprintf(pStream, (",\n"));
	}
	fclose(pStream);
}