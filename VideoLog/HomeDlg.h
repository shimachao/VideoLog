#pragma once

#include <GdiPlus.h>
using namespace Gdiplus;
// CHomeDlg �Ի���

class CHomeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHomeDlg)

public:
	CHomeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHomeDlg();

// �Ի�������
	enum { IDD = IDD_HOME_DLOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
