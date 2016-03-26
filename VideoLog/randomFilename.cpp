#include "StdAfx.h"
#include "randomFilename.h"
#include <stdio.h>
#include <string.h>

static TCHAR *videoFileFuillName = NULL;
static TCHAR *imageFileFullName = NULL;
static TCHAR *videoFileName = NULL;
static TCHAR *imageFileName = NULL;
static char	*videocharFileName = NULL;
static char *imagecharFileName = NULL;

//����ʱ����������ļ�������Ƶȫ·������ͼ��ȫ·����
TCHAR *GetRandFileFullName(const TCHAR*prefix,const TCHAR*suffix)
{
	//���ݵ�ǰʱ�������������
	SYSTEMTIME lt;
	GetLocalTime(&lt);
	TCHAR *wbuf = new TCHAR[16+_tcslen(suffix)+_tcslen(prefix)];
	ZeroMemory(wbuf,(16+_tcslen(suffix)+_tcslen(prefix))*sizeof(TCHAR));
	_stprintf(wbuf,L"%s%4d%02d%02d%02d%02d%02d%s",prefix,lt.wYear,lt.wMonth,lt.wDay,lt.wHour,lt.wMinute,lt.wSecond,suffix);
	//�����Ƶ�ļ���
	if (videoFileName != NULL)
	{
		delete[] videoFileName;
		delete[] videocharFileName;
		videoFileName = NULL;
		videocharFileName = NULL;
	}
	videoFileName = new TCHAR[16+_tcslen(suffix)];
	videocharFileName = new char[16+strlen("avi")];
	_stprintf(videoFileName,L"%4d%02d%02d%02d%02d%02d%s",\
		lt.wYear,lt.wMonth,lt.wDay,lt.wHour,lt.wMinute,lt.wSecond,suffix);
	sprintf(videocharFileName,"%4d%02d%02d%02d%02d%02d%s",\
		lt.wYear,lt.wMonth,lt.wDay,lt.wHour,lt.wMinute,lt.wSecond,".avi");
	//�����Ƶ�ļ�ȫ·��+�ļ���
	if (videoFileFuillName != NULL)
	{
		delete[] videoFileFuillName;
	}
	videoFileFuillName = new TCHAR[16+_tcslen(L".\\videos\\")+_tcslen(suffix)];
	_stprintf(videoFileFuillName,L"%s%4d%02d%02d%02d%02d%02d%s",L".\\videos\\",\
		lt.wYear,lt.wMonth,lt.wDay,lt.wHour,lt.wMinute,lt.wSecond,suffix);
	//�������ͼ�ļ���
	if (imageFileName != NULL)
	{
		delete[] imageFileName;
		delete[] imagecharFileName;
		imageFileName = NULL;
		imagecharFileName = NULL;
	}
	imageFileName = new TCHAR[16+_tcslen(L".jpg")];
	imagecharFileName = new char[16+strlen(".jpg")];
	_stprintf(imageFileName,L"%4d%02d%02d%02d%02d%02d%s",\
		lt.wYear,lt.wMonth,lt.wDay,lt.wHour,lt.wMinute,lt.wSecond,L".jpg");
	sprintf(imagecharFileName,"%4d%02d%02d%02d%02d%02d%s",\
		lt.wYear,lt.wMonth,lt.wDay,lt.wHour,lt.wMinute,lt.wSecond,".jpg");
	//���ͼ��ȫ·����
	if (imageFileFullName != NULL)
	{
		delete[] imageFileFullName;
	}
	imageFileFullName = new TCHAR[16+_tcslen(L".\\images\\")+_tcslen(L".jpg")];
	_stprintf(imageFileFullName,L"%s%4d%02d%02d%02d%02d%02d%s",L".\\images\\",\
		lt.wYear,lt.wMonth,lt.wDay,lt.wHour,lt.wMinute,lt.wSecond,L".jpg");
	return wbuf;
}


char *GetVideocharFileName(void)
{
	return videocharFileName;
}


TCHAR *GetVideoFileName(void)
{
	return videoFileName;
}


TCHAR *GetVideoFullFileName(void)
{
	return videoFileFuillName;
}


char *GetImagecharFileName(void)
{
	return imagecharFileName;
}


TCHAR *GetImageFileName(void)
{
	return imageFileName;
}


TCHAR *GetImageFullFileName(void)
{
	return imageFileFullName;
}