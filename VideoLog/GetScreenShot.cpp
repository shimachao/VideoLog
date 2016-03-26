#include "GetScreenShot.h"
#include <cstdio>
bool GetScreenShot(TCHAR *filePath,double streamTime,int MODE,LPCTSTR picturePath,int width,int height)
{
	HRESULT hr = CoInitialize(NULL);//初始化COM库

	if (FAILED(hr))
	{
		return false;
	}
	IMediaDet*pDet;
	hr = CoCreateInstance(CLSID_MediaDet,NULL,CLSCTX_INPROC_SERVER,IID_IMediaDet,(void**)&pDet);
	if (FAILED(hr))
	{
		return false;
	}

	BSTR bstrFilePath = _bstr_t(filePath);
	hr = pDet->put_Filename(bstrFilePath);
	//查询视频文件中的视频流
	long iStreams;
	bool bFound = false;
	hr = pDet->get_OutputStreams(&iStreams);
	if(FAILED(hr))  return false;
	for (long i = 0;i < iStreams;i++)
	{
		GUID major_type;
		hr = pDet->put_CurrentStream(i);
		hr = pDet->get_StreamType(&major_type);
		if (major_type == MEDIATYPE_Video)
		{
			bFound = true;
			break;
		}
	}
	if (bFound == false)
	{
		//没有找到视频流
		return false;
	}

	//获得视频帧大小
	if (width == 0 || height == 0)
	{
		AM_MEDIA_TYPE mt;
		hr = pDet->get_StreamMediaType(&mt);
		
		VIDEOINFOHEADER *pVih = (VIDEOINFOHEADER*)(mt.pbFormat);
		width = pVih->bmiHeader.biWidth;
		height = pVih->bmiHeader.biHeight;
		if (height < 0) height *= -1;
		
		FreeMediaType(mt);
	}//if

	//获得视频帧数据
	long buffSize;
	hr = pDet->GetBitmapBits(0,&buffSize,0,width,height);
	char *pBuffer = new char[buffSize];
	if(!pBuffer)
		return false;

	double streamTotalTime;
	pDet->get_StreamLength(&streamTotalTime);
	if (MODE == PROPROTION)
	{
		streamTime = streamTotalTime*streamTime;
	}
	if (streamTime > streamTotalTime)
	{
		streamTime = streamTotalTime;
	}

	hr = pDet->GetBitmapBits(streamTime,0,pBuffer,width,height);
	pDet->Release();
	CoUninitialize();
	if (FAILED(hr))
	{
		//获取视频帧数据失败
		return false;
	}
	//保存到图片文件
	BITMAPINFOHEADER * bmih = (BITMAPINFOHEADER*)pBuffer;
	HDC hdcDest = ::GetDC(0);
	void * pData = pBuffer + sizeof(BITMAPINFOHEADER);
	// Note: In general a BITMAPINFOHEADER can include extra color 
	// information at the end, so calculating the offset to the image 
	// data is not generally correct. However, the IMediaDet interface 
	// always returns an RGB-24 image with no extra color information 
	BITMAPINFO bmi;
	ZeroMemory(&bmi, sizeof(BITMAPINFO));
	CopyMemory(&(bmi.bmiHeader),bmih,sizeof(BITMAPINFOHEADER));
	HBITMAP hB = CreateDIBitmap(hdcDest,
								bmih,
								CBM_INIT,
								pData,
								&bmi,
								DIB_RGB_COLORS);
	// write to bitmap
	BITMAPFILEHEADER   hdr;     //Bitmap的头信息 
	LPBITMAPINFOHEADER    lpbi; // Bitmap的文件信息（包括数据） 
	lpbi = (LPBITMAPINFOHEADER)pBuffer; 
	int nColors = 1 << lpbi->biBitCount; 
	if (nColors > 256) 
		nColors = 0; 
	hdr.bfType     = ((WORD) ('M' << 8) | 'B');     //always is "BM" 
	hdr.bfSize     = buffSize + sizeof( hdr ); 
	hdr.bfReserved1    = 0; 
	hdr.bfReserved2    = 0; 
	hdr.bfOffBits = (DWORD) (sizeof(BITMAPFILEHEADER) + lpbi->biSize);
	// 保存图  
	CFile bitmapFile(picturePath, CFile::modeNoTruncate|
		CFile::modeReadWrite | CFile::modeCreate | CFile::typeBinary); 
	bitmapFile.Write(&hdr, sizeof(BITMAPFILEHEADER)); 
	bitmapFile.Write(pBuffer, buffSize); 
	bitmapFile.Close();
	
	delete [] pBuffer;

	return true;
}


bool GetScreenShotJpg(TCHAR *filePath,double streamTime,int MODE,LPCTSTR picturePath)
{
	HRESULT hr = CoInitialize(NULL);//初始化COM库

	if (FAILED(hr))
	{
		return false;
	}
	IMediaDet*pDet;
	hr = CoCreateInstance(CLSID_MediaDet,NULL,CLSCTX_INPROC_SERVER,IID_IMediaDet,(void**)&pDet);
	if (FAILED(hr))
	{
		return false;
	}

	BSTR bstrFilePath = _bstr_t(filePath);
	hr = pDet->put_Filename(bstrFilePath);
	//查询视频文件中的视频流
	long iStreams;
	bool bFound = false;
	hr = pDet->get_OutputStreams(&iStreams);
	if(FAILED(hr))  return false;
	for (long i = 0;i < iStreams;i++)
	{
		GUID major_type;
		hr = pDet->put_CurrentStream(i);
		hr = pDet->get_StreamType(&major_type);
		if (major_type == MEDIATYPE_Video)
		{
			bFound = true;
			break;
		}
	}
	if (bFound == false)
	{
		//没有找到视频流
		return false;
	}

	//获得视频帧大小
	AM_MEDIA_TYPE mt;
	hr = pDet->get_StreamMediaType(&mt);

	VIDEOINFOHEADER *pVih = (VIDEOINFOHEADER*)(mt.pbFormat);
	int width = pVih->bmiHeader.biWidth;
	int height = pVih->bmiHeader.biHeight;
	if (height < 0) height *= -1;

	FreeMediaType(mt);
	

	//获得视频帧数据
	long buffSize;
	hr = pDet->GetBitmapBits(0,&buffSize,0,width,height);
	char *pBuffer = new char[buffSize];
	if(!pBuffer)
		return false;

	double streamTotalTime;
	pDet->get_StreamLength(&streamTotalTime);
	if (MODE == PROPROTION)
	{
		streamTime = streamTotalTime*streamTime;
	}
	if (streamTime > streamTotalTime)
	{
		streamTime = streamTotalTime;
	}

	hr = pDet->GetBitmapBits(streamTime,0,pBuffer,width,height);
	if (FAILED(hr))
	{
		//获取视频帧数据失败
		return false;
	}

	pDet->Release();
	//printf("%d - %d*%d*3 = %d\n",buffSize,width,height,buffSize-width*height*3);
	//申请并初始化jpeg压缩对象，同时指定错误处理器
	struct jpeg_compress_struct jcs;
	//声明错误处理器，并赋值给jcs.err域
	struct jpeg_error_mgr jem;
	jcs.err = jpeg_std_error(&jem);

	jpeg_create_compress(&jcs);

	//指定压缩后的图像所存放的目标文件，注意，目标以二进制模式打开
	FILE* f = _tfopen(picturePath,L"wb");
	if (NULL == f)
	{
		delete[] pBuffer;
		return false;
	}
	jpeg_stdio_dest(&jcs,f);

	//设置压缩参数，主要参数有图像宽、高、色彩通道数（1,：索引图像，3,：其他），色彩空间（JSC_GRAYSCALE表示灰度图，
	//JCS_RGB表示彩色图像），压缩质量等,如下：
	jcs.image_width = width;
	jcs.image_height = height;
	jcs.input_components = 3;
	jcs.in_color_space = JCS_RGB;

	jpeg_set_defaults(&jcs);
	jpeg_set_quality(&jcs,80,true);
	/*jcs.scale_num = 1;	//这两个可以设置输出的图片大小和原来图片大小的比例，scale_num为分子，scale_denom为分母
	jcs.scale_denom =2;*/
	jpeg_calc_jpeg_dimensions(&jcs);
	//因为获得的帧数据格式是40字节头信息+BGR序列，要调整成RGB序列
	char *pData = pBuffer+40;
	int b,r;
	volatile int tem;
	for (b=0,r=2;r<buffSize-40;b+=3,r+=3)
	{
		tem = pData[b];
		pData[b] = pData[r];
		pData[r] = tem;
	}

	//开始压缩
	jpeg_start_compress(&jcs,TRUE);
	JSAMPROW row_pointer[1];
	int row_stride = jcs.image_width*3;

	while(jcs.next_scanline < jcs.image_height)
	{
		//windows下的像素是一行一行从下到上的，所以从最下面一行开始读取
		//row_pointer[0] =(BYTE*) & pBuffer[jcs.next_scanline*row_stride];
		row_pointer[0] = (BYTE*) &pData[(jcs.image_height-jcs.next_scanline-1)*row_stride];
		jpeg_write_scanlines(&jcs,row_pointer,1);
	}
	//压缩完毕
	jpeg_finish_compress(&jcs);
	fclose(f);
	//释放JPEG压缩对象
	jpeg_destroy_compress(&jcs);
	delete []pBuffer;
	CoUninitialize();
	return true;

}
