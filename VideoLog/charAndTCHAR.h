#pragma once

//char�ַ���ת��Ϊ���ַ���
void TcharToChar(const TCHAR* tchar, char* _char);
//���ַ���תΪchar�ַ���
void CharToTchar (const char * _char, TCHAR * tchar);

//utf-16��wide character��תΪutf-8����,Ҫ�������null��β�����ص�ָ��Ҫ�ǵ��ͷ�
char *WideCharToUTF8(LPCWSTR wstr);

//UTF-16(wide character)תΪANSI�����ַ���,Ҫ�������null��β�����ص�ָ��Ҫ�ǵ��ͷ�
char *WideCharToANSI(LPCWSTR wstr);

//UTF-8����תΪWide �ַ�����,Ҫ�������null��β�����ص�ָ��Ҫ�ǵ��ͷ�
WCHAR *utf8ToWideChar(char * str);
