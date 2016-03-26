// MainDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoLog.h"
#include "MainDlg.h"
#include "afxdialogex.h"

#pragma comment (lib,"Gdiplus.lib")

// CMainDlg 对话框

IMPLEMENT_DYNAMIC(CMainDlg, CDialogEx)

CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMainDlg::IDD, pParent)
	, gdiplusToken(0)
	, pGraphics(NULL)
	, pCameraWind(NULL)
	, pUpDownWind(NULL)
	, pHomeDlg(NULL)
	, pFriendDlg(NULL)
	, pVideoListDlg(NULL)
{
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken,&gdiplusStartupInput,NULL);
}

CMainDlg::~CMainDlg()
{
	if (pGraphics != NULL)
	{
		delete pGraphics;
	}

	if (pCameraWind != NULL)
	{
		delete pCameraWind;
	}
	delete pVideoListDlg;
	GdiplusShutdown(gdiplusToken);
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)
	ON_WM_NCHITTEST()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_CAMERA, &CMainDlg::OnCamera)
	ON_WM_NCLBUTTONDOWN()
	ON_WM_CLOSE()
	ON_MESSAGE(WM_UPDOWNDLG, &CMainDlg::OnUpdowndlg)
	ON_MESSAGE(WM_HOME, &CMainDlg::OnHome)
	ON_WM_PAINT()
	ON_WM_PAINT()
	ON_MESSAGE(WM_FRIENDGROUP, &CMainDlg::OnFriendgroup)
	ON_MESSAGE(WM_VIDEO_LIST, &CMainDlg::OnVideoList)
END_MESSAGE_MAP()


// CMainDlg 消息处理程序


LRESULT CMainDlg::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rc;
	GetClientRect(&rc);
	CRect rcTitle(0,0,rc.Width(),100);
	ClientToScreen(&rcTitle);
	return rcTitle.PtInRect(point)?HTCAPTION:CDialog::OnNcHitTest(point);
	//return CDialogEx::OnNcHitTest(point);
}


BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if   (pMsg->message==WM_KEYDOWN && (pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE))   
	{    
		return   TRUE;   
	}   

	return CDialogEx::PreTranslateMessage(pMsg);
}


//BOOL CMainDlg::OnEraseBkgnd(CDC* pDC)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	//MessageBox(L"main窗口收到WM_ERASEBK消息");
///*	if (pGraphics == NULL)
//	{
//		pGraphics = new Graphics(pDC->GetSafeHdc());
//	}
//	ColorMatrix ClrMatrix =
//	{
//		1.0f,0.0f,0.0f,0.0f,0.0f,
//		0.0f,1.0f,0.0f,0.0f,0.0f,
//		0.0f,0.0f,1.0f,0.0f,0.0f,
//		0.0f,0.0f,0.0f,0.9f,0.0f,
//		0.0f,0.0f,0.0f,0.0f,1.0f
//	};
//	ImageAttributes imgAttr;
//	imgAttr.SetColorMatrix(&ClrMatrix,ColorMatrixFlagsDefault,ColorAdjustTypeBitmap);
//	*/
///*	Image bkimage(L"res/bk.jpg");
//	RECT rc;
//	GetClientRect(&rc);
//	pGraphics->DrawImage(&bkimage,0,0,rc.right-rc.left,rc.bottom-rc.top);
//	
//	Pen	pen(Color(130,255,255,255));//argb
//	pGraphics->DrawLine(&pen,0,100,800,100);
//	Image homeimage(L"res/home.png");
//	pGraphics->DrawImage(&homeimage,Rect(18,18,64,64),0,0,128,128,UnitPixel,NULL);
//	Image videoimage(L"res/video.png");
//	pGraphics->DrawImage(&videoimage,Rect(118,18,64,64),0,0,128,128,UnitPixel,NULL);
//	Image cameraimage(L"res/camera.png");
//	pGraphics->DrawImage(&cameraimage,Rect(218,18,64,64),0,0,128,128,UnitPixel,NULL);
//	Image uploadimage(L"res/upload.png");
//	pGraphics->DrawImage(&uploadimage,Rect(318,18,64,64),0,0,128,128,UnitPixel,NULL);
//	Image downloadimage(L"res/download.png");
//	pGraphics->DrawImage(&downloadimage,Rect(418,18,64,64),0,0,128,128,UnitPixel,NULL);
//	return TRUE;*/
//	return CDialogEx::OnEraseBkgnd(pDC);
//}


BOOL CMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	MoveWindow(0,0,800,570,0);
	CenterWindow();
	// TODO:  在此添加额外的初始化
		pHomeDlg = new CHomeDlg;
		pHomeDlg->Create(IDD_HOME_DLOG,this);
		pHomeDlg->MoveWindow(0,100,800,470);
	
		pCameraWind = new CameraDlg;
		pCameraWind->Create(IDD_CAMERA_WIND,this);
		pCameraWind->MoveWindow(0,100,800,470);

		pUpDownWind = new CUpDownDlg;
		pUpDownWind->Create(IDD_UP_DOWN_DLG,this);
		pUpDownWind->MoveWindow(0,100,800,470);

		pFriendDlg = new CFriendGroupDlg;
		pFriendDlg->Create(IDD_FRIEND_GROUP_DLOG,this);
		pFriendDlg->MoveWindow(0,100,800,470);
		extern HWND upDownDlghwnd;
		upDownDlghwnd = pUpDownWind->GetSafeHwnd();

		pVideoListDlg = new CVideoListDlg;
		pVideoListDlg->Create(IDD_VIDEO_LIST_DLG,this);
		pVideoListDlg->MoveWindow(0,100,800,570);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


afx_msg LRESULT CMainDlg::OnCamera(WPARAM wParam, LPARAM lParam)
{
	//开启拍摄界面
	pFriendDlg->ShowWindow(SW_HIDE);
	pHomeDlg->ShowWindow(SW_HIDE);
	pUpDownWind->ShowWindow(SW_HIDE);
	pVideoListDlg->ShowWindow(SW_HIDE);

	pCameraWind->ShowWindow(SW_SHOW);

	return 0;
}


void CMainDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	POINT p(point);
	ScreenToClient(&p);
	if (p.y <= 100 && p.y >= 18)
	{
		int buttonth = p.x/100;
		switch(buttonth)
		{
		case 0:
			PostMessage(WM_HOME);
			break;
		case 1:
			PostMessage(WM_CAMERA);
			break;
		case 2:
			PostMessage(WM_UPDOWNDLG);
			break;
		case 3:
			PostMessage(WM_FRIENDGROUP);
			break;
		case 4:
			PostMessage(WM_VIDEO_LIST);
			break;
		}
	}
	CRect closeRect(775,0,800,25);
	CRect mimimazeRect(750,0,775,25);
	if (closeRect.PtInRect(p))
	{
		SendMessage(WM_CLOSE);
	}
	else if (mimimazeRect.PtInRect(p))
	{
		SendMessage(WM_SYSCOMMAND,SC_MINIMIZE,0);
	}
	CRect linkRect(550,30,760,80);
	if (linkRect.PtInRect(p))
	{
		ShellExecute(NULL,L"open",L"http://121.199.24.119/templates/index.html",NULL,NULL,SW_SHOWNORMAL);
	}
	
	CDialogEx::OnNcLButtonDown(nHitTest, point);
}


void CMainDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (NULL != pCameraWind)
	{
		::PostMessage(pCameraWind->GetSafeHwnd(),WM_CLOSE,0,0);
	}
	CDialogEx::OnClose();
}


afx_msg LRESULT CMainDlg::OnUpdowndlg(WPARAM wParam, LPARAM lParam)
{
	//MessageBox(L"updown msg");
	pFriendDlg->ShowWindow(SW_HIDE);
	
	pHomeDlg->ShowWindow(SW_HIDE);
	
	pCameraWind->ShowWindow(SW_HIDE);

	pVideoListDlg->ShowWindow(SW_HIDE);
	
	pUpDownWind->ShowWindow(SW_SHOW);
	return 0;
}


afx_msg LRESULT CMainDlg::OnHome(WPARAM wParam, LPARAM lParam)
{
	pFriendDlg->ShowWindow(SW_HIDE);
	
	pCameraWind->ShowWindow(SW_HIDE);
	
	pUpDownWind->ShowWindow(SW_HIDE);

	pVideoListDlg->ShowWindow(SW_HIDE);
	
	pHomeDlg->ShowWindow(SW_SHOW);
	return 0;
}



void CMainDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	HDC hdc = ::GetDC(m_hWnd);
	if (pGraphics == NULL)
	{
		pGraphics = new Graphics(hdc);
	}
/*	ColorMatrix ClrMatrix =
	{
		1.0f,0.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,0.9f,0.0f,
		0.0f,0.0f,0.0f,0.0f,1.0f
	};
	ImageAttributes imgAttr;
	imgAttr.SetColorMatrix(&ClrMatrix,ColorMatrixFlagsDefault,ColorAdjustTypeBitmap);
	*/
	Image menubarimage(L"res/menubar.jpg");
	pGraphics->DrawImage(&menubarimage,0,0,800,100);
	
	Pen	pen(Color(130,255,255,255));//argb
	pGraphics->DrawLine(&pen,0,99,800,99);
	Image homeimage(L"res/home.png");
	pGraphics->DrawImage(&homeimage,Rect(18,18,64,64),0,0,128,128,UnitPixel,NULL);
	Image videoimage(L"res/video.png");
	pGraphics->DrawImage(&videoimage,Rect(118,18,64,64),0,0,128,128,UnitPixel,NULL);
	/*Image cameraimage(L"res/camera.png");
	pGraphics->DrawImage(&cameraimage,Rect(218,18,64,64),0,0,128,128,UnitPixel,NULL);*/
	Image uploadimage(L"res/upload.png");
	pGraphics->DrawImage(&uploadimage,Rect(218,18,64,64),0,0,128,128,UnitPixel,NULL);
	/*Image downloadimage(L"res/download.png");
	pGraphics->DrawImage(&downloadimage,Rect(418,18,64,64),0,0,128,128,UnitPixel,NULL);*/
	Image friendGroupImage(L"res/friend.png");
	pGraphics->DrawImage(&friendGroupImage,Rect(318,18,64,64),0,0,128,128,UnitPixel,NULL);

	Image videoListImage(L"res/download.png");
	pGraphics->DrawImage(&videoListImage,Rect(418,18,64,64),0,0,128,128,UnitPixel,NULL);
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
}


afx_msg LRESULT CMainDlg::OnFriendgroup(WPARAM wParam, LPARAM lParam)
{
	pHomeDlg->ShowWindow(SW_HIDE);
	pCameraWind->ShowWindow(SW_HIDE);
	pUpDownWind->ShowWindow(SW_HIDE);
	pVideoListDlg->ShowWindow(SW_HIDE);

	pFriendDlg->ShowWindow(SW_SHOW);
	return 0;
}


afx_msg LRESULT CMainDlg::OnVideoList(WPARAM wParam, LPARAM lParam)
{
	pHomeDlg->ShowWindow(SW_HIDE);
	pCameraWind->ShowWindow(SW_HIDE);
	pUpDownWind->ShowWindow(SW_HIDE);
	pFriendDlg->ShowWindow(SW_HIDE);

	pVideoListDlg->ShowWindow(SW_SHOW);
	return 0;
}
