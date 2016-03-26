#pragma once
#include "LogProc.h"
#include <GdiPlus.h>
using namespace Gdiplus;
// CLogDlg 对话框

class CLogDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLogDlg)

public:
	CLogDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLogDlg();

// 对话框数据
	enum { IDD = IDD_LOG_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
private:
	// 套接字描述符
	SOCKET m_sock;
public:
	afx_msg void OnBnClickedLogButton();
	static void TcharToChar(const TCHAR* tchar, char* _char);
private:
	// 用户登陆过程
	int login(void);
	// 用户账号字符串指针
	char* account;
	// 用户密码字符串指针
	char* password;
protected:
	afx_msg LRESULT OnUdpsockread(WPARAM wParam, LPARAM lParam);
private:
	CLogProc* pLogProc;
public:
	afx_msg void OnClickedRegister();
	afx_msg void OnClickedForgotPassword();
protected:
	afx_msg LRESULT OnLogCancel(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnPaint();
private:
	Graphics* pGraphics;
	ULONG_PTR gdiplusToken;
};
