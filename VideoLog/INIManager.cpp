#include "stdafx.h"
#include "INIManager.h"
#include <string.h>
#include <WinSock2.h>

CINIManager* CINIManager::m_pINIManager = NULL;


CINIManager::CINIManager(void):m_ImageDir(NULL),m_ServerIP(0),m_ServerLogPort(0),
								m_UserName(NULL),m_VideoDir(NULL)
{
	m_Path = L"res/videolog.ini";
	ReadConfig();
}


CINIManager* CINIManager::GetINIManager()
{
	if (NULL == m_pINIManager)
	{
		m_pINIManager = new CINIManager;
	}

	return m_pINIManager;
}


// 读取平配置文件，只给构造函数调用
void CINIManager::ReadConfig(void)
{
	//读取服务端IP地址
	TCHAR ipWstr[16] = {'\0'};
	::GetPrivateProfileString(L"NetWork",L"ServerIP",L"0.0.0.0",ipWstr,16,m_Path);
	char ipstr[16] = {'\0'};
	TcharToChar(ipWstr,ipstr);
	m_ServerIP = inet_addr(ipstr);//已经是网络字节序了
	//读取服务器登陆端口号
	m_ServerLogPort = ::GetPrivateProfileInt(L"NetWork",L"ServerLogPort",0,m_Path);
	m_ServerLogPort = htons(m_ServerLogPort);//转为网络字节序
	//读取服务器事务处理端口号
	m_ServicesPort = ::GetPrivateProfileInt(L"NetWork",L"ServicesPort",0,m_Path);
	m_ServicesPort = htons(m_ServicesPort);
	//读取视频文件保存目录
	if (m_VideoDir != NULL)
	{
		delete[] m_VideoDir;
	}

	TCHAR videoPath[255] = {'\0'};
	::GetPrivateProfileString(L"Local",L"VideoPath",L"",videoPath,255,m_Path);

	size_t len = _tcslen(videoPath);
	m_VideoDir = new TCHAR[len+1];
	ZeroMemory((void*)m_VideoDir,(len+1)*2);
	_tcscat(m_VideoDir,videoPath);

	//读取视频缩略图保存目录
	if (m_ImageDir != NULL)
	{
		delete[] m_ImageDir;
	}

	TCHAR imagePath[255] = {'\0'};
	::GetPrivateProfileString(L"Local",L"ImagePath",L"",imagePath,255,m_Path);

	len = _tcslen(imagePath);
	m_ImageDir = new TCHAR[len+1];
	ZeroMemory((void*)m_ImageDir,(len+1)*2);
	_tcscat(m_ImageDir,imagePath);

	//读取默认用户名
	if (m_UserName != NULL)
	{
		delete[] m_UserName;
		m_UserName = NULL;
	}

	TCHAR userName[25] = {'\0'};
	::GetPrivateProfileString(L"User",L"UserName",L"",userName,25,m_Path);
	
	len = _tcslen(userName);
	m_UserName = new TCHAR[len+1];
	ZeroMemory((void*)m_UserName,(len+1)*2);
	_tcscat(m_UserName,userName);

}


void CINIManager::SetUserName(LPTSTR userName)
{
	if (m_UserName != NULL)
	{
		delete[] m_UserName;
		m_UserName = NULL;
	}

	size_t len = _tcslen(userName);
	m_UserName = new TCHAR[len+1];
	ZeroMemory((void*)m_UserName,(len+1)*2);
	_tcscat(m_UserName,userName);
}


UINT32 CINIManager::GetServerIP(void)
{
	return m_ServerIP;
	//return UINT32();
}


UINT16 CINIManager::GetServerLogPort(void)
{
	return UINT16(m_ServerLogPort);
}



UINT16 CINIManager::GetServicesPort(void)
{
	return m_ServicesPort;
}



LPTSTR CINIManager::GetVideoDir(void)
{
	size_t len = _tcslen(m_VideoDir);
	LPTSTR p = new TCHAR[len+1];
	memcpy(p,m_VideoDir,(len+1)*2);

	return p;
}


LPTSTR CINIManager::GetImageDir(void)
{
	size_t len = _tcslen(m_ImageDir);
	LPTSTR p = new TCHAR[len+1];
	memcpy(p,m_ImageDir,(len+1)*2);

	return p;
}


LPTSTR CINIManager::GetUserName(void)
{
	LPTSTR p = new TCHAR[25];
	memcpy(p,m_UserName,50);

	return p;
}


char* CINIManager::GetcharUserName(void)
{
	char *p = new char[25];
	ZeroMemory(p,25);
	TcharToChar(m_UserName,p);

	return p;
}


BOOL CINIManager::WriteUserName(TCHAR*name)
{
	SetUserName(name);
	return WritePrivateProfileString(L"User",L"UserName",name,m_Path);
}
