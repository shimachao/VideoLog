// LogProc.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VideoLog.h"
#include "LogProc.h"
#include "afxdialogex.h"


// CLogProc �Ի���

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


// CLogProc ��Ϣ�������


BOOL CLogProc::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	GetDlgItem(IDC_ANIMATION)->MoveWindow(0,0,60,60);
	GetDlgItem(IDC_ANIMATION)->CenterWindow();
	if (m_GifControl.Load(L"./res/logproc.gif") != FALSE)
	{
		m_GifControl.Draw();
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


LRESULT CLogProc::OnNcHitTest(CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect rc;
	GetClientRect(&rc);
	CRect rcTitle(0,0,rc.Width(),rc.Height()/3);
	ClientToScreen(&rcTitle);
	return rcTitle.PtInRect(point)?HTCAPTION:CDialog::OnNcHitTest(point);
	//return CDialogEx::OnNcHitTest(point);
}


void CLogProc::OnBnClickedCancle()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	::SendMessage(GetParent()->GetSafeHwnd(),WM_LOG_CANCEL,0,0);
	OnCancel();
}


BOOL CLogProc::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	//���ε�Esc����Enter����Ϣ
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
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
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

	// TODO:  �ڴ˸��� DC ���κ�����
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
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
