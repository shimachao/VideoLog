// CLogDlg.cpp : ʵ���ļ�
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
// CLogDlg �Ի���

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


// CLogDlg ��Ϣ�������


BOOL CLogDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//��ʼ��winsock
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
		MessageBox(L"����udp�׽���ʧ��!",L"error",MB_ICONWARNING);
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
		MessageBox(L"connect ʧ��!",L"ERROR",MB_ICONWARNING);
	}

	if(WSAAsyncSelect(m_sock,m_hWnd,WM_UDPSOCKREAD,FD_READ) != 0)
	{
		MessageBox(L"WSAAsyncSelect failed",L"error",MB_ICONWARNING);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


HBRUSH CLogDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	// TODO:  �ڴ˸��� DC ���κ�����
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
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	//return (HBRUSH)GetStockObject(NULL_BRUSH); 
	return hbr;
}


void CLogDlg::OnBnClickedLogButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	
	//�����������û������浽�����ļ�
	CINIManager *pINI = CINIManager::GetINIManager();
	pINI->WriteUserName(emailW);

	TcharToChar(emailW,account);
	TcharToChar(passwordW,password);
	login();//�����˺ź����뵽��������¼
	ShowWindow(SW_HIDE);
	if (pLogProc == NULL)
	{
		pLogProc = new CLogProc;
		pLogProc->Create(IDD_LOGPROC_DLOG,this);//��ʾ��¼���̶�������
	}
	pLogProc->ShowWindow(SW_SHOW);
	
}


void CLogDlg::TcharToChar(const TCHAR* tchar, char* _char)
{
	int iLength ;  
	//��ȡ�ֽڳ���   
	iLength = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);  
	//��tcharֵ����_char    
	WideCharToMultiByte(CP_ACP, 0, tchar, -1, _char, iLength, NULL, NULL);   
}


// �û���½����
int CLogDlg::login(void)
{
	//����ų��� ����� ���볤�� ���� 
	//����ų��Ⱥ����볤�Ⱦ�Ϊunsigned char���ͣ�ռһ���ֽ�
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
		//MessageBox(L"��½ʧ�ܣ��û������������",L"error",MB_OK);
		SetDlgItemText(IDC_MSG,L"�û����������������������");
		pLogProc->Stop();
		ShowWindow(SW_SHOW);
	}
	else if('y' == buf[0])
	{
		int id = *(int*)&buf[1];
		//MessageBox(L"��½�ɹ�",L"message",MB_OK);
		SetDlgItemText(IDC_MSG,L"��½�ɹ���");
		pLogProc->Stop();
		OnOK();
	}
	return 0;
}


void CLogDlg::OnClickedRegister()
{
	ShellExecute(NULL,L"open",L"http://121.199.24.119/register1.html",NULL,NULL,SW_SHOWNORMAL);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CLogDlg::OnClickedForgotPassword()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
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
