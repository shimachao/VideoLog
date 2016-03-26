#pragma once
#include "afxcmn.h"
#include <stdio.h>
#include <string.h>
#include <WinSock2.h>

// CFriendGroupDlg 对话框

class CFriendGroupDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFriendGroupDlg)

public:
	CFriendGroupDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFriendGroupDlg();

// 对话框数据
	enum { IDD = IDD_FRIEND_GROUP_DLOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CListCtrl m_FriendList;
	SOCKET m_Sockfd;
public:
	afx_msg void OnNMRClickFriendList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSendVideoMail();
};
