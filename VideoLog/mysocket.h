#pragma once

#include <WinSock2.h>

//�����׽��ֲ����ӷ����һ���Ը㶨�������ip��ַ�Ͷ˿ں�port�����������ֽ���ġ�
//�ɹ������׽���fd��ʧ���򷵻�INVALID_SOCKET
SOCKET connectToServer(UINT32 ip,UINT16 port);
