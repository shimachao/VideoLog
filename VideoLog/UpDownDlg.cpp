// UpDownDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoLog.h"
#include "UpDownDlg.h"
#include "afxdialogex.h"
#include "UpDownProThreadFunc.h"
#include <utility>
#include <stdio.h>

// CUpDownDlg 对话框

IMPLEMENT_DYNAMIC(CUpDownDlg, CDialogEx)

CUpDownDlg::CUpDownDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUpDownDlg::IDD, pParent)
{
	m_VideoProssCtrl = CRect(10,10,200,30);
	m_ImageProssCtrl = CRect(250,10,100,30);
}

CUpDownDlg::~CUpDownDlg()
{
}

void CUpDownDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUpDownDlg, CDialogEx)
	ON_MESSAGE(WM_UP, &CUpDownDlg::OnUp)
	ON_MESSAGE(WM_UPOVER, &CUpDownDlg::OnUpover)
END_MESSAGE_MAP()



void CUpDownDlg::MoveProssCtrlRect(void)
{
	m_VideoProssCtrl.MoveToY((m_VideoProssCtrl.top+60)%500);
	m_ImageProssCtrl.MoveToY((m_ImageProssCtrl.top+60)%500);
}
// CUpDownDlg 消息处理程序

afx_msg LRESULT CUpDownDlg::OnUp(WPARAM wParam, LPARAM lParam)
{
	//上传相应的视频和图片文件
	TCHAR* filename[2];
	filename[0] = GetVideoFullFileName();
	filename[1] = GetImageFullFileName();
	for (int i =0; i < 2; i++)
	{
		CProgressCtrl *upPross = new CProgressCtrl;
		if (0 == i)
		{
			upPross->Create(WS_CHILD|WS_VISIBLE|PBS_SMOOTH,m_VideoProssCtrl,this,134+i+m_VideoProssCtrl.left);
		}
		else if (1 == i)
		{
			upPross->Create(WS_CHILD|WS_VISIBLE|PBS_SMOOTH,m_ImageProssCtrl,this,134+i+m_ImageProssCtrl.left);
		}
		
		
		//开启上传进程
		PAR_TO_PROTHREAD *par = new PAR_TO_PROTHREAD;
		ZeroMemory(par,sizeof(PAR_TO_PROTHREAD));
		par->pPro = upPross;
		par->filename = filename[i];
		par->hWnd = this->GetSafeHwnd();

		m_hWndMap.insert(std::make_pair(upPross->GetSafeHwnd(),upPross));
		CreateThread(NULL,0,UpDownProThreadFunc,(LPVOID)par,0,NULL);
	}
	//移动下一次上传时，进度条的位置
	MoveProssCtrlRect();	
	return 0;
}


afx_msg LRESULT CUpDownDlg::OnUpover(WPARAM wParam, LPARAM lParam)
{
	//::MessageBox(NULL,L"WM_UPOVER",L"msg",MB_OK);
	HWND hWnd = (HWND)wParam;
	std::map<HWND,CProgressCtrl*>::iterator it = m_hWndMap.find(hWnd);
	if (it != m_hWndMap.end())
	{
		//::MessageBox(NULL,L"delete pPro",L"msg",MB_OK);
		CProgressCtrl*pPro = it->second;
		pPro->DestroyWindow();
		delete pPro;
		m_hWndMap.erase(it);
	}
	return 0;
}
