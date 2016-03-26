#pragma once
#include "sqlite3.h"
#include "VideoListCtrl.h"

class CVideoDB
{
public:
	CVideoDB(void);
	~CVideoDB(void);

public:
	bool InsertVideoRecord(char *imageURL,char *videoName,char *autor,char *time,char *videoURL,char *explain);//插入记录

	bool SelectVideoDB(CVideoListCtrl*);	//查询数据记录

	bool SelectLastRecord(CVideoListCtrl*);	//查询最新插入的一条记录

	//查询的回调函数，把查询到的结果插入到视频列表框
	friend int HandleSelectResult(void* para,int nColumn,char **clumnValue,char**columnName);

private:
	sqlite3* m_db;

};

