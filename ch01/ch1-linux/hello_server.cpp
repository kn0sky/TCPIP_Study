#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);

// 套接字由socket函数生成，成功时返回文件描述符，失败返回-1
// int socket(int domain, int type, int protocol);

// 给套接字分配地址信息，成功返回0，失败返回-1
// int bind(int sockfd, const struct sockaddr *addr,socklen_t addrlen);

// 将套接字转化为可接收连接的状态，成功返回0，失败返回-1
// int listen(int sockfd, int backlog);

// 接收套接字连接请求，成功返回文件描述符，失败返回-1
// int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

// 发起套接字连接，成功返回0，失败返回-1
// int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen); 

/*
创建接收请求的套接字流程如下：
1. 调用socket函数创建套接字
2. 调用bind函数分配端口号和地址
3. 调用listen函数将套接字转换为可接受连接状态
4. 调用accept函数接收连接请求
*/

/*
客户端向服务端发起连接流程：
1. 调用socket函数创建套接字
2. 调用connect函数向服务器发起请求
*/


int main(int argc, char *argv[])
{
    int serv_sock;
    int clnt_sock;

    sockaddr_in serv_addr;
    sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    char* message = "hello ,there is selph!";

    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
    }

    // 创建服务端socket
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)error_handling("socket() error");

    // 填充服务端绑定地址结构
    memset(&serv_addr, 0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    // 绑定地址
    if(bind(serv_sock,(const sockaddr *)&serv_addr,sizeof(serv_addr)) == -1){    
        error_handling("bind() error");
    }

    // 监听地址和端口
    if(listen(serv_sock,5) == -1){
        error_handling("listen() error");
    }

    puts("Server is listening...");

    // 接收客户端请求套接字
    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock,(sockaddr*)&clnt_addr,&clnt_addr_size);
    if(serv_sock == -1)error_handling("accept() error");

    // 向套接字写入内容
    write(clnt_sock, message,strlen(message));

    // 关闭创建的套接字
    close(serv_sock);
    close(clnt_sock);
    
    return 0;
}

void error_handling(char *message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
