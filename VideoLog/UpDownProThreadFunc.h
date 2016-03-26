#pragma once
#include <Windows.h>
struct PAR_TO_PROTHREAD{
	HWND hWnd;
	CProgressCtrl* pPro;
	TCHAR *filename;
};
DWORD WINAPI UpDownProThreadFunc(LPVOID lpParam);
