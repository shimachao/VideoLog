#pragma once
#include "afxwin.h"
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")


// CMailDlg 对话框

class CMailDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMailDlg)

public:
	CMailDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMailDlg();

// 对话框数据
	enum { IDD = IDD_MAIL_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CComboBox	m_FriendComBox;
	ULONG_PTR   gdiplusToken;
	Graphics*	m_pGraphics;
	TCHAR		m_ImagePath[512];
	Image*		m_pPreviewImage;
	TCHAR*		m_pVideoFileName;
public:
	virtual BOOL OnInitDialog();
	void AddFriendAddrs(LPCTSTR name,LPCTSTR id);	//添加好友姓名和ID到FREIEND ComBox
	void SetAddressee(LPCTSTR name,LPCTSTR id);		//设置收件人
	void SetPreviewImage(WCHAR*imagePath);			//设置预览图片
	afx_msg void OnPaint();
	afx_msg void OnBnClickedSend();
};
