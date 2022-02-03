#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>

int main(int argc,char* argv[]){
    int fd;
    char w_buf[]="Let's it go o o!\n";
    char r_buf[64]={0};

    // 以可读可写，不存在就创建的方式打开文件
    fd = open("data.txt",O_CREAT | O_RDWR | O_TRUNC);
    if(fd == -1)exit(1);

    printf("File Descriptor:%d\n",fd);

    // 写入buf到文件，会修改文件指
    if(write(fd,w_buf,sizeof(w_buf)) == -1)exit(1);

    // 移动文件指针到文件头
    lseek(fd,0,SEEK_SET);

    // 读取文件内容并打印
    if(read(fd,r_buf,sizeof(r_buf)) == -1)exit(1);
    printf("%s\n",r_buf);    

    close(fd);
    return 0;
}