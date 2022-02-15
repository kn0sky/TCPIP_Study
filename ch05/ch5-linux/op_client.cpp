#include <iostream>
#include <functional>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>


typedef struct _DATA{
    int op_num;
    char op_calc;
    int num_arr[0];
}DATA,*PDATA;

int main(int argc,char* argv[]){
    int sock;
    sockaddr_in sock_addr;        
    PDATA pData;
    int pData_size=0;

    if(argc != 3){
        printf("usage: %s <ip> <port>\n",argv[0]);
        exit(-1);
    }

    // 1. 创建socket
    sock = socket(PF_INET,SOCK_STREAM,0);

    // 2. 连接服务端
    memset(&sock_addr,0,sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = inet_addr(argv[1]);
    sock_addr.sin_port = htons(atoi(argv[2]));

    connect(sock,(const sockaddr*)&sock_addr,sizeof(sock_addr));

    // 3. 数据交互
    /*
        协议设计：
        - 客户端连接到服务端之后以1字节整数形式传递待运算数字个数
        - 客户端向服务端传递每个整数，4字节
        - 传递整数之后传递运算符信息（+ - * /），1字节
        - 服务器以4字节返回运算结果
        - 客户端得到结果就与服务端断开连接
    */

    // 获取运算数数量
    int op_num = 0;
    fputs("type op num:",stdout);
    op_num = fgetc(stdin) - '0';
    
    // 给通信结构体申请空间
    pData_size = sizeof(DATA)+op_num*sizeof(int);
    pData = (PDATA)malloc(pData_size);
    pData->op_num = op_num;

    // 依次输入操作数，保存到结构体里
    for (uint8_t i = 0; i < op_num; i++){
        printf("type the op %d: ",i);
        int buf=0;
        std::cin >> buf;
        pData->num_arr[i] = buf;
    }

    // 获取运算符，填充结构体
    char calc_op;
    std::cout << "type a calc type: ";
    std::cin >> calc_op;
    pData->op_calc = calc_op;

    // 发送结构体（先发大小，然后再发数据）
    write(sock,&pData_size,sizeof(pData_size));
    char readbuf[10];
    read(sock,readbuf,3);
    if(!strncmp(readbuf,"OK",2)){
        write(sock,pData,pData_size);
    }else{
        fputs("Server ERROR",stdout);
    }

    // 接收返回值
    int res;
    read(sock,&res,sizeof(res));
    std::cout << "res is "<< res<< std::endl;
    
    free(pData);
    close(sock);
    return 0;
}