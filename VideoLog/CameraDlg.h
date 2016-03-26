#pragma once

#include "Capture.h"
#include <GdiPlus.h>
#include "CameraDlg.h"
#include "GetScreenShot.h"
#include "randomFilename.h"
#include "VideoDB.h"
#include "afxwin.h"
using namespace Gdiplus;
// CameraDlg �Ի���

class CameraDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CameraDlg)

public:
	CameraDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CameraDlg();

// �Ի�������
	enum { IDD = IDD_CAMERA_WIND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	Capture* pCapture;
public:
//	afx_msg void OnBnClickedStartCapture();
	afx_msg void OnClose();
private:
	Graphics* pGraphics;
	ULONG_PTR gdiplusToken;
public:
//	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
//	afx_msg void OnBnClickedStopCapture();
//	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
private:
//	CButton m_StopBtn;
	BOOL m_CameraFlag;
	CVideoDB m_VideoDB;
public:
	afx_msg void OnBnClickedStartBtn();
	afx_msg void OnBnClickedStopBtn();
	afx_msg void OnBnClickedNoteOk();

private:
	//����һ�������¼���������ݿ�
	void InsertVideoLogRecord(void);	//�����ε���Ƶ��¼���浽�������ݿ�
	bool NotifyServerVideoRecod(void);	//��֪������һ�����û���һ����Ƶ��־��¼����
};
