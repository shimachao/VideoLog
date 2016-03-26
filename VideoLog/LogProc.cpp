// LogProc.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoLog.h"
#include "LogProc.h"
#include "afxdialogex.h"


// CLogProc 对话框

IMPLEMENT_DYNAMIC(CLogProc, CDialogEx)

CLogProc::CLogProc(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLogProc::IDD, pParent)
	, pGraphics(NULL)
	, gdiplusToken(0)
{
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken,&gdiplusStartupInput,NULL);
}

CLogProc::~CLogProc()
{
	if (pGraphics != NULL)
	{
		delete pGraphics;
	}
	GdiplusShutdown(gdiplusToken);
}

void CLogProc::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ANIMATION, m_GifControl);
}


BEGIN_MESSAGE_MAP(CLogProc, CDialogEx)
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_CANCLE, &CLogProc::OnBnClickedCancle)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CLogProc 消息处理程序


BOOL CLogProc::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	GetDlgItem(IDC_ANIMATION)->MoveWindow(0,0,60,60);
	GetDlgItem(IDC_ANIMATION)->CenterWindow();
	if (m_GifControl.Load(L"./res/logproc.gif") != FALSE)
	{
		m_GifControl.Draw();
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


LRESULT CLogProc::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rc;
	GetClientRect(&rc);
	CRect rcTitle(0,0,rc.Width(),rc.Height()/3);
	ClientToScreen(&rcTitle);
	return rcTitle.PtInRect(point)?HTCAPTION:CDialog::OnNcHitTest(point);
	//return CDialogEx::OnNcHitTest(point);
}


void CLogProc::OnBnClickedCancle()
{
	// TODO: 在此添加控件通知处理程序代码
	::SendMessage(GetParent()->GetSafeHwnd(),WM_LOG_CANCEL,0,0);
	OnCancel();
}


BOOL CLogProc::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	//屏蔽掉Esc键和Enter键消息
	if   (pMsg->message==WM_KEYDOWN && (pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE))   
	{    
		return   TRUE;   
	}   

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CLogProc::Stop(void)
{
	::SendMessage(GetParent()->GetSafeHwnd(),WM_LOG_CANCEL,0,0);
	OnCancel();
}


void CLogProc::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	HDC hdc = ::GetDC(m_hWnd);
	if (pGraphics == NULL)
	{
		pGraphics = new Graphics(hdc);
	}
	CRect rect;
	GetClientRect(&rect);
	Image dlgBkimage(L"res/dlg2.jpg");
	pGraphics->DrawImage(&dlgBkimage,Rect(0,0,800,470),0,0,rect.Width(),rect.Height(),UnitPixel,NULL);
}


HBRUSH CLogProc::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	/*if (CTLCOLOR_STATIC == nCtlColor )
	{
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)GetStockObject(NULL_BRUSH); 
	}*/
	if (pWnd->GetDlgCtrlID() == IDC_ANIMATION)
	{
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)GetStockObject(NULL_BRUSH); 
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
