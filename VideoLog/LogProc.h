#pragma once
#include "afxwin.h"
#include "PictureEx.h"
#include <GdiPlus.h>
using namespace Gdiplus;
// CLogProc �Ի���

class CLogProc : public CDialogEx
{
	DECLARE_DYNAMIC(CLogProc)

public:
	CLogProc(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLogProc();

// �Ի�������
	enum { IDD = IDD_LOGPROC_DLOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
