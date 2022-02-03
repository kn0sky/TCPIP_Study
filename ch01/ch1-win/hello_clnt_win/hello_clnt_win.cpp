#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")

int main(int argc, char* argv[]) {
	
	SOCKET hSock;
	SOCKADDR_IN servAddr, clntAddr;
	WSADATA wsaData;
	int szClntAddr;
	int str_len;
	char message[64] = { 0 };
	
	if (argc != 3) {
		exit(1);
	}

	// winsock初始化
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))exit(1);

	// 创建socket
	hSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hSock == INVALID_SOCKET)exit(1);

	// 填充服务端地址结构体
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.S_un.S_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	// 向服务器发起连接
	if(connect(hSock, (const sockaddr*)&servAddr, sizeof(sockaddr)) == SOCKET_ERROR)exit(1);

	// 接收信息
	str_len = recv(hSock, message, sizeof(message) - 1, 0);
	if (str_len == -1)exit(1);
	printf("Msg From Server: %s\n",message);
	
	
	// 关闭套接字句柄
	closesocket(hSock);

	// 卸载winsock
	WSACleanup();
	return 0;
}
