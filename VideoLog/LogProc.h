#pragma once
#include "afxwin.h"
#include "PictureEx.h"
#include <GdiPlus.h>
using namespace Gdiplus;
// CLogProc 对话框

class CLogProc : public CDialogEx
{
	DECLARE_DYNAMIC(CLogProc)

public:
	CLogProc(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLogProc();

// 对话框数据
	enum { IDD = IDD_LOGPROC_DLOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CPictureEx m_GifControl;
public:
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnBnClickedCancle();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void Stop(void);
	afx_msg void OnPaint();
private:
	Graphics* pGraphics;
	ULONG_PTR gdiplusToken;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
