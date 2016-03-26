// HomeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VideoLog.h"
#include "HomeDlg.h"
#include "afxdialogex.h"


// CHomeDlg �Ի���

IMPLEMENT_DYNAMIC(CHomeDlg, CDialogEx)

CHomeDlg::CHomeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHomeDlg::IDD, pParent)
	, pGraphics(NULL)
	, gdiplusToken(0)
{
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken,&gdiplusStartupInput,NULL);
}

CHomeDlg::~CHomeDlg()
{
	if (pGraphics != NULL)
	{
		delete pGraphics;
	}
	GdiplusShutdown(gdiplusToken);
}

void CHomeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHomeDlg, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CHomeDlg ��Ϣ�������

void CHomeDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	HDC hdc = ::GetDC(m_hWnd);
	if (pGraphics == NULL)
	{
		pGraphics = new Graphics(hdc);
	}
	Image dlgBkimage(L"res/dlg2.jpg");
	pGraphics->DrawImage(&dlgBkimage,Rect(0,0,800,470),0,0,800,470,UnitPixel,NULL);
	Image videoinage(L"res/video.png");
	Image uploadimage(L"res/upload.png");
	Image friendimage(L"res/friend.png");
	pGraphics->DrawImage(&videoinage,Rect(100,100,128,128),0,0,128,128,UnitPixel,NULL);
	pGraphics->DrawImage(&uploadimage,Rect(300,100,128,128),0,0,128,128,UnitPixel,NULL);
	pGraphics->DrawImage(&friendimage,Rect(500,100,128,128),0,0,128,128,UnitPixel,NULL);
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}
