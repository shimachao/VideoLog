// VideoPlayer.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoLog.h"
#include "VideoPlayer.h"
#include "afxdialogex.h"


// CVideoPlayerDlg 对话框

IMPLEMENT_DYNAMIC(CVideoPlayerDlg, CDialogEx)

CVideoPlayerDlg::CVideoPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVideoPlayerDlg::IDD, pParent)
{
	
}

CVideoPlayerDlg::~CVideoPlayerDlg()
{
}

void CVideoPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OCXMEDIAPLAYER, m_Player);
}


BEGIN_MESSAGE_MAP(CVideoPlayerDlg, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CVideoPlayerDlg 消息处理程序


void CVideoPlayerDlg::PutURL(LPCTSTR filePath)
{
	ShowWindow(SW_SHOW);
	m_Player.put_URL(filePath);
}


BOOL CVideoPlayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_Controls = m_Player.get_controls();
	RECT rect;
	GetClientRect(&rect);
	m_Player.MoveWindow(&rect);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CVideoPlayerDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_Controls.stop();
	CDialogEx::OnClose();
}
