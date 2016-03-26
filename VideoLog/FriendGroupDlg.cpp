// FriendGroupDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoLog.h"
#include "FriendGroupDlg.h"
#include "afxdialogex.h"
#include "INIManager.h"
#include "MailDlg.h"
#include <list>
using std::list;


// CFriendGroupDlg 对话框

IMPLEMENT_DYNAMIC(CFriendGroupDlg, CDialogEx)

CFriendGroupDlg::CFriendGroupDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFriendGroupDlg::IDD, pParent)
{
	m_Sockfd = INVALID_SOCKET;
	WSADATA wsaData;
	int rs;
	 ASSERT((rs = WSAStartup(MAKEWORD(2,0),&wsaData)) == NOERROR);
	//创建套接字
	 CINIManager *pINI = CINIManager::GetINIManager();
	m_Sockfd = connectToServer(pINI->GetServerIP(),pINI->GetServicesPort());

}

CFriendGroupDlg::~CFriendGroupDlg()
{
	WSACleanup();
}

void CFriendGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FRIEND_LIST, m_FriendList);
}


BEGIN_MESSAGE_MAP(CFriendGroupDlg, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_FRIEND_LIST, &CFriendGroupDlg::OnNMRClickFriendList)
	ON_COMMAND(ID_SEND_VIDEO_MAIL, &CFriendGroupDlg::OnSendVideoMail)
END_MESSAGE_MAP()


// CFriendGroupDlg 消息处理程序


BOOL CFriendGroupDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_FriendList.MoveWindow(10,10,400,300);
	//设置列表的扩展风格
	DWORD dwStyle = m_FriendList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮
	dwStyle |= LVS_EX_GRIDLINES;	//网格线
	m_FriendList.SetExtendedStyle(dwStyle);
	m_FriendList.InsertColumn(0,L"好友ID",0,150);
	m_FriendList.InsertColumn(1,L"昵称",0,150);
	m_FriendList.InsertColumn(2,L"状态",0,100); //状态为：在线或离线

	//如果可以连接服务器，就从服务器用户好友列表
	if (m_Sockfd != INVALID_SOCKET)
	{
		//告知服务器:我要查询我的好友信息
		send(m_Sockfd,"friends",strlen("friends"),0);
		//等待服务器应答后才能进行下一步
		char tem[4] = {'\0'};
		recv(m_Sockfd,tem,4,0);
		if (strcmp("yes",tem) != 0)
		{
			AfxMessageBox(L"服务器无法处理friends请求");
			return TRUE;
		}
		//发送自己的用户id
		CINIManager *pINI = CINIManager::GetINIManager();
		char *userid = pINI->GetcharUserName();
		send(m_Sockfd,userid,strlen(userid),0);
		delete[] userid;
		//等待服务器批量回复好友信息:好友id，昵称，状态（在线or下线）
		char n = 0;
		if (recv(m_Sockfd,&n,1,0) <= 0)
		{
			return TRUE;
		}
		char *utf8str = new char[n+1];
		ZeroMemory(utf8str,n+1);
		int res = recv(m_Sockfd,utf8str,n,0);
		ASSERT(res == n);
		//处理好友信息
		WCHAR *wstr = utf8ToWideChar(utf8str);
		WCHAR *token;
		WCHAR seps[] = _T(",");
		
		extern list<CString> friendInfoList;

		token = wcstok(wstr,seps);
		while(token != NULL)
		{
			//插入到列表中
			int row = m_FriendList.GetItemCount();
			m_FriendList.InsertItem(row,token);//插入行同时设置列0
			friendInfoList.push_back(token);
			//设置列1
			token = wcstok(NULL,seps);
			if (NULL == token)
			{
				break;
			}
			m_FriendList.SetItemText(row,1,token);
			friendInfoList.push_back(token);

			token = wcstok(NULL,seps);
		}
		delete[] wstr;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CFriendGroupDlg::OnNMRClickFriendList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if(pNMItemActivate->iItem != -1)
	{
		CMenu menu;
		menu.LoadMenu(IDR_FRIEND_LIST_MENU);
		CMenu *pSubMenu = menu.GetSubMenu(0);
		CPoint point;
		GetCursorPos(&point);
		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);
		menu.DestroyMenu();
	}
	
	*pResult = 0;
}


void CFriendGroupDlg::OnSendVideoMail()
{
	// TODO: 在此添加命令处理程序代码
	//获得处于选中状态的用户昵称和ID
	POSITION pos = m_FriendList.GetFirstSelectedItemPosition();
	int nItem = 0;
	if (pos == NULL)
	{
		MessageBox(L"未选中一个好友");
		return;
	}
	
	nItem = m_FriendList.GetNextSelectedItem(pos);
	
	TCHAR id[512] = {'\0'};
	m_FriendList.GetItemText(nItem,0,id,512);
	TCHAR name[512] = {'\0'};
	m_FriendList.GetItemText(nItem,1,name,512);
	//弹出发送邮件菜单
	static CMailDlg* pMailDlg = NULL;
	if (pMailDlg != NULL)
	{
		delete pMailDlg;
		pMailDlg = NULL;
	}
	pMailDlg = new CMailDlg;
	pMailDlg->Create(IDD_MAIL_DLG);
	pMailDlg->SetAddressee(name,id);//设置收件人是谁
	pMailDlg->ShowWindow(SW_SHOW);
}
