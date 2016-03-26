#pragma once
#include <afx.h>
class CINIManager
{
public:
	static CINIManager* GetINIManager();//获得指向配置文件类的指针


private:
	CINIManager();


private:
	static CINIManager* m_pINIManager;	//指向配置文件实例的指针
	LPCTSTR m_Path;						//配置文件路径

	UINT32	m_ServerIP;					//服务器IP地址,网络字节序
	UINT16	m_ServerLogPort;			//服务器的登陆端口，网络字节序
	UINT16  m_ServicesPort;				//服务器事务处理端口，去登陆以外的端口

	LPTSTR m_VideoDir;					//视频文件放置目录
	LPTSTR m_ImageDir;					//视频缩略图放置目录

	LPTSTR	m_UserName;					//默认用户名
	
	void ReadConfig(void);				// 读取平配置文件，只给构造函数调用
	void SetUserName(LPTSTR);			//设置用户名，只供WriteUserName使用


public:
	UINT32 GetServerIP(void);			//获得服务器端IP
	UINT16 GetServerLogPort(void);		//获得服务器端登陆端口号
	UINT16 GetServicesPort(void);		//获得服务器事务处理端口
	LPTSTR GetVideoDir(void);			//获得保存视频文件的目录路径
	LPTSTR GetImageDir(void);			//获得保存缩略图文件的目录路径
	LPTSTR GetUserName(void);			//获得最近的用户名
	char*  GetcharUserName(void);
	BOOL	WriteUserName(TCHAR*);		//写入这次登陆用的用户名，作为下次登陆时默认的用户名显示在登陆界面上
};

