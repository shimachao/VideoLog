#pragma once

#include "Capture.h"
#include <GdiPlus.h>
#include "CameraDlg.h"
#include "GetScreenShot.h"
#include "randomFilename.h"
#include "VideoDB.h"
#include "afxwin.h"
using namespace Gdiplus;
// CameraDlg 对话框

class CameraDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CameraDlg)

public:
	CameraDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CameraDlg();

// 对话框数据
	enum { IDD = IDD_CAMERA_WIND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	Capture* pCapture;
public:
//	afx_msg void OnBnClickedStartCapture();
	afx_msg void OnClose();
private:
	Graphics* pGraphics;
	ULONG_PTR gdiplusToken;
public:
//	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
//	afx_msg void OnBnClickedStopCapture();
//	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
private:
//	CButton m_StopBtn;
	BOOL m_CameraFlag;
	CVideoDB m_VideoDB;
public:
	afx_msg void OnBnClickedStartBtn();
	afx_msg void OnBnClickedStopBtn();
	afx_msg void OnBnClickedNoteOk();

private:
	//插入一次拍摄记录到本地数据库
	void InsertVideoLogRecord(void);	//将本次的视频记录保存到本地数据库
	bool NotifyServerVideoRecod(void);	//告知服务器一条本用户的一条视频日志记录产生
};
