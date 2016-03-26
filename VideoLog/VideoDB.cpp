#include "StdAfx.h"
#include "VideoDB.h"

#include <string>
using namespace std;

CVideoDB::CVideoDB(void)
	: m_db(NULL)
{
	if (sqlite3_open("./res/videoLog.db",&m_db) != SQLITE_OK)
	{
		MessageBox(NULL,L"连接数据库失败!",L"error",MB_OK);
	}
}


CVideoDB::~CVideoDB(void)
{
	sqlite3_close(m_db);
}


bool CVideoDB::InsertVideoRecord(char *imageURL,char *videoName,char *autor,char *time,char *videoURL,char *explain)
{

	string sqlstr;
	sqlstr += "INSERT INTO videos(imageURL,videoName,autor,time,videoURL,explain) VALUES(";

	sqlstr += "'";	sqlstr += imageURL;	sqlstr += "',";
	sqlstr += "'";	sqlstr += videoName;sqlstr += "',";
	sqlstr += "'";	sqlstr += autor;	sqlstr += "',";
	sqlstr += "datetime('now','localtime'),";
	sqlstr += "'";	sqlstr += videoURL;	sqlstr += "',";
	sqlstr += "'";	sqlstr += explain;	sqlstr += "')";
	char *msg = 0;
	if (sqlite3_exec(m_db,sqlstr.c_str(),NULL,NULL,&msg))
	{
		AfxMessageBox(L"数据库插入错误！");
		sqlite3_free(msg);
		return false;
	}
	return true;
}


extern void CharToTchar (const char * _char, TCHAR * tchar);

int HandleSelectResult(void* para,int nColumn,char **clumnValue,char**columnName)
{
	CVideoListCtrl *pListCtrl = (CVideoListCtrl*)para;
	TCHAR imageURL[255];
	CharToTchar(clumnValue[0],imageURL);
	pListCtrl->InsertImage(imageURL);

	TCHAR videoName[50];
	CharToTchar(clumnValue[1],videoName);
	TCHAR autor[50];
	CharToTchar(clumnValue[2],autor);
	TCHAR timestr[20];
	CharToTchar(clumnValue[3],timestr);
	TCHAR videoURL[255];
	CharToTchar(clumnValue[4],videoURL);
	pListCtrl->SetOneRow(videoName,autor,timestr,videoURL);
	return 0;
}


bool CVideoDB::SelectVideoDB(CVideoListCtrl*videoListCtrl)
{
	char *errmsg;
	int result = sqlite3_exec(m_db,"select imageURL,videoName,autor,time,videoURL,explain from videos",HandleSelectResult,(void*)videoListCtrl,&errmsg);
	if (result != SQLITE_OK)
	{
		AfxMessageBox(L"数据库查询错误!");
		sqlite3_free(errmsg);
	}
	return true;
}


bool CVideoDB::SelectLastRecord(CVideoListCtrl*videoListCtrl)
{
	char *errmsg;
	int result = sqlite3_exec(m_db,"select imageURL,videoName,autor,time,videoURL,explain from videos\
		where id=(select max(id) from videos)",HandleSelectResult,(void*)videoListCtrl,&errmsg);
	if (result != SQLITE_OK)
	{
		AfxMessageBox(L"数据库查询错误!");
		sqlite3_free(errmsg);
	}
	return true;
}
