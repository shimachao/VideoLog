// ImageListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoListCtrl.h"


// CVideoListCtrl

IMPLEMENT_DYNAMIC(CVideoListCtrl, CListCtrl)

CVideoListCtrl::CVideoListCtrl()
{
	m_nColumes = 5;
}

CVideoListCtrl::~CVideoListCtrl()
{
}


BEGIN_MESSAGE_MAP(CVideoListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(NM_HOVER, &CVideoListCtrl::OnNMHover)
END_MESSAGE_MAP()



// CVideoListCtrl 消息处理程序




void CVideoListCtrl::SetImageSize(int nImageWidth, int nImageHeight)
{
	m_nImageWidth = nImageWidth;
	m_nImageHeight = nImageHeight;
	m_ImageList.Create(m_nImageWidth,m_nImageHeight,ILC_COLOR24,0,5);
	SetImageList(&m_ImageList,LVSIL_SMALL);

	m_nIconWidth = m_nIconHeight = m_nImageHeight;
}



void CVideoListCtrl::SetPlayIcon(PCTSTR iconPath)
{
	CImage img;
	img.Load(iconPath);
	HDC hDC = img.GetDC();
	CDC *pDC = CDC::FromHandle(hDC);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC,m_nImageWidth,m_nImageHeight);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap *pOld = memDC.SelectObject(&bmp);
	//把目标DC中的位图DC变成白色
	CBrush brush;
	brush.CreateStockObject(WHITE_BRUSH);//创建一个白色的笔刷
	memDC.FillRect(CRect(0,0,m_nImageWidth,m_nImageHeight),&brush);

	SetStretchBltMode(memDC.m_hDC,COLORONCOLOR);
	img.StretchBlt(memDC.m_hDC,CRect(0,0,m_nImageHeight,m_nImageHeight),CRect(0,0,img.GetWidth(),img.GetHeight()),SRCCOPY);
	memDC.SelectObject(pOld->m_hObject);
	img.ReleaseDC();
	int m = m_ImageList.GetImageCount();
	m_ImageList.Add(&bmp,RGB(0,0,0));
	m = m_ImageList.GetImageCount();
}


void CVideoListCtrl::InsertColumns(LPCTSTR video,LPCTSTR name,LPCTSTR autor,LPCTSTR time,LPCTSTR play,LPCTSTR url)
{
	InsertColumn(0,video,LVCFMT_CENTER,m_nImageWidth);
	InsertColumn(1,name,LVCFMT_CENTER,100);
	InsertColumn(2,autor,LVCFMT_CENTER,100);
	InsertColumn(3,time,LVCFMT_CENTER,110);
	InsertColumn(4,play,LVCFMT_CENTER,m_nImageHeight);
	InsertColumn(5,url,LVCFMT_LEFT,100);
}

void CVideoListCtrl::InsertImage(LPCTSTR imagePath)
{
	//插入缩略图
	CImage img;
	HRESULT hr = img.Load(imagePath);
	if (FAILED(hr))
	{
		img.Load(L"res/white.bmp");
	}

	HDC hDC = img.GetDC();
	CDC *pDC = CDC::FromHandle(hDC);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC,m_nImageWidth,m_nImageHeight);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap *pOld = memDC.SelectObject(&bmp);
	SetStretchBltMode(memDC.m_hDC,STRETCH_HALFTONE);
	img.StretchBlt(memDC.m_hDC,CRect(0,0,m_nImageWidth,m_nImageHeight),CRect(0,0,img.GetWidth(),img.GetHeight()),SRCCOPY);
	memDC.SelectObject(pOld->m_hObject);
	img.ReleaseDC();
	m_ImageList.Add(&bmp,RGB(0,0,0));

	int m = m_ImageList.GetImageCount();
	//插入图片
	LV_ITEM lvitem;
	ZeroMemory(&lvitem,sizeof(LV_ITEM));
	lvitem.mask = LVIF_IMAGE;
	lvitem.iItem = GetItemCount();
	lvitem.iSubItem = 0;
	lvitem.stateMask = 0;
	lvitem.iImage = GetItemCount()+1;//第0个图片为播放图标，所以从1开始
	//lvitem.pszText = L"";
	InsertItem(&lvitem);
	//插入播放图标
	lvitem.iSubItem = m_nColumes-1;
	lvitem.iImage = 0;

	SetItem(&lvitem);
}



void CVideoListCtrl::SetOneRow(LPWSTR name,LPWSTR autor,LPWSTR time,LPWSTR url)
{
	LV_ITEM lvitem;
	ZeroMemory(&lvitem,sizeof(LV_ITEM));
	lvitem.mask = LVIF_TEXT;
	lvitem.iItem = GetItemCount()-1;
	lvitem.iSubItem = 1;
	lvitem.stateMask = 0;
	lvitem.pszText = name;
	SetItem(&lvitem);

	lvitem.iSubItem = 2;
	lvitem.pszText = autor;
	SetItem(&lvitem);

	lvitem.iSubItem = 3;
	lvitem.pszText = time;
	SetItem(&lvitem);

	lvitem.iSubItem = 5;
	lvitem.pszText = url;
	SetItem(&lvitem);
}




void CVideoListCtrl::InsertOneRow(LPCTSTR imagePath,LPWSTR name,LPWSTR autor,LPWSTR time,LPWSTR url)
{
	InsertImage(imagePath);
	SetOneRow(name,autor,time,url);
}



void CVideoListCtrl::OnNMHover(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 1;
}
