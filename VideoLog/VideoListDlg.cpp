// VideoListDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VideoLog.h"
#include "VideoListDlg.h"
#include "afxdialogex.h"
#include <list>
using std::list;


// CVideoListDlg �Ի���

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


// CVideoListDlg ��Ϣ�������

HWND videoListDlgHwnd;

BOOL CVideoListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	videoListDlgHwnd = m_hWnd;
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	MoveWindow(0,100,800,470);
	pPlayer = new CVideoPlayerDlg;
	pPlayer->Create(IDD_PLAYER_DLG,NULL);
	//������Ƶ�б�����Ƶ�����е�λ��
	RECT rect;
	GetClientRect(&rect);
	m_VideoListCtrl.MoveWindow(0,0,rect.right-rect.left,rect.bottom-rect.top);
	//������Ƶ�б�ķ��
	DWORD dwStyle = m_VideoListCtrl.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_SUBITEMIMAGES;
	dwStyle |= LVS_EX_TRACKSELECT;
	m_VideoListCtrl.SetExtendedStyle(dwStyle);
	//ʹ����Ƶ�б�ǰ����Ҫָ��ͼƬ��С�Ͳ���ͼ��
	m_VideoListCtrl.SetImageSize(160,120);
	m_VideoListCtrl.SetPlayIcon(L"res/play.jpg");
	//��ָ������Щ��
	m_VideoListCtrl.InsertColumns(L"��Ƶ",L"��Ƶ����",L"����",L"�ϴ�ʱ��",L"����",L"URL");
	//�������ݱ������ݿ������е�����
	m_VideoDB.SelectVideoDB(&m_VideoListCtrl);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}



void CVideoListDlg::OnNMDblclkVideoList(NMHDR *pNMHDR, LRESULT *pResult)
{   //�û��ڿؼ���˫����������
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//���ѡ���е�λ��
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
	// �Ҽ������˵�����
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
	// TODO: �ڴ���������������
	//���ѡ���е�λ��
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
	// TODO: �ڴ���������������
	//���ѡ���е�λ��
	POSITION pos = m_VideoListCtrl.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		MessageBox(L"δѡ����Ƶ");
		return;
	}
	int nItem = m_VideoListCtrl.GetNextSelectedItem(pos);
	TCHAR path[255] = {'\0'};
	m_VideoListCtrl.GetItemText(nItem,5,path,255);
	if (TRUE != PathFileExists(path))
	{
		MessageBox(L"��Ƶ�ļ������ڣ��Ƿ��Ѵ�Ӳ��ɾ��");
		return;
	}
	//���ͼƬ·��
	CString strImagePath(path);
	strImagePath.Replace(L"videos",L"images");
	strImagePath.Replace(L"avi",L"jpg");
	//���������ʼ�����
	if (m_pMailDlg != NULL)
	{
		delete m_pMailDlg;
		m_pMailDlg = NULL;
	}
	m_pMailDlg = new CMailDlg;
	m_pMailDlg->Create(IDD_MAIL_DLG);
	m_pMailDlg->ShowWindow(SW_SHOW);
	//����Ԥ��ͼƬ
	TCHAR imagePath[512] = {'\0'};
	_stprintf(imagePath,strImagePath);
	m_pMailDlg->SetPreviewImage(imagePath);
	//���ý��պ����б�
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

