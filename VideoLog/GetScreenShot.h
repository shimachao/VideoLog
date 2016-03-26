#pragma once
/*
版本：V0.1
作者：http://blog.csdn.net/shimachao
基于VS2010
已在VS2010+win7 64位上测试通过
打包文件包括：GetScreenShot.h(本文件),GetScreenShot.cpp,Qedit.h
函数名：GetScreenShot
函数用来从一个avi视频文件获得视频的缩略图（预览图）
支持的格式只能是：avi,rm,mpeg,wmv等IMediaDet支持的视频格式


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
参数说明：
filePath：要提取预览视频的完全路径(路径+文件名)
streamTime,MODE:预览的时间点时间模式
	VALUE为值模式，时间点为streamTime
	PROPROTION为比例模式，时间点为streamTime*视频流总长度
如果给的时间点超过了视频流的时间，将捕获视频最后一帧画面
picturePath:生成的缩略图保存路径
width,height:所生成缩略图的大小,默认为值为0，如果为0，生成的缩略图大小为源视频帧的大小
*/


bool GetScreenShotJpg(TCHAR *filePath,double streamTime,int MODE,LPCTSTR picturePath);
