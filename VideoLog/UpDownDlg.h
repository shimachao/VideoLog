#pragma once
#include <map>

// CUpDownDlg �Ի���

class CUpDownDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUpDownDlg)

public:
	CUpDownDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUpDownDlg();

// �Ի�������
	enum { IDD = IDD_UP_DOWN_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
