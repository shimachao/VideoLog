#pragma once
/*
�汾��V0.1
���ߣ�http://blog.csdn.net/shimachao
����VS2010
����VS2010+win7 64λ�ϲ���ͨ��
����ļ�������GetScreenShot.h(���ļ�),GetScreenShot.cpp,Qedit.h
��������GetScreenShot
����������һ��avi��Ƶ�ļ������Ƶ������ͼ��Ԥ��ͼ��
֧�ֵĸ�ʽֻ���ǣ�avi,rm,mpeg,wmv��IMediaDet֧�ֵ���Ƶ��ʽ


*/
#include <afx.h>
#include <comdef.h>
#include <windows.h>
#include <DShow.h>
#include <streams.h>
#pragma include_alias( "dxtrans.h", "qedit.h" )
#define __IDxtCompositor_INTERFACE_DEFINED__
#define __IDxtAlphaSetter_INTERFACE_DEFINED__
#define __IDxtJpeg_INTERFACE_DEFINED__
#define __IDxtKey_INTERFACE_DEFINED__
#include "Qedit.h"

extern "C"{
#include "jpeglib.h"
};
#pragma comment(lib,"strmiids.lib")
#pragma comment(lib,"Strmbasd.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"jpeg.lib")

#define VALUE 1
#define PROPROTION 2

bool GetScreenShot(TCHAR *filePath,double streamTime,int MODE,LPCTSTR picturePath,int width=0,int height=0);
/*
����˵����
filePath��Ҫ��ȡԤ����Ƶ����ȫ·��(·��+�ļ���)
streamTime,MODE:Ԥ����ʱ���ʱ��ģʽ
	VALUEΪֵģʽ��ʱ���ΪstreamTime
	PROPROTIONΪ����ģʽ��ʱ���ΪstreamTime*��Ƶ���ܳ���
�������ʱ��㳬������Ƶ����ʱ�䣬��������Ƶ���һ֡����
picturePath:���ɵ�����ͼ����·��
width,height:����������ͼ�Ĵ�С,Ĭ��ΪֵΪ0�����Ϊ0�����ɵ�����ͼ��СΪԴ��Ƶ֡�Ĵ�С
*/


bool GetScreenShotJpg(TCHAR *filePath,double streamTime,int MODE,LPCTSTR picturePath);
