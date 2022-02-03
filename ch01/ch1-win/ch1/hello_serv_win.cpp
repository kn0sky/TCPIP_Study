#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

int main(int argc, char* argv[]) {
	
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;
	WSADATA wsaData;
	int szClntAddr;
	char message[] = "hello selph";
	if (argc != 2) {
		printf("error usage");
		exit(1);
	}

	// winsock初始化
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))exit(1);

	// 创建socket
	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET)exit(1);

	// 填充服务端地址结构体
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	// 绑定地址和端口
	if (bind(hServSock, (const sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)exit(1);

	// 设置端口监听
	if (listen(hServSock, 5) == SOCKET_ERROR)exit(1);

	// 接收请求连接
	szClntAddr = sizeof(clntAddr);
	hClntSock = accept(hServSock, (sockaddr*)&clntAddr, &szClntAddr);
	if (hClntSock == INVALID_SOCKET)exit(1);

	// 回复信息给客户端
	send(hClntSock, message, sizeof(message), 0);
	
	// 关闭套接字句柄
	closesocket(hServSock);
	closesocket(hClntSock);

	// 卸载winsock
	WSACleanup();
	return 0;
}