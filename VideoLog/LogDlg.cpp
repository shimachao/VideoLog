// CLogDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoLog.h"
#include "LogDlg.h"
#include "afxdialogex.h"
#include "LogProc.h"
#include "INIManager.h"
#include <WinSock2.h>
#include <memory.h>
#pragma comment(lib,"Ws2_32.lib")

#define  PORT 34365
// CLogDlg 对话框

IMPLEMENT_DYNAMIC(CLogDlg, CDialogEx)

CLogDlg::CLogDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLogDlg::IDD, pParent)
	, account(NULL)
	, password(NULL)
	, pLogProc(NULL)
	, pGraphics(NULL)
	, gdiplusToken(0)
{
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken,&gdiplusStartupInput,NULL);
}

CLogDlg::~CLogDlg()
{
	closesocket(m_sock);
	if (pGraphics != NULL)
	{
		delete pGraphics;
	}
	GdiplusShutdown(gdiplusToken);
	WSACleanup();
}

void CLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLogDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_LOG_BUTTON, &CLogDlg::OnBnClickedLogButton)
	ON_MESSAGE(WM_UDPSOCKREAD, &CLogDlg::OnUdpsockread)
	ON_STN_CLICKED(IDC_REGISTER, &CLogDlg::OnClickedRegister)
	ON_STN_CLICKED(IDC_FORGOT_PASSWORD, &CLogDlg::OnClickedForgotPassword)
	ON_MESSAGE(WM_LOG_CANCEL, &CLogDlg::OnLogCancel)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CLogDlg 消息处理程序


BOOL CLogDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//初始化winsock
	WSADATA wsaData;
	int rs = WSAStartup(MAKEWORD(2,0),&wsaData);
	if (NOERROR != rs)
	{
		MessageBox(L"WSAStartp failed!",L"error!",MB_OK | MB_ICONWARNING);
		return TRUE;
	}
	m_sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if (INVALID_SOCKET == m_sock)
	{
		MessageBox(L"创建udp套接字失败!",L"error",MB_ICONWARNING);
		return TRUE;
	}
	CINIManager *pINI = CINIManager::GetINIManager();
	sockaddr_in servAddr;
	memset(&servAddr,0,sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = pINI->GetServerIP();
	servAddr.sin_port = pINI->GetServerLogPort();
	if (connect(m_sock,(struct sockaddr*)&servAddr,sizeof(servAddr)) != 0)
	{
		MessageBox(L"connect 失败!",L"ERROR",MB_ICONWARNING);
	}

	if(WSAAsyncSelect(m_sock,m_hWnd,WM_UDPSOCKREAD,FD_READ) != 0)
	{
		MessageBox(L"WSAAsyncSelect failed",L"error",MB_ICONWARNING);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


HBRUSH CLogDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	// TODO:  在此更改 DC 的任何特性
	if (CTLCOLOR_STATIC == nCtlColor )
	{
		switch (pWnd->GetDlgCtrlID())
		{
		case IDC_MSG:
			pDC->SetTextColor(RGB(255,0,0));
			break;
		case IDC_REGISTER:case IDC_FORGOT_PASSWORD:
			pDC->SetTextColor(RGB(0,0,255));
			break;
		}
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)GetStockObject(NULL_BRUSH); 
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	//return (HBRUSH)GetStockObject(NULL_BRUSH); 
	return hbr;
}


void CLogDlg::OnBnClickedLogButton()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR emailW[33] = {'\0'};
	TCHAR passwordW[33] = {'\0'};
	GetDlgItemText(IDC_ACCOUNT_EDIT,emailW,32);
	GetDlgItemText(IDC_PASSWORD_EDIT,passwordW,32);
	if (NULL == account)
	{
		account = new char[33];
		memset(account,0,33);
	}
	if (NULL == password)
	{
		password = new char[33];
		memset(password,0,33);
	}
	
	//将这次输入的用户名保存到配置文件
	CINIManager *pINI = CINIManager::GetINIManager();
	pINI->WriteUserName(emailW);

	TcharToChar(emailW,account);
	TcharToChar(passwordW,password);
	login();//发送账号和密码到服务器登录
	ShowWindow(SW_HIDE);
	if (pLogProc == NULL)
	{
		pLogProc = new CLogProc;
		pLogProc->Create(IDD_LOGPROC_DLOG,this);//显示登录过程动画界面
	}
	pLogProc->ShowWindow(SW_SHOW);
	
}


void CLogDlg::TcharToChar(const TCHAR* tchar, char* _char)
{
	int iLength ;  
	//获取字节长度   
	iLength = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);  
	//将tchar值赋给_char    
	WideCharToMultiByte(CP_ACP, 0, tchar, -1, _char, iLength, NULL, NULL);   
}


// 用户登陆过程
int CLogDlg::login(void)
{
	//邮箱号长度 邮箱号 密码长度 密码 
	//邮箱号长度和密码长度均为unsigned char类型，占一个字节
	char *sendbuf = new char[strlen(password)+strlen(account)+2];
	memset(sendbuf,0,strlen(password)+strlen(account)+2);
	sendbuf[0] = (unsigned char)strlen(account);
	memcpy(&sendbuf[1],account,strlen(account));
	sendbuf[1+strlen(account)] = (unsigned char)strlen(password);
	memcpy(&sendbuf[2+strlen(account)],password,strlen(password));

	send(m_sock,sendbuf,strlen(password)+strlen(account)+2,0);
	free(sendbuf);
	return 0;
}


afx_msg LRESULT CLogDlg::OnUdpsockread(WPARAM wParam, LPARAM lParam)
{
	char buf[4] = {'\0'};
	int len = recv(m_sock,buf,4,0);
	if(0 == buf[0])
		return 0;

	if ('n' == buf[0])
	{
		//MessageBox(L"登陆失败，用户名或密码错误",L"error",MB_OK);
		SetDlgItemText(IDC_MSG,L"用户名或密码错误，请重新输入");
		pLogProc->Stop();
		ShowWindow(SW_SHOW);
	}
	else if('y' == buf[0])
	{
		int id = *(int*)&buf[1];
		//MessageBox(L"登陆成功",L"message",MB_OK);
		SetDlgItemText(IDC_MSG,L"登陆成功！");
		pLogProc->Stop();
		OnOK();
	}
	return 0;
}


void CLogDlg::OnClickedRegister()
{
	ShellExecute(NULL,L"open",L"http://121.199.24.119/register1.html",NULL,NULL,SW_SHOWNORMAL);
	// TODO: 在此添加控件通知处理程序代码
}


void CLogDlg::OnClickedForgotPassword()
{
	// TODO: 在此添加控件通知处理程序代码
	ShellExecute(NULL,L"open",L"http://121.199.24.119/forgetpwd.php",NULL,NULL,SW_SHOWNORMAL);
}


afx_msg LRESULT CLogDlg::OnLogCancel(WPARAM wParam, LPARAM lParam)
{
	ShowWindow(SW_SHOW);
	//OnCancel();
	return 0;
}


void CLogDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	HDC hdc = ::GetDC(m_hWnd);
	if (pGraphics == NULL)
	{
		pGraphics = new Graphics(hdc);
	}
	CRect rect;
	GetClientRect(&rect);
	Image dlgBkimage(L"res/dlg.jpg");
	pGraphics->DrawImage(&dlgBkimage,Rect(0,0,800,470),0,0,rect.Width(),rect.Height(),UnitPixel,NULL);
}
