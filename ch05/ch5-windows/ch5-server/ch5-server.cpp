#include <iostream>
#include <functional>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

// 数据包
typedef struct _DATA {
    int op_num;     // 计算操作数数量
    char op_calc;   // 运算符类型：+ - * /
    int num_arr[0]; // 计算操作数数组
}DATA, * PDATA;

// 四则计算处理函数
template<typename Op>
int calc(int const& op_num, int* const& num_arr) {
    Op o;
    int i = 0;
    int res = num_arr[i++];
    while (i < op_num) res = o(res, num_arr[i++]);
    return res;
}

int main(int argc, char* argv[]) {
    PDATA pData;
    SOCKET serv_sock;
    SOCKET clnt_sock;
    sockaddr_in serv_addr;
    sockaddr_in clnt_addr;
    int clnt_addr_len;

    WSADATA wsaData = { 0 };
    if (WSAStartup(MAKEWORD(2, 2), &wsaData)) { // WSAStartup成功返回0
        exit(1);
    }

    // 验证参数数量
    if (argc != 2) {
        printf("usage: %s <port>", argv[0]);
        exit(1);
    }

    // 1. 创建服务端socket
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);

    // 2. 绑定地址到服务端socket
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    bind(serv_sock, (const sockaddr*)&serv_addr, sizeof(serv_addr));

    // 3. 监听端口
    listen(serv_sock, 5);
    printf("listen...\n");
    // 4. 接收请求
    clnt_addr_len = sizeof(clnt_addr);
    while (true) {
        /* code */
        clnt_sock = accept(serv_sock, (sockaddr*)&clnt_addr, &clnt_addr_len);
        if (clnt_sock == -1) {
            printf("connect failed!\n");
            continue;
        }
        else {
            printf("recv a connection\n");
        }
        // 5. 数据处理
        /*
        协议设计：
        - 客户端连接到服务端之后以1字节整数形式传递待运算数字个数
        - 客户端向服务端传递每个整数，4字节
        - 传递整数之后传递运算符信息（+ - * /），1字节
        - 服务器以4字节返回运算结果
        - 客户端得到结果就与服务端断开连接
        */

        // 接收结构体大小
        int size = 0;
        recv(clnt_sock, (char*) &size, sizeof(int), 0);
        if (size == 0) {
            printf("read info error\n");
            exit(-2);
        }
        send(clnt_sock, "OK", 3, 0);

        // 申请结构体空间 ，接收结构体
        pData = (PDATA)malloc(size);
        int read_size = 0;
        while (read_size < size) {
            read_size += recv(clnt_sock, (char*)(pData + read_size), size, 0);
        }

        // 处理运算数据
        int calc_res;
        switch (pData->op_calc)
        {
        case '+':
            calc_res = calc<std::plus<int>>(pData->op_num, pData->num_arr); break;
        case '-':
            calc_res = calc<std::minus<int>>(pData->op_num, pData->num_arr); break;
        case '*':
            calc_res = calc<std::multiplies<int>>(pData->op_num, pData->num_arr); break;
        case '/':
            calc_res = calc<std::divides<int>>(pData->op_num, pData->num_arr); break;
        default:
            calc_res = 0; break;
        }

        // 返回运算结果
        send(clnt_sock, (const char*)&calc_res, sizeof(int), 0);

        free(pData);
        printf("close connection\n");
        closesocket(clnt_sock);
    }

    // 6. 关闭连接
    closesocket(serv_sock);
    WSACleanup();
    return 0;
}