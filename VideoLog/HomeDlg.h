#pragma once

#include <GdiPlus.h>
using namespace Gdiplus;
// CHomeDlg 对话框

class CHomeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHomeDlg)

public:
	CHomeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHomeDlg();

// 对话框数据
	enum { IDD = IDD_HOME_DLOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
private:
	Graphics* pGraphics;
	ULONG_PTR gdiplusToken;
public:
//	afx_msg void OnPaint();
	afx_msg void OnPaint();
};
