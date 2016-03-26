// MailDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoLog.h"
#include "MailDlg.h"
#include "afxdialogex.h"


// CMailDlg 对话框

IMPLEMENT_DYNAMIC(CMailDlg, CDialogEx)

CMailDlg::CMailDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMailDlg::IDD, pParent)
{
	// Initialize GDI+.
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	ZeroMemory(m_ImagePath,sizeof(m_ImagePath));
	m_pVideoFileName = NULL;
}

CMailDlg::~CMailDlg()
{
	GdiplusShutdown(gdiplusToken);
}

void CMailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FRIEND_COMBO, m_FriendComBox);
}


BEGIN_MESSAGE_MAP(CMailDlg, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_SEND, &CMailDlg::OnBnClickedSend)
END_MESSAGE_MAP()


// CMailDlg 消息处理程序


BOOL CMailDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//TODO: 设置各控件的大小和位置
	GetDlgItem(IDC_VIDEO_PREVIEW)->MoveWindow(5,40,320,240);
	
	//初始化HDC和Graphics
	HDC hdc = ::GetDC(GetDlgItem(IDC_VIDEO_PREVIEW)->m_hWnd);
	m_pGraphics = new Graphics(hdc);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CMailDlg::AddFriendAddrs(LPCTSTR name,LPCTSTR id)
{
	CString str;
	str.Format(L"%s<%s>",name,id);
	m_FriendComBox.AddString(str);
}


void CMailDlg::SetAddressee(LPCTSTR name,LPCTSTR id)
{
	AddFriendAddrs(name,id);
	m_FriendComBox.SetCurSel(0);
}


void CMailDlg::SetPreviewImage(WCHAR*imagePath)
{
	m_pPreviewImage = new Image(imagePath);
	_stprintf(m_ImagePath,imagePath);

	CString cstrVideoFilename(imagePath);
	cstrVideoFilename.Replace(L".\\images\\",L"");
	cstrVideoFilename.Replace(L"jpg",L"avi");
	if (m_pVideoFileName != NULL)
	{
		delete[] m_pVideoFileName;
		m_pVideoFileName = NULL;
	}
	m_pVideoFileName = new TCHAR[cstrVideoFilename.GetLength()+1];
	memcpy(m_pVideoFileName,cstrVideoFilename.GetBuffer(),(cstrVideoFilename.GetLength()+1)*sizeof(TCHAR));
}


void CMailDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	m_pGraphics->DrawImage(m_pPreviewImage,Rect(0,0,320,240),0,0,320,240,UnitPixel,NULL);
}


void CMailDlg::OnBnClickedSend()
{
	//建立和服务器的TCP连接
	//initialize winsock
	WSADATA wsaData;
	int rs;
	if ((rs = WSAStartup(MAKEWORD(2,0),&wsaData)) != NOERROR)
	{
		MessageBox(L"winsock初始化失败!",L"CameraDlg");
		return;
	}
	//create  socket
	SOCKET sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (INVALID_SOCKET == sockfd)
	{
		MessageBox(L"创建套接字失败！",L"MailDlg");
		WSACleanup();
		return;
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
		MessageBox(L"连接网络失败",L"MailDlg");
		//printf("connect failed with error:%d\n",WSAGetLastError());
		closesocket(sockfd);
		WSACleanup();
		return;
	}

	//告知事务类型：插入数据库视频邮件记录:sendmail
	send(sockfd,"sendmail",strlen("sendmail"),0);
	//等待服务器的应答后才能进行下一步
	char tem[4] = {'\0'};
	recv(sockfd,tem,4,0);
	if (strcmp("yes",tem) != 0)
	{
		MessageBox(L"服务器无法识别请求",L"MailDlg");
		closesocket(sockfd);
		WSACleanup();
		return;
	}
	//发送要插入的信息：发送人，接收人，视频文件名，附加文字信息
	TCHAR *sender = pINI->GetUserName();
	CString receiver;
	m_FriendComBox.GetWindowText(receiver);
	int istart = receiver.Find(L'<') + 1;
	int nCount = receiver.Find(L'>') - 1;
	nCount = nCount - istart +1;
	receiver = receiver.Mid(istart,nCount);
	
	CString content;
	GetDlgItemText(IDC_MAIL_CONTENT,content);
	CString wsendbuf;
	
	wsendbuf.Format(L"%s,%s,%s,%s",sender,receiver,m_pVideoFileName,content);
	delete[] sender;

	char *utf8str = WideCharToUTF8(wsendbuf.GetBuffer());
	send(sockfd,utf8str,strlen(utf8str),0);
	delete[] utf8str;
}
