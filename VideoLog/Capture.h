//视频+音频 捕获链表类
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

	//根据graph指令和graph现有的状态控制graph的状态
	void Run(void);
	void Pause(void);
	void Stop();
	GRAPH_STATE GetGraphState(void);			//获得graph捕获链的状态

private:
	HRESULT InitCaptureGraphBulder(void);		//初始化pGraph和pBuilder
	HRESULT QueryInterfaces(void);				//查询pControl、pVWindow接口
	HRESULT CreateSysEnum(void);				//创建系统设备枚举器

	HRESULT CreateCapture(REFCLSID,IBaseFilter**,WCHAR*);	
	HRESULT CreateCompressor(REFCLSID,WCHAR*,IBaseFilter**);

	HRESULT SetupPreviewWindow(void);			//设置视频预览窗口

	HRESULT ConnectFilters(void);				//连接各个filter，并创建pMux

	HRESULT Preview(void);						//预览视频

private:
	GRAPH_STATE graph_flag;				//流的状态：已停止，已暂停，运行ing

	IGraphBuilder			*pGraph;		//graph管理器
	ICaptureGraphBuilder2	*pBuilder;		//增强捕获链表管理器
	IMediaControl			*pControl;		//流控制接口
	IVideoWindow			*pVWindow;		//视频预览窗口
	HWND					m_hwnd;			//视频要显示的窗口句柄

	ICreateDevEnum	*pSysDevEnum;			//系统设备枚举器
	IBaseFilter		*pVideoCap;				//视频采集filter
	IBaseFilter		*pAudioCap;				//音频采集filter
	IBaseFilter		*pVideoCompressor;		//视频压缩器
	IBaseFilter		*pAudioCompressor;		//音频压缩器
	IBaseFilter		*pMux;					//混合filter接口
	IFileSinkFilter *pSink;					//捕获的文件名接口
public:
	HRESULT SetVideoFrameSize(void);
};
