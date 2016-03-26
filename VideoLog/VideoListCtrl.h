#pragma once

// CVideoListCtrl
/*
设置播放图标
一次插入一行；
单击图标播放；
缩略图放第一列，图标放最后一列
视频（图片）-视频名称-作者-上传时间-播放图标-视频路径 6
以后可能会加入其它图标（比如删除，状态等），所以用单独的CImageList来保存图标，而不是放在缩略图中

实现细节：
CListCtrl单击事件，获取具体单击的是哪一行，哪一列
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
	CImageList	m_ImageList;	//缩略图列表
	CImageList  m_IconList;		//播放图标列表

	int m_nImageWidth;	//视频缩略图的宽度
	int m_nImageHeight;	//视频缩略图的高度
	
	int m_nIconWidth;		//播放图标的宽度
	int m_nIconHeight;	//播放图标的高度

	int m_nColumes;		//列数

public:
	void SetImageSize(int nImageWidth, int nImageHeight);	//设置缩略图的大小
	
	void SetPlayIcon(PCTSTR iconPath);		//根据icon图片路径加入播放图标

	void InsertColumns(LPCTSTR video,LPCTSTR name,LPCTSTR autor,LPCTSTR time,LPCTSTR play,LPCTSTR url);

	void InsertImage(LPCTSTR imagePath);													//插入一行的图片，指明图标
	void SetOneRow(LPWSTR name,LPWSTR autor,LPWSTR time,LPWSTR url);						//一次设置一行
	void InsertOneRow(LPCTSTR imagePath,LPWSTR name,LPWSTR autor,LPWSTR time,LPWSTR url);	//插入一行


	afx_msg void OnNMHover(NMHDR *pNMHDR, LRESULT *pResult);
};

