#pragma once
#include "VideoPlayer.h"
#include "afxcmn.h"
#include "VideoListCtrl.h"
#include "VideoDB.h"
#include "MailDlg.h"
// CVideoListDlg 对话框

class CVideoListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVideoListDlg)

public:
	CVideoListDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVideoListDlg();

// 对话框数据
	enum { IDD = IDD_VIDEO_LIST_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CVideoPlayerDlg* pPlayer;
	CVideoDB	m_VideoDB;
public:
	virtual BOOL OnInitDialog();
public:
//	afx_msg void OnNMDblclkVideoList(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnNMRClickVideoList(NMHDR *pNMHDR, LRESULT *pResult);
private:
	CVideoListCtrl m_VideoListCtrl;
	CMailDlg*	m_pMailDlg;
public:
	afx_msg void OnNMDblclkVideoList(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	afx_msg LRESULT OnSelectLastRecord(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnNMRClickVideoList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPlay();
	afx_msg void OnSendMail();
};
