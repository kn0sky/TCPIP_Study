#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

int main(int argc,char* argv[]){
    int serv_sock,clnt_sock;
    char message[BUF_SIZE];
    int len_str,i;

    sockaddr_in serv_addr,clnt_addr;
    socklen_t clnt_addr_len;

    if(argc != 2){
        printf("usage:%s <port>\n", argv[0]);
        return -1;
    }

    // 1. 创建套接字
    serv_sock = socket(PF_INET,SOCK_STREAM,0);

    // 2. 填充服务端地址信息，绑定地址到套接字
    memset(&serv_addr,0,sizeof(serv_addr));    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    bind(serv_sock,(const sockaddr*)&serv_addr,sizeof(serv_addr));

    // 3. 设置套接字进入可监听状态
    if(!listen(serv_sock,5)){
        printf("Listening...\n");
    }

    // 4. 循环接收客户端请求并处理
    clnt_addr_len = sizeof(clnt_addr);
    while(true){
        // 接收请求
        clnt_sock = accept(serv_sock,(sockaddr*)&clnt_addr,&clnt_addr_len);
        if(clnt_sock == -1){
            printf("accept error\n");
            continue;
        }
        printf("recv a connect requist\n");

        // 处理单个请求，如果发送的
        while (true){
            memset(message,0,sizeof(message)-1);            
            int read_size = read(clnt_sock,message,sizeof(message)-1);
            //fputs(message,stdout);
            if(!strcmp(message,"Q\n") || !strcmp(message,"q\n")){
                write(clnt_sock,"interrupt the connection\n",26);
                break;
            }else{
                write(clnt_sock,message,read_size);
            }
        }
        printf("close the connection\n");
        close(clnt_sock);
    }

    // 5. 断开连接
    close(serv_sock);

    return 0;
}