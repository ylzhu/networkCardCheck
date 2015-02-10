
// MFCListMacDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Networkcard.h"
#include "NetworkcardDlg.h"
#include "afxdialogex.h"
#include "windows.h"

//#param warning(disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


char *w2c(char *pcstr, const wchar_t *pwstr, size_t len)
{
	int nlength = wcslen(pwstr);

	//��ȡת����ĳ���
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

	// ͨ�����ϵõ��Ľ����ת��unicode �ַ�Ϊascii �ַ�
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

//��char* ת��wchar_t*��ʵ�ֺ������£� 

//���ǰ�asii�ַ�ת��Ϊunicode�ַ�����������ͬ��ԭ��
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CMFCListMacDlg �Ի���



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


// CMFCListMacDlg ��Ϣ�������

BOOL CMFCListMacDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	ZeroMemory(szSaveFileName, sizeof(szSaveFileName));
	ZeroMemory(szSuffix, sizeof(szSuffix));
	ZeroMemory(szBlankChar, sizeof(szBlankChar));
	ZeroMemory(szPath, sizeof(szPath));
	
	//strcpy(szSaveFileName, "MacAddressList");
	wcscpy_s(szSaveFileName, "MacAddressList");
	wcscpy_s(szSuffix, TEXT("cer"));
	wcscpy_s(szBlankChar, TEXT("-"));
	SetDlgItemTextW(IDC_EDIT_SUFFIX, szSuffix);
	SetDlgItemTextW(IDC_EDIT_BLANK_CHAR, szBlankChar);
	SetDlgItemTextW(IDC_EDIT_SAVE_FILENAME, szSaveFileName);

	SendDlgListboxMessage(IDC_LISTBOX_MSG, TEXT("Ĭ�Ϻ�׺��:"));
	SendDlgListboxMessage(IDC_LISTBOX_MSG, szSuffix);
	SendDlgListboxMessage(IDC_LISTBOX_MSG, TEXT("Ĭ�Ϲ��˼����:"));
	SendDlgListboxMessage(IDC_LISTBOX_MSG, szBlankChar);
	SendDlgListboxMessage(IDC_LISTBOX_MSG, TEXT("Ĭ�ϱ����ļ���:"));
	SendDlgListboxMessage(IDC_LISTBOX_MSG, szSaveFileName);

	((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(1);
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK1))
	{
		// ��ѡ
		u8checkBoxStatus = 1;
		SendDlgListboxMessage(IDC_LISTBOX_MSG, TEXT("�б���Ϣȥ���ļ���׺."));
	}
	else
	{
		u8checkBoxStatus = 0;
	}
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFCListMacDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMFCListMacDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCListMacDlg::OnEnChangeBlank_char()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	GetDlgItemTextW(IDC_EDIT_BLANK_CHAR, szBlankChar, 20);
	//GetDlgItemTextW(IDC_EDIT_BLANK_CHAR, szBlankChar);
	//GetDlgItemTextW(IDC_EDIT_SAVE_FILENAME, szSaveFileName);
	//SendDlgListboxMessage(IDC_LISTBOX_MSG, TEXT("ȥ��:"));
	//SendDlgListboxMessage(IDC_LISTBOX_MSG, szBlankChar);

}


void CMFCListMacDlg::OnBnClickedButtonCurrentPath()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	GetCurrentDirectory(sizeof(szPath), szPath);
	//MessageBox(szPath);
	//SetWindowTextW(GetDlgItem(hDlg, IDC_BUTTON_CURRENT_PATH), _T(""));
	SetDlgItemTextW(IDC_STATIC_PATH, szPath);

	//SetDlgItemTextW(IDC_LISTBOX_MSG, TEXT("Current path:"));
	//SetDlgItemTextW(IDC_LISTBOX_MSG, szPath);

	SendDlgListboxMessage(IDC_LISTBOX_MSG, TEXT("Current path:"));
	SendDlgListboxMessage(IDC_LISTBOX_MSG, szPath);
}

void CMFCListMacDlg::SendDlgListboxMessage(int listboxID, TCHAR* szString)
{
#if 1
	long lNumber;
	SendDlgItemMessage(listboxID, LB_ADDSTRING, NULL, reinterpret_cast<LPARAM>(szString));	//��ӡ�ִ���listbox
	lNumber = SendDlgItemMessage(listboxID, LB_GETCOUNT, NULL, NULL);						//��ȡlistbox���ִ���Ŀ
	SendDlgItemMessage(listboxID, LB_SETCURSEL, lNumber - 1, 0);							//ʹ���ִ���ѡ, ͬʱ����ʹ�ִ���listbox�Ŀɼ�����.
	SendDlgItemMessage(listboxID, LB_SETCURSEL, -1, 0);		
	//UpdateData(true);
#endif//ȡ�����з�ѡ.
	
}


void CMFCListMacDlg::OnBnClickedButtonOtherPath()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	WCHAR szPath_tmp[MAX_PATH];     //���ѡ���Ŀ¼·�� 
	//CString str;

	ZeroMemory(szPath_tmp, sizeof(szPath_tmp));

	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPath_tmp;
	bi.lpszTitle = TEXT("��ѡ����Ҫ�����Ŀ¼��");
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	//����ѡ��Ŀ¼�Ի���
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);

	if (lp && SHGetPathFromIDList(lp, szPath_tmp))
	{
		//str.Format(TEXT("ѡ���Ŀ¼Ϊ %s"), szPath);
		//MessageBox(str);
		wcscpy_s(szPath, szPath_tmp);
		SetDlgItemTextW(IDC_STATIC_PATH, szPath);
		SendDlgListboxMessage(IDC_LISTBOX_MSG, TEXT("Select path:"));
		SendDlgListboxMessage(IDC_LISTBOX_MSG, szPath);


	}
	else
	{
		MessageBox(TEXT("��Ч��Ŀ¼��������ѡ��"));
		SendDlgListboxMessage(IDC_LISTBOX_MSG, TEXT("��Ч��Ŀ¼��������ѡ��"));
	}
		
}


void CMFCListMacDlg::OnEnChangeEditSuffix()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	GetDlgItemTextW(IDC_EDIT_SUFFIX, szSuffix, 20);
	//GetDlgItemTextW(IDC_EDIT_BLANK_CHAR, szBlankChar);
	//GetDlgItemTextW(IDC_EDIT_SAVE_FILENAME, szSaveFileName);
	//SendDlgListboxMessage(IDC_LISTBOX_MSG, TEXT("��׺����Ϊ:"));
	//SendDlgListboxMessage(IDC_LISTBOX_MSG, szSuffix);
	
}


void CMFCListMacDlg::OnBnClickedCheck1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK1))
	{
		// ��ѡ
		u8checkBoxStatus = 1;
		//SendDlgListboxMessage(IDC_LISTBOX_MSG, TEXT("�б���Ϣȥ���ļ���׺."));
	}
	else
	{
		u8checkBoxStatus = 0;
		//SendDlgListboxMessage(IDC_LISTBOX_MSG, TEXT("�б���Ϣ�����ļ���׺."));
	}
}


void CMFCListMacDlg::OnEnChangeEditSaveFilename()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	GetDlgItemTextW(IDC_EDIT_SAVE_FILENAME, szSaveFileName, 50);
	
	//SendDlgListboxMessage(IDC_LISTBOX_MSG, TEXT("�ļ���:"));
	//SendDlgListboxMessage(IDC_LISTBOX_MSG, szSaveFileName);
}

#if 0
 DWORD WINAPI CMFCListMacDlg::FindFileThread(LPVOID lpParameter)
{
	WCHAR szPath_tmp[500];
	WCHAR szMacstr[50];
	WCHAR szWildcard[20];
	WCHAR szFileName[MAX_PATH];
	char szFilename_c[MAX_PATH];

	WCHAR szRecordNum[60];
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
		_tcscpy(szWildcard, TEXT("*."));
	}
	else
	{
		_tcscpy(szWildcard, TEXT("\\*."));
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
		SendDlgListboxMessage(IDC_LISTBOX_MSG, TEXT("No file found!"));
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
	_tcscpy(szRecordNum, TEXT("record number: "));
	w2c(szRecordNum_c, szRecordNum, sizeof(szRecordNum));
	sprintf(szRecordNum_c, "%s%04d", szRecordNum_c, record);
	c2w(szRecordNum, sizeof(szRecordNum_c), szRecordNum_c);
	SendDlgListboxMessage(IDC_LISTBOX_MSG, szRecordNum);

	// print save file name.
	SendDlgListboxMessage(IDC_LISTBOX_MSG, TEXT("Mac list save to file:"));
	SendDlgListboxMessage(IDC_LISTBOX_MSG, szFileName);
	((CButton*)GetDlgItem(IDOK))->EnableWindow(TRUE);
	return 1;
#endif
}
#endif


void CMFCListMacDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
#if 1
	WCHAR szPath_tmp[500];
	WCHAR szMacstr[50];
	WCHAR szWildcard[20];
	WCHAR szFileName[MAX_PATH];
	char szFilename_c[MAX_PATH];

	WCHAR szRecordNum[60];
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
		_tcscpy(szWildcard, TEXT("*."));
	}
	else
	{
		_tcscpy(szWildcard, TEXT("\\*."));
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
		SendDlgListboxMessage(IDC_LISTBOX_MSG, TEXT("No file found!"));
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
	_tcscpy(szRecordNum, TEXT("record number: "));
	w2c(szRecordNum_c, szRecordNum, sizeof(szRecordNum));
	sprintf(szRecordNum_c, "%s%04d", szRecordNum_c, record);
	c2w(szRecordNum, sizeof(szRecordNum_c), szRecordNum_c);
	SendDlgListboxMessage(IDC_LISTBOX_MSG, szRecordNum);

	// print save file name.
	SendDlgListboxMessage(IDC_LISTBOX_MSG, TEXT("Mac list save to file:"));
	SendDlgListboxMessage(IDC_LISTBOX_MSG, szFileName);
	((CButton*)GetDlgItem(IDOK))->EnableWindow(TRUE);
#endif
#else
	CreateThread(NULL, 0, CMFCListMacDlg::FindFileThread, this, 0, NULL);
#endif
}

void CMFCListMacDlg::SeparateMacWithRule(TCHAR* szString, TCHAR* szMacString)
{
	WCHAR szInputStr[50] = TEXT("");
	_tcscpy(szInputStr, szString);
	int lenInputStr = 0;
	int lenBlanChar = 0;
	int numi = 0, numj = 0, numk = 0;
	
	lenInputStr = wcslen(szInputStr);
	lenBlanChar = wcslen(szBlankChar);
	for (numi = 0; numi < lenInputStr; numi++)
	{
		if (szInputStr[numi] == TEXT('.'))
		{
			szInputStr[numi] = TEXT('\0');
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

void CMFCListMacDlg::WriteMacToFile(TCHAR* szMacString, TCHAR* szfileName)
{
	WCHAR szInputStr[50] = TEXT("");
	WCHAR szFileName[MAX_PATH] = TEXT("");
	FILE *pStream = NULL;

	ZeroMemory(szInputStr, sizeof(szInputStr));
	ZeroMemory(szFileName, sizeof(szFileName));

	_tcscpy(szInputStr, szMacString);

	_tcscpy(szFileName, szfileName);
	_tcscat(szFileName, TEXT(".csv"));

	pStream = _wfopen(szFileName, TEXT("a+"));
	if (pStream)
	{
		fwprintf(pStream, szInputStr);
		fwprintf(pStream, TEXT(",\n"));
	}
	fclose(pStream);
}