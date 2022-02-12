#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

#define BUF_SIZE 1024

int main(int argc, char* argv[]) {
    WSADATA wsaData = { 0 };
    if (WSAStartup(MAKEWORD(2, 2), &wsaData)) { // WSAStartup成功返回0
        exit(1);
    }

    SOCKET hServSock, hClntSock;
    char message[BUF_SIZE];
    int len_str, i;
    SOCKADDR_IN serv_addr, clnt_addr;
    int clnt_addr_len;

    if (argc != 2) {
        printf("usage:%s <port>\n", argv[0]);
        return -1;
    }

    // 1. 创建套接字
    hServSock = socket(PF_INET, SOCK_STREAM, 0);

    // 2. 填充服务端地址信息，绑定地址到套接字
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    bind(hServSock, (const sockaddr*)&serv_addr, sizeof(serv_addr));

    // 3. 设置套接字进入可监听状态
    if (!listen(hServSock, 5)) {
        printf("Listening...\n");
    }

    // 4. 循环接收客户端请求并处理
    clnt_addr_len = sizeof(clnt_addr);
    while (true) {
        // 接收请求
        hClntSock = accept(hServSock, (sockaddr*)&clnt_addr, &clnt_addr_len);
        if (hClntSock == -1) {
            printf("accept error\n");
            continue;
        }
        printf("recv a connect requist\n");

        // 处理单个请求，如果发送的
        while (true) {
            memset(message, 0, sizeof(message) - 1);
            int read_size = recv(hClntSock, message, sizeof(message) - 1,0);
            //fputs(message,stdout);
            if (!strcmp(message, "Q\n") || !strcmp(message, "q\n")) {
                send(hClntSock, "interrupt the connection\n", 26,0);
                break;
            }
            else {
                send(hClntSock, message, read_size,0);
            }
        }
        printf("close the connection\n");
        closesocket(hClntSock);
    }

    // 5. 断开连接
    closesocket(hServSock);
    WSACleanup();

    return 0;
}