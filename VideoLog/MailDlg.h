#pragma once
#include "afxwin.h"
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")


// CMailDlg �Ի���

class CMailDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMailDlg)

public:
	CMailDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMailDlg();

// �Ի�������
	enum { IDD = IDD_MAIL_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	void AddFriendAddrs(LPCTSTR name,LPCTSTR id);	//��Ӻ���������ID��FREIEND ComBox
	void SetAddressee(LPCTSTR name,LPCTSTR id);		//�����ռ���
	void SetPreviewImage(WCHAR*imagePath);			//����Ԥ��ͼƬ
	afx_msg void OnPaint();
	afx_msg void OnBnClickedSend();
};
