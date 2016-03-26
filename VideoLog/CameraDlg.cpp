// CameraDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VideoLog.h"
#include "CameraDlg.h"
#include "afxdialogex.h"
#pragma comment (lib,"Gdiplus.lib")

// CameraDlg �Ի���

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


// CameraDlg ��Ϣ�������


BOOL CameraDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	GetDlgItem(IDC_PICTURE)->MoveWindow(50,50,400,300);
	pCapture = new Capture(GetDlgItem(IDC_PICTURE)->GetSafeHwnd());
	GetDlgItem(IDC_STOP_BTN)->MoveWindow(350,350,100,50);
	GetDlgItem(IDC_STOP_BTN)->EnableWindow(FALSE);
	GetDlgItem(IDC_START_BTN)->MoveWindow(50,350,100,50);
	GetDlgItem(IDC_TITLE)->MoveWindow(50,370,50,20);
	GetDlgItem(IDC_VIDEO_NAME)->MoveWindow(100,370,350,20);
	GetDlgItem(IDC_NOTE_EDIT)->MoveWindow(50,395,400,70);
	GetDlgItem(IDC_NOTE_OK)->MoveWindow(450,395,70,70);
	SetDlgItemText(IDC_NOTE_EDIT,L"��");
	SetDlgItemText(IDC_VIDEO_NAME,L"�ޱ���");
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CameraDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	//��ȡ��Ƶ����ͼ
	TCHAR *video = ::GetVideoFullFileName();
	TCHAR *image = ::GetImageFullFileName();
	::GetScreenShotJpg(video,0.5,PROPROTION,image);
	//���뵽�������ݿ�
	InsertVideoLogRecord();
	//֪ͨ��Ƶ�б�Ի��򣬲�ѯ���һ�εļ�¼��Ҳ������εļ�¼������ʾ���б����
	extern HWND videoListDlgHwnd;
	::PostMessage(videoListDlgHwnd,WM_SELECT_LAST_RECORD,0,0);
	//��֪������һ����Ƶ��¼����
	NotifyServerVideoRecod();
	//��ʼ�ϴ���Ƶ��ͼƬ
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
	//�����ͷ�������TCP����
	//initialize winsock
	WSADATA wsaData;
	int rs;
	if ((rs = WSAStartup(MAKEWORD(2,0),&wsaData)) != NOERROR)
	{
		MessageBox(L"winsock��ʼ��ʧ��!",L"CameraDlg");
		return false;
	}
	//create  socket
	SOCKET sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (INVALID_SOCKET == sockfd)
	{
		MessageBox(L"�����׽���ʧ�ܣ�",L"CameraDlg");
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
		MessageBox(L"��������ʧ��",L"CameraDlg");
		//printf("connect failed with error:%d\n",WSAGetLastError());
		closesocket(sockfd);
		WSACleanup();
		return false;
	}

	//��֪�������ͣ��������ݿ���Ƶ��¼:videodb
	send(sockfd,"videodb",strlen("videodb"),0);
	//�ȴ���������Ӧ�����ܽ�����һ��
	char tem[4] = {'\0'};
	recv(sockfd,tem,4,0);
	if (strcmp("yes",tem) != 0)
	{
		MessageBox(L"�������޷�ʶ������",L"CameraDlg");
		closesocket(sockfd);
		WSACleanup();
		return false;
	}
	//����Ҫ�������Ϣ����Ƶ���⡢�ļ��������ߡ�����������Ϣ
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
