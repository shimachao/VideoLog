#include "charAndTCHAR.h"
#include "stdafx.h"

void TcharToChar(const TCHAR* tchar, char* _char)
{
	int iLength ;  
	//��ȡ�ֽڳ���   
	iLength = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);  
	//��tcharֵ����_char    
	WideCharToMultiByte(CP_ACP, 0, tchar, -1, _char, iLength, NULL, NULL);   
}


void CharToTchar (const char * _char, TCHAR * tchar)  
{  
	int iLength ;  

	iLength = MultiByteToWideChar (CP_ACP, 0, _char, strlen (_char) + 1, NULL, 0);  
	MultiByteToWideChar (CP_ACP, 0, _char, strlen (_char) + 1, tchar, iLength);  
}  


char *WideCharToUTF8(LPCWSTR wstr)
{
	int len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);  
	char *str = new char[len+1];
	ZeroMemory(str,len+1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	return str;
}

char *WideCharToANSI(LPCWSTR wstr)
{
	int len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);  
	char *str = new char[len+1];
	ZeroMemory(str,len+1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	return str;
}

//UTF-8����תΪWide �ַ�����,Ҫ�������null��β�����ص�ָ��Ҫ�ǵ��ͷ�
WCHAR *utf8ToWideChar(char * str)
{
	int iLength = MultiByteToWideChar (CP_UTF8,0,str,-1,NULL,0);  
	WCHAR *wstr = new WCHAR[iLength];
	ZeroMemory(wstr,iLength*sizeof(WCHAR));
	MultiByteToWideChar (CP_UTF8,0,str,-1, wstr, iLength);  
	return wstr;
}