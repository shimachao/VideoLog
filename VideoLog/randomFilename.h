#pragma	once
#include <Windows.h>
//���һ��������ļ�������ָ��Ŀ¼�ͺ�׺��
TCHAR *GetRandFileFullName(const TCHAR*prefix,const TCHAR*suffix);

//�õ��ղ����ɵ���Ƶ�ļ���ȫ·��������Ϊ����ͼģ������
TCHAR *GetVideoFullFileName(void);
//����ͼ
TCHAR *GetImageFullFileName(void);


char *GetVideocharFileName(void);
TCHAR *GetVideoFileName(void);

char *GetImagecharFileName(void);
TCHAR *GetImageFileName(void);
