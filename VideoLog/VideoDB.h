#pragma once
#include "sqlite3.h"
#include "VideoListCtrl.h"

class CVideoDB
{
public:
	CVideoDB(void);
	~CVideoDB(void);

public:
	bool InsertVideoRecord(char *imageURL,char *videoName,char *autor,char *time,char *videoURL,char *explain);//�����¼

	bool SelectVideoDB(CVideoListCtrl*);	//��ѯ���ݼ�¼

	bool SelectLastRecord(CVideoListCtrl*);	//��ѯ���²����һ����¼

	//��ѯ�Ļص��������Ѳ�ѯ���Ľ�����뵽��Ƶ�б��
	friend int HandleSelectResult(void* para,int nColumn,char **clumnValue,char**columnName);

private:
	sqlite3* m_db;

};

