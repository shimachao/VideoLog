// CameraDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoLog.h"
#include "CameraDlg.h"
#include "afxdialogex.h"
#pragma comment (lib,"Gdiplus.lib")

// CameraDlg 对话框

IMPLEMENT_DYNAMIC(CameraDlg, CDialogEx)

CameraDlg::CameraDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CameraDlg::IDD, pParent)
	, pCapture(NULL)
	, pGraphics(NULL)
	, gdiplusToken(0)
	, m_CameraFlag(FALSE)
{
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken,&gdiplusStartupInput,NULL);
}

CameraDlg::~CameraDlg()
{
	if (pCapture != NULL)
	{
		pCapture->Stop();
		delete pCapture;
	}

	if (pGraphics != NULL)
	{
		delete pGraphics;
	}
	GdiplusShutdown(gdiplusToken);
}

void CameraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_STOP_CAPTURE, m_StopBtn);
}


BEGIN_MESSAGE_MAP(CameraDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_DRAWITEM()
	ON_WM_ERASEBKGND()
	ON_WM_DRAWITEM()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_START_BTN, &CameraDlg::OnBnClickedStartBtn)
	ON_BN_CLICKED(IDC_STOP_BTN, &CameraDlg::OnBnClickedStopBtn)
	ON_BN_CLICKED(IDC_NOTE_OK, &CameraDlg::OnBnClickedNoteOk)
END_MESSAGE_MAP()


// CameraDlg 消息处理程序


BOOL CameraDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	
	GetDlgItem(IDC_PICTURE)->MoveWindow(50,50,400,300);
	pCapture = new Capture(GetDlgItem(IDC_PICTURE)->GetSafeHwnd());
	GetDlgItem(IDC_STOP_BTN)->MoveWindow(350,350,100,50);
	GetDlgItem(IDC_STOP_BTN)->EnableWindow(FALSE);
	GetDlgItem(IDC_START_BTN)->MoveWindow(50,350,100,50);
	GetDlgItem(IDC_TITLE)->MoveWindow(50,370,50,20);
	GetDlgItem(IDC_VIDEO_NAME)->MoveWindow(100,370,350,20);
	GetDlgItem(IDC_NOTE_EDIT)->MoveWindow(50,395,400,70);
	GetDlgItem(IDC_NOTE_OK)->MoveWindow(450,395,70,70);
	SetDlgItemText(IDC_NOTE_EDIT,L"无");
	SetDlgItemText(IDC_VIDEO_NAME,L"无标题");
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CameraDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	pCapture->Stop();
	//delete pCapture;
	CDialogEx::OnClose();
}



void CameraDlg::OnBnClickedStartBtn()
{
	GetDlgItem(IDC_START_BTN)->EnableWindow(FALSE);
	pCapture->Run();
	GetDlgItem(IDC_STOP_BTN)->EnableWindow(TRUE);
}


void CameraDlg::OnBnClickedStopBtn()
{
	GetDlgItem(IDC_START_BTN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STOP_BTN)->ShowWindow(SW_HIDE);
	pCapture->Stop();

	GetDlgItem(IDC_TITLE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_VIDEO_NAME)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_NOTE_EDIT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_NOTE_OK)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_VIDEO_NAME)->SetFocus();
}


void CameraDlg::OnBnClickedNoteOk()
{
	GetDlgItem(IDC_TITLE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_VIDEO_NAME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_NOTE_EDIT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_NOTE_OK)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_PICTURE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_START_BTN)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STOP_BTN)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_START_BTN)->EnableWindow(TRUE);
	GetDlgItem(IDC_STOP_BTN)->EnableWindow(FALSE);
	//提取视频缩略图
	TCHAR *video = ::GetVideoFullFileName();
	TCHAR *image = ::GetImageFullFileName();
	::GetScreenShotJpg(video,0.5,PROPROTION,image);
	//插入到本地数据库
	InsertVideoLogRecord();
	//通知视频列表对话框，查询最近一次的记录（也就是这次的记录）并显示在列表框上
	extern HWND videoListDlgHwnd;
	::PostMessage(videoListDlgHwnd,WM_SELECT_LAST_RECORD,0,0);
	//告知服务器一条视频记录产生
	NotifyServerVideoRecod();
	//开始上传视频和图片
	extern HWND upDownDlghwnd;
	::SendMessage(upDownDlghwnd,WM_UP,0,0);
}


void CameraDlg::InsertVideoLogRecord(void)
{
	CString videotitle;
	CString explain;
	GetDlgItemText(IDC_VIDEO_NAME,videotitle);
	GetDlgItemText(IDC_NOTE_EDIT,explain);
	char cvideotitle[50];
	TcharToChar(videotitle.GetBuffer(),cvideotitle);
	char cexplain[255];
	TcharToChar(explain.GetBuffer(),cexplain);
	
	TCHAR *videoURL = ::GetVideoFullFileName();
	TCHAR *imageURL = ::GetImageFullFileName();
	char cvideoURL[255] = {'\0'};
	TcharToChar(videoURL,cvideoURL);
	char cimageURL[255] = {'\0'};
	TcharToChar(imageURL,cimageURL);
	CINIManager *pINI = CINIManager::GetINIManager();
	char *username = pINI->GetcharUserName();
	m_VideoDB.InsertVideoRecord(cimageURL,cvideotitle,username,NULL,cvideoURL,cexplain);
	delete[] username;
}


bool CameraDlg::NotifyServerVideoRecod(void)
{
	//建立和服务器的TCP连接
	//initialize winsock
	WSADATA wsaData;
	int rs;
	if ((rs = WSAStartup(MAKEWORD(2,0),&wsaData)) != NOERROR)
	{
		MessageBox(L"winsock初始化失败!",L"CameraDlg");
		return false;
	}
	//create  socket
	SOCKET sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (INVALID_SOCKET == sockfd)
	{
		MessageBox(L"创建套接字失败！",L"CameraDlg");
		WSACleanup();
		return false;
	}
	//connect sockfd to server
	CINIManager* pINI = CINIManager::GetINIManager();
	sockaddr_in servAddr;
	memset(&servAddr,0,sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = pINI->GetServerIP();
	servAddr.sin_port = pINI->GetServicesPort();
	if (SOCKET_ERROR == connect(sockfd,(SOCKADDR*)&servAddr,sizeof(servAddr)))
	{
		MessageBox(L"连接网络失败",L"CameraDlg");
		//printf("connect failed with error:%d\n",WSAGetLastError());
		closesocket(sockfd);
		WSACleanup();
		return false;
	}

	//告知事务类型：插入数据库视频记录:videodb
	send(sockfd,"videodb",strlen("videodb"),0);
	//等待服务器的应答后才能进行下一步
	char tem[4] = {'\0'};
	recv(sockfd,tem,4,0);
	if (strcmp("yes",tem) != 0)
	{
		MessageBox(L"服务器无法识别请求",L"CameraDlg");
		closesocket(sockfd);
		WSACleanup();
		return false;
	}
	//发送要插入的信息：视频标题、文件名、作者、附加文字信息
	CString title;
	CString explainstr;
	GetDlgItemText(IDC_VIDEO_NAME,title);
	GetDlgItemText(IDC_NOTE_EDIT,explainstr);
	CString wsendbuf;
	TCHAR *autor = pINI->GetUserName();
	wsendbuf.Format(L"%s,%s,%s,%s",title,::GetVideoFileName(),autor,explainstr);
	delete[] autor;

	char *utf8str = WideCharToUTF8(wsendbuf.GetBuffer());
	send(sockfd,utf8str,strlen(utf8str),0);
	delete[] utf8str;
	
}
