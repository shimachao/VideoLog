#include "StdAfx.h"
#include "Capture.h"
#include <assert.h>
#include "randomFilename.h"

#define FILTER_RELEASE(x) if(x!=NULL) {x->Release();x=NULL;}
Capture::Capture(HWND mhwnd)
{
	CoInitialize(NULL);
	pGraph = NULL;
	pBuilder = NULL;
	pControl = NULL;
	pVWindow = NULL;

	pSysDevEnum = NULL;
	pVideoCap = NULL;
	pAudioCap = NULL;
	pVideoCompressor = NULL;
	pAudioCompressor = NULL;
	pMux = NULL;
	graph_flag = State_Stopped;
	m_hwnd = mhwnd;
	Preview();
}

Capture::~Capture(void)
{
	if (pControl)
	{
		pControl->Stop();
		FILTER_RELEASE(pControl);
	}
	FILTER_RELEASE(pAudioCap);
	FILTER_RELEASE(pAudioCompressor);
	FILTER_RELEASE(pBuilder);
	FILTER_RELEASE(pGraph);
	FILTER_RELEASE(pMux);
	FILTER_RELEASE(pSysDevEnum);
	FILTER_RELEASE(pVideoCap);
	FILTER_RELEASE(pVideoCompressor);
	FILTER_RELEASE(pVWindow);
	m_hwnd = NULL;

	CoUninitialize();
}

TCHAR*filename = NULL;
void Capture::Run(void)
{
	if(pControl == NULL) return;
	if (graph_flag == State_Stopped)
	{
		pControl->Stop();
		ConnectFilters();
	}
	if (filename != NULL)
	{
		delete[] filename;
		filename = NULL;
	}
	filename = GetRandFileFullName(L"./videos/",L".avi");
	pSink->SetFileName(filename,0);
	pControl->Run();
	graph_flag = State_Running;
}

void Capture::Pause(void)
{
	if(pControl) pControl->Pause();
	graph_flag = State_Paused;
}

void Capture::Stop(void)
{
	if(pControl) pControl->Stop();
	graph_flag = State_Paused;
}

GRAPH_STATE Capture::GetGraphState(void)
{
	return graph_flag;
}

HRESULT Capture::InitCaptureGraphBulder(void)
{
	assert(pGraph==NULL&&pBuilder==NULL);

	HRESULT hr = CoCreateInstance(CLSID_CaptureGraphBuilder2,NULL,CLSCTX_INPROC_SERVER,
									IID_ICaptureGraphBuilder2,(void**)&pBuilder);
	if (SUCCEEDED(hr))
	{
		hr = CoCreateInstance(CLSID_FilterGraph,NULL,CLSCTX_INPROC_SERVER,
								IID_IGraphBuilder,(void**)&pGraph);
		if (SUCCEEDED(hr))
		{
			pBuilder->SetFiltergraph(pGraph);
			return S_OK;
		}
		else
			pBuilder->Release();
	}
	return hr;
}

HRESULT Capture::QueryInterfaces(void)
{
	assert(pGraph!=NULL);
	HRESULT hr = pGraph->QueryInterface(IID_IMediaControl,(void**)&pControl);//ý����ƽӿ�
	hr |= pGraph->QueryInterface(IID_IVideoWindow,(void**)&pVWindow);		//��Ƶ��ʾ���ڽӿ�

	return hr;
}

HRESULT Capture::CreateSysEnum(void)
{
	assert(pSysDevEnum==NULL);
	return CoCreateInstance(CLSID_SystemDeviceEnum,NULL,CLSCTX_INPROC_SERVER,
									IID_ICreateDevEnum,(void**)&pSysDevEnum);
}

HRESULT Capture::CreateCapture(REFCLSID category,IBaseFilter**ppCapFilter,
								WCHAR*frendlyname)
{
	assert(pSysDevEnum!=NULL);

	IEnumMoniker *pClassEnum = NULL;
	HRESULT hr = pSysDevEnum->CreateClassEnumerator(category,&pClassEnum,0);
	if (hr!=S_OK)
	{
		return hr;
	}
	ULONG cFetched;
	IMoniker* pMonker;
	if (pClassEnum->Next(1,&pMonker,&cFetched)==S_OK)
	{
		hr = pMonker->BindToObject(0,0,IID_IBaseFilter,(void**)ppCapFilter);
		pMonker->Release();
	}
	pClassEnum->Release();
	if (SUCCEEDED(hr))
	{
		hr = pGraph->AddFilter(*ppCapFilter,frendlyname);
	}
	return hr;
}

HRESULT Capture::CreateCompressor(REFCLSID category,WCHAR*frendlyname,
									IBaseFilter**ppCompressorFilter)
{
	IEnumMoniker *pClassEnum = NULL;
	HRESULT hr = pSysDevEnum->CreateClassEnumerator(category,&pClassEnum,0);
	if (FAILED(hr))
	{
		return hr;
	}

	ULONG cFetched;
	IMoniker *pMoniker = NULL;
	while(pClassEnum->Next(1,&pMoniker,&cFetched) == S_OK)
	{
		IPropertyBag *pProp = NULL;
		pMoniker->BindToStorage(0,0,IID_IPropertyBag,(void**)&pProp);
		VARIANT varName;
		varName.vt = VT_BSTR;
		pProp->Read(L"FriendlyName",&varName,NULL);
		CString str = varName.bstrVal;

		if (str.Find(frendlyname,0)!=-1)
		{
			pMoniker->BindToObject(NULL,NULL,IID_IBaseFilter,(void**)ppCompressorFilter);
			pMoniker->Release();
			break;
		}

		VariantClear(&varName);
		pProp->Release();
	}
	pClassEnum->Release();

	if (*ppCompressorFilter == NULL)
	{
		if(category==CLSID_VideoCompressorCategory)
		AfxMessageBox(_T("û���ҵ���Ƶѹ�����!"));
		else
		AfxMessageBox(_T("û�ҵ���Ƶѹ�������"));
		return S_FALSE;
	}
	pGraph->AddFilter(*ppCompressorFilter,frendlyname);
	return S_OK;
}

HRESULT Capture::SetupPreviewWindow(void)
{
	assert(m_hwnd!=NULL);

	HRESULT hr = pVWindow->put_Visible(OAFALSE);		//��Ƶ�������ú�ǰ���ɼ�
	hr |= pVWindow->put_Owner((OAHWND)m_hwnd);			//����Ԥ������������
	if (FAILED(hr))
	{
		return hr;
	}

	hr = pVWindow->put_WindowStyle(WS_CLIPCHILDREN|WS_CHILD);//���ô�����ʾ����
	if (FAILED(hr))
	{
		return hr;
	}
	CRect rc;
	::GetClientRect(m_hwnd,&rc);
	hr = pVWindow->SetWindowPosition(0,0,rc.right,rc.bottom);
	hr |= pVWindow->put_Visible(OATRUE);		//����Ԥ�����ڿɼ�
	return hr;
}
HRESULT Capture::Preview(void)
{
	//������ƵԤ����
	HRESULT hr = InitCaptureGraphBulder();
	assert(SUCCEEDED(hr));
	hr = QueryInterfaces();
	assert(SUCCEEDED(hr));
	hr = CreateSysEnum();
	assert(SUCCEEDED(hr));
	hr = CreateCapture(CLSID_VideoInputDeviceCategory,&pVideoCap,L"Video Capture");
	assert(SUCCEEDED(hr));
	//������Ƶ�����СΪ320x240
	SetVideoFrameSize();
	hr = pBuilder->RenderStream(&PIN_CATEGORY_PREVIEW,&MEDIATYPE_Video,
		pVideoCap,NULL,NULL);
	assert(SUCCEEDED(hr));
	SetupPreviewWindow();
	pControl->Run();
	return hr;
}


HRESULT Capture::ConnectFilters(void)
{
	assert(pBuilder != NULL);
	HRESULT hr = CreateCapture(CLSID_AudioInputDeviceCategory,&pAudioCap,L"Audio Capture");
	assert(SUCCEEDED(hr));
	hr = CreateCompressor(CLSID_VideoCompressorCategory,L"x264vfw - H.264/MPEG-4 AVC codec",
							&pVideoCompressor);
	assert(SUCCEEDED(hr));
	hr = CreateCompressor(CLSID_AudioCompressorCategory,L"GSM",
							&pAudioCompressor);
	assert(SUCCEEDED(hr));
	//���ò����ļ������ơ�����pMux
	//filename = GetRandFileFullName(L"./videos/",L".avi");
	hr = pBuilder->SetOutputFileName(&MEDIASUBTYPE_Avi,L"c:\\tem.avi",
										&pMux,&pSink);
	//������Ƶ������
	hr = pBuilder->RenderStream(&PIN_CATEGORY_CAPTURE,&MEDIATYPE_Video,
								pVideoCap,pVideoCompressor,pMux);
	assert(SUCCEEDED(hr));
	//������Ƶ������
	hr = pBuilder->RenderStream(&PIN_CATEGORY_CAPTURE,&MEDIATYPE_Audio,
								pAudioCap,pAudioCompressor,pMux);
	assert(SUCCEEDED(hr));
	//������Ƶ��Ϊ����
	IConfigAviMux *pConfig = NULL;
	hr = pMux->QueryInterface(IID_IConfigAviMux,(void**)&pConfig);
	hr |= pConfig->SetMasterStream(1);
	hr |= pConfig->Release();
	assert(SUCCEEDED(hr));
/*	//���òο�ʱ��
	IReferenceClock *pIReferenceClock = NULL;
	hr = pVideoCap->QueryInterface(IID_IReferenceClock,(void**)&pIReferenceClock);
	assert(SUCCEEDED(hr));
	IMediaFilter *pIMediaFilter = NULL;
	hr = pGraph->QueryInterface(IID_IMediaFilter,(void**)&pIMediaFilter);
	assert(SUCCEEDED(hr));
	pIMediaFilter->SetSyncSource(pIReferenceClock);
	pIMediaFilter->Release();
	pIReferenceClock->Release();
*/
	//������ʱ����
	IAMGraphStreams *pIAMGStream = NULL;
	hr = pGraph->QueryInterface(IID_IAMGraphStreams,(void**)&pIAMGStream);
	assert(SUCCEEDED(hr));
	hr = pIAMGStream->SyncUsingStreamOffset(TRUE);
	assert(SUCCEEDED(hr));
	pIAMGStream->Release();

	//����AVI MUX��put_mod
	 IConfigInterleaving *pIConfg = NULL;
	 hr = pMux->QueryInterface(IID_IConfigInterleaving,(void**)&pIConfg);
	 assert(SUCCEEDED(hr));
	 hr = pIConfg->put_Mode(INTERLEAVE_CAPTURE);
	 assert(SUCCEEDED(hr));
	 pIConfg->Release();
	 
	return S_OK;
}

HRESULT Capture::SetVideoFrameSize(void)
{
	IAMStreamConfig *pConfig;
	//��ʼ��IAMStreamConfig interface
	HRESULT hr = pBuilder->FindInterface(&PIN_CATEGORY_CAPTURE,&MEDIATYPE_Interleaved,pVideoCap,
		IID_IAMStreamConfig,(void**)&pConfig);
	if (hr != NOERROR)
	{
		hr = pBuilder->FindInterface(&PIN_CATEGORY_CAPTURE,&MEDIATYPE_Video,pVideoCap,
			IID_IAMStreamConfig,(void**)&pConfig);
		if(hr != S_OK)
		{
			AfxMessageBox(L"load config fialed!");
			return S_FALSE;
			//return hr;
		}
	}

	//���ò���(ȡԭ���Ĳ������޸���Ҫ�ı�Ĳ���)
	AM_MEDIA_TYPE *mmt;
	pConfig->GetFormat(&mmt);
	VIDEOINFOHEADER *vvih = (VIDEOINFOHEADER*)mmt->pbFormat;
	AM_MEDIA_TYPE*pmt = mmt;
	VIDEOINFOHEADER *pvih = (VIDEOINFOHEADER*)pmt->pbFormat;
	pvih->bmiHeader.biHeight = 240;
	pvih->bmiHeader.biWidth = 320;
	pvih->AvgTimePerFrame = 20;
	pmt->pbFormat = (BYTE *)pvih;
	pConfig->SetFormat(pmt);
	return hr;
}
