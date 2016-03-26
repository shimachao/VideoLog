// VideoPlayer.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VideoLog.h"
#include "VideoPlayer.h"
#include "afxdialogex.h"


// CVideoPlayerDlg �Ի���

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


// CVideoPlayerDlg ��Ϣ�������


void CVideoPlayerDlg::PutURL(LPCTSTR filePath)
{
	ShowWindow(SW_SHOW);
	m_Player.put_URL(filePath);
}


BOOL CVideoPlayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_Controls = m_Player.get_controls();
	RECT rect;
	GetClientRect(&rect);
	m_Player.MoveWindow(&rect);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CVideoPlayerDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_Controls.stop();
	CDialogEx::OnClose();
}
