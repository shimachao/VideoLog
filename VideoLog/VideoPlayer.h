#pragma once

#include "CWMPPlayer4.h"
#include "CWMPControls3.h"
// CVideoPlayerDlg �Ի���

class CVideoPlayerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVideoPlayerDlg)

public:
	CVideoPlayerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CVideoPlayerDlg();

// �Ի�������
	enum { IDD = IDD_PLAYER_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	CWMPControls3	m_Controls;
	CWMPPlayer4 m_Player;
public:
	void PutURL(LPCTSTR filePath);
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
};
