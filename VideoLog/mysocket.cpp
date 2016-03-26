#include "stdafx.h"
#include "mysocket.h"

//mysocket.h��ʵ���ļ�
SOCKET connectToServer(UINT32 ip,UINT16 port)
{
	//�����׽���
	SOCKET sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	//ASSERT(m_Sockfd != INVALID_SOCKET);
	if (INVALID_SOCKET == sockfd)
	{
		return sockfd;
	}

	//���ӷ����
	sockaddr_in servAddr;
	memset(&servAddr,0,sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = ip;
	servAddr.sin_port = port;
	if (SOCKET_ERROR == connect(sockfd,(SOCKADDR*)&servAddr,sizeof(servAddr)))
	{
		closesocket(sockfd);
		sockfd = INVALID_SOCKET;
	}
	return sockfd;
}