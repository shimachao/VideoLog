#include "GetScreenShot.h"
#include <cstdio>
bool GetScreenShot(TCHAR *filePath,double streamTime,int MODE,LPCTSTR picturePath,int width,int height)
{
	HRESULT hr = CoInitialize(NULL);//��ʼ��COM��

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
	//��ѯ��Ƶ�ļ��е���Ƶ��
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
		//û���ҵ���Ƶ��
		return false;
	}

	//�����Ƶ֡��С
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

	//�����Ƶ֡����
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
		//��ȡ��Ƶ֡����ʧ��
		return false;
	}
	//���浽ͼƬ�ļ�
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
	BITMAPFILEHEADER   hdr;     //Bitmap��ͷ��Ϣ 
	LPBITMAPINFOHEADER    lpbi; // Bitmap���ļ���Ϣ���������ݣ� 
	lpbi = (LPBITMAPINFOHEADER)pBuffer; 
	int nColors = 1 << lpbi->biBitCount; 
	if (nColors > 256) 
		nColors = 0; 
	hdr.bfType     = ((WORD) ('M' << 8) | 'B');     //always is "BM" 
	hdr.bfSize     = buffSize + sizeof( hdr ); 
	hdr.bfReserved1    = 0; 
	hdr.bfReserved2    = 0; 
	hdr.bfOffBits = (DWORD) (sizeof(BITMAPFILEHEADER) + lpbi->biSize);
	// ����ͼ  
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
	HRESULT hr = CoInitialize(NULL);//��ʼ��COM��

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
	//��ѯ��Ƶ�ļ��е���Ƶ��
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
		//û���ҵ���Ƶ��
		return false;
	}

	//�����Ƶ֡��С
	AM_MEDIA_TYPE mt;
	hr = pDet->get_StreamMediaType(&mt);

	VIDEOINFOHEADER *pVih = (VIDEOINFOHEADER*)(mt.pbFormat);
	int width = pVih->bmiHeader.biWidth;
	int height = pVih->bmiHeader.biHeight;
	if (height < 0) height *= -1;

	FreeMediaType(mt);
	

	//�����Ƶ֡����
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
		//��ȡ��Ƶ֡����ʧ��
		return false;
	}

	pDet->Release();
	//printf("%d - %d*%d*3 = %d\n",buffSize,width,height,buffSize-width*height*3);
	//���벢��ʼ��jpegѹ������ͬʱָ����������
	struct jpeg_compress_struct jcs;
	//������������������ֵ��jcs.err��
	struct jpeg_error_mgr jem;
	jcs.err = jpeg_std_error(&jem);

	jpeg_create_compress(&jcs);

	//ָ��ѹ�����ͼ������ŵ�Ŀ���ļ���ע�⣬Ŀ���Զ�����ģʽ��
	FILE* f = _tfopen(picturePath,L"wb");
	if (NULL == f)
	{
		delete[] pBuffer;
		return false;
	}
	jpeg_stdio_dest(&jcs,f);

	//����ѹ����������Ҫ������ͼ����ߡ�ɫ��ͨ������1,������ͼ��3,����������ɫ�ʿռ䣨JSC_GRAYSCALE��ʾ�Ҷ�ͼ��
	//JCS_RGB��ʾ��ɫͼ�񣩣�ѹ��������,���£�
	jcs.image_width = width;
	jcs.image_height = height;
	jcs.input_components = 3;
	jcs.in_color_space = JCS_RGB;

	jpeg_set_defaults(&jcs);
	jpeg_set_quality(&jcs,80,true);
	/*jcs.scale_num = 1;	//�������������������ͼƬ��С��ԭ��ͼƬ��С�ı�����scale_numΪ���ӣ�scale_denomΪ��ĸ
	jcs.scale_denom =2;*/
	jpeg_calc_jpeg_dimensions(&jcs);
	//��Ϊ��õ�֡���ݸ�ʽ��40�ֽ�ͷ��Ϣ+BGR���У�Ҫ������RGB����
	char *pData = pBuffer+40;
	int b,r;
	volatile int tem;
	for (b=0,r=2;r<buffSize-40;b+=3,r+=3)
	{
		tem = pData[b];
		pData[b] = pData[r];
		pData[r] = tem;
	}

	//��ʼѹ��
	jpeg_start_compress(&jcs,TRUE);
	JSAMPROW row_pointer[1];
	int row_stride = jcs.image_width*3;

	while(jcs.next_scanline < jcs.image_height)
	{
		//windows�µ�������һ��һ�д��µ��ϵģ����Դ�������һ�п�ʼ��ȡ
		//row_pointer[0] =(BYTE*) & pBuffer[jcs.next_scanline*row_stride];
		row_pointer[0] = (BYTE*) &pData[(jcs.image_height-jcs.next_scanline-1)*row_stride];
		jpeg_write_scanlines(&jcs,row_pointer,1);
	}
	//ѹ�����
	jpeg_finish_compress(&jcs);
	fclose(f);
	//�ͷ�JPEGѹ������
	jpeg_destroy_compress(&jcs);
	delete []pBuffer;
	CoUninitialize();
	return true;

}
