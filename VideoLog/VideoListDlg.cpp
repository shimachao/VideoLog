// VideoListDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoLog.h"
#include "VideoListDlg.h"
#include "afxdialogex.h"
#include <list>
using std::list;


// CVideoListDlg 对话框

IMPLEMENT_DYNAMIC(CVideoListDlg, CDialogEx)

CVideoListDlg::CVideoListDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVideoListDlg::IDD, pParent)
	, pPlayer(NULL),m_pMailDlg(NULL)
{

}

CVideoListDlg::~CVideoListDlg()
{
}

void CVideoListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_VIDEO_LIST, m_VideoList);
	DDX_Control(pDX, IDC_VIDEO_LIST, m_VideoListCtrl);
}


BEGIN_MESSAGE_MAP(CVideoListDlg, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_VIDEO_LIST, &CVideoListDlg::OnNMDblclkVideoList)
	ON_MESSAGE(WM_SELECT_LAST_RECORD, &CVideoListDlg::OnSelectLastRecord)
	ON_NOTIFY(NM_RCLICK, IDC_VIDEO_LIST, &CVideoListDlg::OnNMRClickVideoList)
	ON_COMMAND(ID_PLAY, &CVideoListDlg::OnPlay)
	ON_COMMAND(ID_SEND_MAIL, &CVideoListDlg::OnSendMail)
END_MESSAGE_MAP()


// CVideoListDlg 消息处理程序

HWND videoListDlgHwnd;

BOOL CVideoListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	videoListDlgHwnd = m_hWnd;
	// TODO:  在此添加额外的初始化
	MoveWindow(0,100,800,470);
	pPlayer = new CVideoPlayerDlg;
	pPlayer->Create(IDD_PLAYER_DLG,NULL);
	//设置视频列表在视频窗口中的位置
	RECT rect;
	GetClientRect(&rect);
	m_VideoListCtrl.MoveWindow(0,0,rect.right-rect.left,rect.bottom-rect.top);
	//设置视频列表的风格
	DWORD dwStyle = m_VideoListCtrl.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_SUBITEMIMAGES;
	dwStyle |= LVS_EX_TRACKSELECT;
	m_VideoListCtrl.SetExtendedStyle(dwStyle);
	//使用视频列表前，先要指明图片大小和播放图标
	m_VideoListCtrl.SetImageSize(160,120);
	m_VideoListCtrl.SetPlayIcon(L"res/play.jpg");
	//先指明有哪些列
	m_VideoListCtrl.InsertColumns(L"视频",L"视频名称",L"作者",L"上传时间",L"播放",L"URL");
	//插入数据本地数据库中已有的数据
	m_VideoDB.SelectVideoDB(&m_VideoListCtrl);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}



void CVideoListDlg::OnNMDblclkVideoList(NMHDR *pNMHDR, LRESULT *pResult)
{   //用户在控件中双击了鼠标左键
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//获得选中行的位置
	POSITION pos = m_VideoListCtrl.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		return;
	}
	int nItem = m_VideoListCtrl.GetNextSelectedItem(pos);
	TCHAR path[255] = {'\0'};
	m_VideoListCtrl.GetItemText(nItem,5,path,255);
	if (TRUE == PathFileExists(path))
	{
		pPlayer->PutURL(path);
	}
	*pResult = 0;
}



afx_msg LRESULT CVideoListDlg::OnSelectLastRecord(WPARAM wParam, LPARAM lParam)
{
	m_VideoDB.SelectLastRecord(&m_VideoListCtrl);
	return 0;
}


void CVideoListDlg::OnNMRClickVideoList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// 右键弹出菜单代码
	CMenu menu;
	menu.LoadMenu(IDR_VIDEO_LIST_MENU);
	CMenu *pSubMenu = menu.GetSubMenu(0);
	CPoint point;
	GetCursorPos(&point);
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);
	menu.DestroyMenu();

	*pResult = 0;
}


void CVideoListDlg::OnPlay()
{
	// TODO: 在此添加命令处理程序代码
	//获得选中行的位置
	POSITION pos = m_VideoListCtrl.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		return;
	}
	int nItem = m_VideoListCtrl.GetNextSelectedItem(pos);
	TCHAR path[255] = {'\0'};
	m_VideoListCtrl.GetItemText(nItem,5,path,255);
	if (TRUE == PathFileExists(path))
	{
		pPlayer->PutURL(path);
	}
}


void CVideoListDlg::OnSendMail()
{
	// TODO: 在此添加命令处理程序代码
	//获得选中行的位置
	POSITION pos = m_VideoListCtrl.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		MessageBox(L"未选中视频");
		return;
	}
	int nItem = m_VideoListCtrl.GetNextSelectedItem(pos);
	TCHAR path[255] = {'\0'};
	m_VideoListCtrl.GetItemText(nItem,5,path,255);
	if (TRUE != PathFileExists(path))
	{
		MessageBox(L"视频文件不存在，是否已从硬盘删除");
		return;
	}
	//获得图片路径
	CString strImagePath(path);
	strImagePath.Replace(L"videos",L"images");
	strImagePath.Replace(L"avi",L"jpg");
	//弹出发送邮件界面
	if (m_pMailDlg != NULL)
	{
		delete m_pMailDlg;
		m_pMailDlg = NULL;
	}
	m_pMailDlg = new CMailDlg;
	m_pMailDlg->Create(IDD_MAIL_DLG);
	m_pMailDlg->ShowWindow(SW_SHOW);
	//设置预览图片
	TCHAR imagePath[512] = {'\0'};
	_stprintf(imagePath,strImagePath);
	m_pMailDlg->SetPreviewImage(imagePath);
	//设置接收好友列表
	extern list<CString> friendInfoList;
	list<CString>::iterator id;
	list<CString>::iterator name;
	for (list<CString>::iterator it = friendInfoList.begin(); it != friendInfoList.end(); it++)
	{
		id = it;
		name = ++it;
		m_pMailDlg->AddFriendAddrs(*name,*id);
	}
}

