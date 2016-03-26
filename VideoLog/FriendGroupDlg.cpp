// FriendGroupDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VideoLog.h"
#include "FriendGroupDlg.h"
#include "afxdialogex.h"
#include "INIManager.h"
#include "MailDlg.h"
#include <list>
using std::list;


// CFriendGroupDlg �Ի���

IMPLEMENT_DYNAMIC(CFriendGroupDlg, CDialogEx)

CFriendGroupDlg::CFriendGroupDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFriendGroupDlg::IDD, pParent)
{
	m_Sockfd = INVALID_SOCKET;
	WSADATA wsaData;
	int rs;
	 ASSERT((rs = WSAStartup(MAKEWORD(2,0),&wsaData)) == NOERROR);
	//�����׽���
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


// CFriendGroupDlg ��Ϣ�������


BOOL CFriendGroupDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_FriendList.MoveWindow(10,10,400,300);
	//�����б����չ���
	DWORD dwStyle = m_FriendList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и���
	dwStyle |= LVS_EX_GRIDLINES;	//������
	m_FriendList.SetExtendedStyle(dwStyle);
	m_FriendList.InsertColumn(0,L"����ID",0,150);
	m_FriendList.InsertColumn(1,L"�ǳ�",0,150);
	m_FriendList.InsertColumn(2,L"״̬",0,100); //״̬Ϊ�����߻�����

	//����������ӷ��������ʹӷ������û������б�
	if (m_Sockfd != INVALID_SOCKET)
	{
		//��֪������:��Ҫ��ѯ�ҵĺ�����Ϣ
		send(m_Sockfd,"friends",strlen("friends"),0);
		//�ȴ�������Ӧ�����ܽ�����һ��
		char tem[4] = {'\0'};
		recv(m_Sockfd,tem,4,0);
		if (strcmp("yes",tem) != 0)
		{
			AfxMessageBox(L"�������޷�����friends����");
			return TRUE;
		}
		//�����Լ����û�id
		CINIManager *pINI = CINIManager::GetINIManager();
		char *userid = pINI->GetcharUserName();
		send(m_Sockfd,userid,strlen(userid),0);
		delete[] userid;
		//�ȴ������������ظ�������Ϣ:����id���ǳƣ�״̬������or���ߣ�
		char n = 0;
		if (recv(m_Sockfd,&n,1,0) <= 0)
		{
			return TRUE;
		}
		char *utf8str = new char[n+1];
		ZeroMemory(utf8str,n+1);
		int res = recv(m_Sockfd,utf8str,n,0);
		ASSERT(res == n);
		//���������Ϣ
		WCHAR *wstr = utf8ToWideChar(utf8str);
		WCHAR *token;
		WCHAR seps[] = _T(",");
		
		extern list<CString> friendInfoList;

		token = wcstok(wstr,seps);
		while(token != NULL)
		{
			//���뵽�б���
			int row = m_FriendList.GetItemCount();
			m_FriendList.InsertItem(row,token);//������ͬʱ������0
			friendInfoList.push_back(token);
			//������1
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
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CFriendGroupDlg::OnNMRClickFriendList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���������������
	//��ô���ѡ��״̬���û��ǳƺ�ID
	POSITION pos = m_FriendList.GetFirstSelectedItemPosition();
	int nItem = 0;
	if (pos == NULL)
	{
		MessageBox(L"δѡ��һ������");
		return;
	}
	
	nItem = m_FriendList.GetNextSelectedItem(pos);
	
	TCHAR id[512] = {'\0'};
	m_FriendList.GetItemText(nItem,0,id,512);
	TCHAR name[512] = {'\0'};
	m_FriendList.GetItemText(nItem,1,name,512);
	//���������ʼ��˵�
	static CMailDlg* pMailDlg = NULL;
	if (pMailDlg != NULL)
	{
		delete pMailDlg;
		pMailDlg = NULL;
	}
	pMailDlg = new CMailDlg;
	pMailDlg->Create(IDD_MAIL_DLG);
	pMailDlg->SetAddressee(name,id);//�����ռ�����˭
	pMailDlg->ShowWindow(SW_SHOW);
}
