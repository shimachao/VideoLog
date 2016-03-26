#pragma once
#include <GdiPlus.h>
#include "CameraDlg.h"
#include "UpDownDlg.h"
#include "HomeDlg.h"
#include "FriendGroupDlg.h"
#include "VideoListDlg.h"
using namespace Gdiplus;
// CMainDlg 对话框

class CMainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMainDlg();

// 对话框数据
	enum { IDD = IDD_MAIN_DLOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnNcHitTest(CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
private:
	ULONG_PTR gdiplusToken;
	Graphics* pGraphics;
public:
//	afx_msg void OnPaint();
//	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
protected:
	afx_msg LRESULT OnCamera(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
private:
	CameraDlg* pCameraWind;
public:
	afx_msg void OnClose();
protected:
	afx_msg LRESULT OnUpdowndlg(WPARAM wParam, LPARAM lParam);
private:
	CUpDownDlg* pUpDownWind;
protected:
	afx_msg LRESULT OnHome(WPARAM wParam, LPARAM lParam);
private:
	CHomeDlg* pHomeDlg;
public:
//	afx_msg void OnPaint();
	afx_msg void OnPaint();
private:
	CFriendGroupDlg* pFriendDlg;
protected:
	afx_msg LRESULT OnFriendgroup(WPARAM wParam, LPARAM lParam);
private:
	CVideoListDlg* pVideoListDlg;
protected:
	afx_msg LRESULT OnVideoList(WPARAM wParam, LPARAM lParam);
};
