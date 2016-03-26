#include "stdafx.h"
#include "UpDownProThreadFunc.h"
#include "INIManager.h"
#include <stdio.h>
#include <string.h>
#include <WinSock2.h>

#pragma comment(lib,"Ws2_32.lib")

DWORD WINAPI UpDownProThreadFunc(LPVOID lpParam)
{
	PAR_TO_PROTHREAD *par = (PAR_TO_PROTHREAD*)lpParam;
	CProgressCtrl *pro = par->pPro;
	TCHAR *filename = par->filename;
	extern HWND upDownDlghwnd;
	//initialize winsock
	WSADATA wsaData;
	int rs;
	if ((rs = WSAStartup(MAKEWORD(2,0),&wsaData)) != NOERROR)
	{
		MessageBox(NULL,L"WSAStartup failed",L"error",MB_OK);
		::SendMessage(upDownDlghwnd,WM_UPOVER,(WPARAM)(pro->m_hWnd),0);
		delete par;
		return -1;
	}
	//create  socket
	SOCKET sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (INVALID_SOCKET == sockfd)
	{
		MessageBox(NULL,L"socket error",L"error",MB_OK);
		WSACleanup();
		::SendMessage(upDownDlghwnd,WM_UPOVER,(WPARAM)pro->m_hWnd,0);
		delete par;
		return -1;
	}
	//connect sockfd to server
	CINIManager* pINI = CINIManager::GetINIManager();
	sockaddr_in servAddr;
	ZeroMemory(&servAddr,sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	//servAddr.sin_addr.s_addr = inet_addr("121.199.24.119");
	servAddr.sin_addr.s_addr = pINI->GetServerIP();
	servAddr.sin_port = pINI->GetServicesPort();
	if (SOCKET_ERROR == connect(sockfd,(SOCKADDR*)&servAddr,sizeof(servAddr)))
	{
		AfxMessageBox(L"上传文件连接网络失败");
		//printf("connect failed with error:%d\n",WSAGetLastError());
		closesocket(sockfd);
		WSACleanup();
		::SendMessage(upDownDlghwnd,WM_UPOVER,(WPARAM)(pro->m_hWnd),0);
		delete par;
		return -1;
	}
	//连接成功

	//发送事务类型：上传文件UP
	send(sockfd,"UP",strlen("UP"),0);
	//等待服务器的应答后，才能进行下一步
	char tem[4] = {'\0'};
	recv(sockfd,tem,4,0);
	if (strcmp("yes",tem) != 0)
	{
		closesocket(sockfd);
		WSACleanup();
		::SendMessage(upDownDlghwnd,WM_UPOVER,(WPARAM)(pro->m_hWnd),0);
		delete par;
		return -1;
	}
	//告知文件类型和文件名:"video",$filename或者"image",$filename
	if (filename[_tcslen(filename)-1] == L'i')
	{
		//avi视频文件
		
		char buf[25] = {'\0'};
		sprintf(buf,"%s,%s","video",GetVideocharFileName());
		send(sockfd,buf,strlen(buf),0);
	}
	else if (filename[_tcslen(filename)-1] == L'g')
	{
		//jpg图像文件
		char buf[25] = {'\0'};
		sprintf(buf,"%s,%s","image",GetImagecharFileName());
		send(sockfd,buf,strlen(buf),0);
	}

	//open a file and transfer it to serv 
	FILE *fp = _tfopen(filename,L"rb");
	if (NULL == fp)
	{
		//perror("fopen  file fialed!\n");
		MessageBox(NULL,L"打开文件失败！",L"error",MB_OK);
		closesocket(sockfd);
		WSACleanup();
		::SendMessage(upDownDlghwnd,WM_UPOVER,(WPARAM)(pro->m_hWnd),0);
		delete par;
		return -1;
	}
	long filesize = 0;
	if (fseek(fp,0,SEEK_END) == 0)
	{
		filesize = ftell(fp);
	}
	else
	{
		MessageBox(NULL,L"获得文件长度失败！",L"error",MB_OK);
	}
	rewind(fp);
	pro->SetRange32(0,filesize);
	pro->SetStep(1024);
	//printf("now start tranfer file to server..\n");
	int readlen;
	char buf[1024];
	while(TRUE)
	{
		ZeroMemory(buf,1024);
		readlen = fread(buf,sizeof(char),1024,fp);
		send(sockfd,buf,readlen,0);
		if (readlen < 1024 && feof(fp) != 0 )
		{
			//printf("file tranfer over!\n");
		//	MessageBox(NULL,L"file tranfer over",L"OK",MB_OK);
			break;
		}
		
		pro->StepIt();
		if (ferror(fp) != 0)
		{
			//perror("read file error!");
			MessageBox(NULL,L"read file error",L"error",MB_OK);
			break;
		}
	}
	fclose(fp);
	closesocket(sockfd);
	WSACleanup();

	delete par;
	::SendMessage(par->hWnd,WM_UPOVER,(WPARAM)(pro->m_hWnd),0);
	
	return 0;
}
