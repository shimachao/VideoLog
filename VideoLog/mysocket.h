#pragma once

#include <WinSock2.h>

//创建套接字并连接服务端一次性搞定，传入的ip地址和端口号port必须是网络字节序的。
//成功返回套接字fd，失败则返回INVALID_SOCKET
SOCKET connectToServer(UINT32 ip,UINT16 port);
