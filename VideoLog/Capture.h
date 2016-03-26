//��Ƶ+��Ƶ ����������
#pragma once
//#include <Qedit.h>
#include <DShow.h>
#pragma comment (lib,"Strmiids")
#pragma comment (lib,"Quartz")
typedef FILTER_STATE GRAPH_STATE;
class Capture
{
public:
	Capture(HWND);
	virtual ~Capture(void);

	//����graphָ���graph���е�״̬����graph��״̬
	void Run(void);
	void Pause(void);
	void Stop();
	GRAPH_STATE GetGraphState(void);			//���graph��������״̬

private:
	HRESULT InitCaptureGraphBulder(void);		//��ʼ��pGraph��pBuilder
	HRESULT QueryInterfaces(void);				//��ѯpControl��pVWindow�ӿ�
	HRESULT CreateSysEnum(void);				//����ϵͳ�豸ö����

	HRESULT CreateCapture(REFCLSID,IBaseFilter**,WCHAR*);	
	HRESULT CreateCompressor(REFCLSID,WCHAR*,IBaseFilter**);

	HRESULT SetupPreviewWindow(void);			//������ƵԤ������

	HRESULT ConnectFilters(void);				//���Ӹ���filter��������pMux

	HRESULT Preview(void);						//Ԥ����Ƶ

private:
	GRAPH_STATE graph_flag;				//����״̬����ֹͣ������ͣ������ing

	IGraphBuilder			*pGraph;		//graph������
	ICaptureGraphBuilder2	*pBuilder;		//��ǿ�������������
	IMediaControl			*pControl;		//�����ƽӿ�
	IVideoWindow			*pVWindow;		//��ƵԤ������
	HWND					m_hwnd;			//��ƵҪ��ʾ�Ĵ��ھ��

	ICreateDevEnum	*pSysDevEnum;			//ϵͳ�豸ö����
	IBaseFilter		*pVideoCap;				//��Ƶ�ɼ�filter
	IBaseFilter		*pAudioCap;				//��Ƶ�ɼ�filter
	IBaseFilter		*pVideoCompressor;		//��Ƶѹ����
	IBaseFilter		*pAudioCompressor;		//��Ƶѹ����
	IBaseFilter		*pMux;					//���filter�ӿ�
	IFileSinkFilter *pSink;					//������ļ����ӿ�
public:
	HRESULT SetVideoFrameSize(void);
};
