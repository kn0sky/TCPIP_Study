#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);

// 套接字由socket函数生成，成功时返回文件描述符，失败返回-1
// int socket(int domain, int type, int protocol);

// 发起套接字连接，成功返回0，失败返回-1
// int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen); 

/*
客户端向服务端发起连接流程：
1. 调用socket函数创建套接字
2. 调用connect函数向服务器发起请求
*/


int main(int argc, char *argv[])
{
    int sock;
    int str_len=0;
    int idx=0, read_len = 0;    
    sockaddr_in serv_addr;

    char message[30] ={0};

    if (argc != 3)
    {
        printf("Usage: %s <IP> <port>\n", argv[0]);
    }

    // 创建socket
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)error_handling("socket() error");

    // 填充服务端绑定地址结构
    memset(&serv_addr, 0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    // 向服务端发起请求
    if(connect(sock,(const sockaddr*)&serv_addr,sizeof(serv_addr)) == -1){
        error_handling("connect() error");
    }

    puts("Connecting...");

    // 从套接字读取内容
    while(read_len = read(sock, &message[idx++],1)){
        if(read_len == -1)error_handling("read() error");
        str_len += read_len; 
    }

    printf("Msessage From Server(%d byte): %s\n",str_len,message);
    printf("Call read() times is : %d\n",str_len);
    // 关闭套接字
    close(sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
