#pragma once

//char字符串转换为宽字符串
void TcharToChar(const TCHAR* tchar, char* _char);
//宽字符串转为char字符串
void CharToTchar (const char * _char, TCHAR * tchar);

//utf-16（wide character）转为utf-8编码,要求参数以null结尾，返回的指针要记得释放
char *WideCharToUTF8(LPCWSTR wstr);

//UTF-16(wide character)转为ANSI编码字符串,要求参数以null结尾，返回的指针要记得释放
char *WideCharToANSI(LPCWSTR wstr);

//UTF-8编码转为Wide 字符编码,要求参数以null结尾，返回的指针要记得释放
WCHAR *utf8ToWideChar(char * str);
