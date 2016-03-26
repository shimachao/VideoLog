#pragma once
#include <map>

// CUpDownDlg 对话框

class CUpDownDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUpDownDlg)

public:
	CUpDownDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUpDownDlg();

// 对话框数据
	enum { IDD = IDD_UP_DOWN_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnUp(WPARAM wParam, LPARAM lParam);
	void MoveProssCtrlRect(void);
private:
	std::map<HWND,CProgressCtrl*> m_hWndMap;
	CRect	m_VideoProssCtrl;
	CRect	m_ImageProssCtrl;
protected:
	afx_msg LRESULT OnUpover(WPARAM wParam, LPARAM lParam);
};
