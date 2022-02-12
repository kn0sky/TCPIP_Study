﻿#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

#define BUF_SIZE 1024

int main(int argc, char* argv[]) {
    SOCKET hSock;
    SOCKADDR_IN serv_addr;

    WSADATA wsaData = { 0 };
    if (WSAStartup(MAKEWORD(2, 2), &wsaData)) { // WSAStartup成功返回0
        exit(1);
    }

    if (argc != 3) {
        printf("usage:%s <ip-address> <port>\n", argv[0]);
        return -1;
    }

    // 1.创建socket    
    hSock = socket(PF_INET, SOCK_STREAM, 0);

    // 2. 绑定socket信息
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    // 3. 连接服务端
    int ret = connect(hSock, (const sockaddr*)&serv_addr, sizeof(serv_addr));
    if (ret == -1) {
        printf("connect failed!\n");
        return -1;
    }
    printf("connect success!\n");

    // 4. 数据交换
    int read_size = 0;
    while (true) {
        char msg_write[BUF_SIZE] = { 0 };
        char msg_read[BUF_SIZE] = { 0 };

        // 输出提示信息 + 读取输入（注意，这里scanf不行）
        fputs("Input message(Q to quit) :", stdout);
        fgets(msg_write, sizeof(msg_write) - 1, stdin);

        // 将获取到的输入发送给服务器
        send(hSock, msg_write, strlen(msg_write),0);

        // 从服务器那里读取数据
        read_size = recv(hSock, msg_read, sizeof(msg_read) - 1,0);
        msg_read[read_size] = 0;

        // 打印收到的信息
        printf("msg from server : %s", msg_read);

        // 如果发送了q，则断开连接
        if (!strcmp("q\n", msg_write) || !strcmp("Q\n", msg_write)) {
            break;
        }
    }

    // 5. 断开连接
    closesocket(hSock);
    WSACleanup();

    return 0;
}