#pragma once
#include <afx.h>
class CINIManager
{
public:
	static CINIManager* GetINIManager();//���ָ�������ļ����ָ��


private:
	CINIManager();


private:
	static CINIManager* m_pINIManager;	//ָ�������ļ�ʵ����ָ��
	LPCTSTR m_Path;						//�����ļ�·��

	UINT32	m_ServerIP;					//������IP��ַ,�����ֽ���
	UINT16	m_ServerLogPort;			//�������ĵ�½�˿ڣ������ֽ���
	UINT16  m_ServicesPort;				//������������˿ڣ�ȥ��½����Ķ˿�

	LPTSTR m_VideoDir;					//��Ƶ�ļ�����Ŀ¼
	LPTSTR m_ImageDir;					//��Ƶ����ͼ����Ŀ¼

	LPTSTR	m_UserName;					//Ĭ���û���
	
	void ReadConfig(void);				// ��ȡƽ�����ļ���ֻ�����캯������
	void SetUserName(LPTSTR);			//�����û�����ֻ��WriteUserNameʹ��


public:
	UINT32 GetServerIP(void);			//��÷�������IP
	UINT16 GetServerLogPort(void);		//��÷������˵�½�˿ں�
	UINT16 GetServicesPort(void);		//��÷�����������˿�
	LPTSTR GetVideoDir(void);			//��ñ�����Ƶ�ļ���Ŀ¼·��
	LPTSTR GetImageDir(void);			//��ñ�������ͼ�ļ���Ŀ¼·��
	LPTSTR GetUserName(void);			//���������û���
	char*  GetcharUserName(void);
	BOOL	WriteUserName(TCHAR*);		//д����ε�½�õ��û�������Ϊ�´ε�½ʱĬ�ϵ��û�����ʾ�ڵ�½������
};

