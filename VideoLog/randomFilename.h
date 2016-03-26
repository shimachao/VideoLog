#pragma	once
#include <Windows.h>
//获得一个随机的文件名，需指定目录和后缀名
TCHAR *GetRandFileFullName(const TCHAR*prefix,const TCHAR*suffix);

//得到刚才生成的视频文件名全路径，特意为缩略图模块所用
TCHAR *GetVideoFullFileName(void);
//缩略图
TCHAR *GetImageFullFileName(void);


char *GetVideocharFileName(void);
TCHAR *GetVideoFileName(void);

char *GetImagecharFileName(void);
TCHAR *GetImageFileName(void);
