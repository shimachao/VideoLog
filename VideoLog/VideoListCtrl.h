#pragma once

// CVideoListCtrl
/*
���ò���ͼ��
һ�β���һ�У�
����ͼ�겥�ţ�
����ͼ�ŵ�һ�У�ͼ������һ��
��Ƶ��ͼƬ��-��Ƶ����-����-�ϴ�ʱ��-����ͼ��-��Ƶ·�� 6
�Ժ���ܻ��������ͼ�꣨����ɾ����״̬�ȣ��������õ�����CImageList������ͼ�꣬�����Ƿ�������ͼ��

ʵ��ϸ�ڣ�
CListCtrl�����¼�����ȡ���嵥��������һ�У���һ��
*/

class CVideoListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CVideoListCtrl)

public:
	CVideoListCtrl();
	virtual ~CVideoListCtrl();

protected:
	DECLARE_MESSAGE_MAP()

private:
	CImageList	m_ImageList;	//����ͼ�б�
	CImageList  m_IconList;		//����ͼ���б�

	int m_nImageWidth;	//��Ƶ����ͼ�Ŀ��
	int m_nImageHeight;	//��Ƶ����ͼ�ĸ߶�
	
	int m_nIconWidth;		//����ͼ��Ŀ��
	int m_nIconHeight;	//����ͼ��ĸ߶�

	int m_nColumes;		//����

public:
	void SetImageSize(int nImageWidth, int nImageHeight);	//��������ͼ�Ĵ�С
	
	void SetPlayIcon(PCTSTR iconPath);		//����iconͼƬ·�����벥��ͼ��

	void InsertColumns(LPCTSTR video,LPCTSTR name,LPCTSTR autor,LPCTSTR time,LPCTSTR play,LPCTSTR url);

	void InsertImage(LPCTSTR imagePath);													//����һ�е�ͼƬ��ָ��ͼ��
	void SetOneRow(LPWSTR name,LPWSTR autor,LPWSTR time,LPWSTR url);						//һ������һ��
	void InsertOneRow(LPCTSTR imagePath,LPWSTR name,LPWSTR autor,LPWSTR time,LPWSTR url);	//����һ��


	afx_msg void OnNMHover(NMHDR *pNMHDR, LRESULT *pResult);
};

