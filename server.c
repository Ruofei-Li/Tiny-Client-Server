/*
    Tiny-Client-Server 是一个虽小但功能齐全的Web服务器，结合了进程控制、Unix I/O、套接字接口和HTTP。
    它缺乏一个实际服务器所具有的功能性、健壮性和安全性，但是它足够用来为实际的Web浏览器提供静态和动态的内容。
    Tiny-Client-Server 是一个迭代服务器，监听在命令行中传递来的端口上的连接请求。
    通过函数调用打开一个监听套接字以后，服务器执行无限循环，不断地接受连接请求，执行HTTP事物，开关闭连接的它那一端。
*/

//包含一些用到的变量和辅助函数
#include "helper.h"


int main(int argc, char **argv) 
{
    //listenfd监听描述符，connfd：连接描述符，port：输入指定端口号
    int listenfd, connfd, port, clientlen;
    //sockaddr_in是在netinet/in.h头文件中声明的套接字地址结构体
    struct sockaddr_in clientaddr;
    
    //检查输入参数，正确的函数调用方式：./server 1025
    //注意：0-1024端口号是系统端口，1025-65534是用户可以指定的端口号，65535是系统保留
    if (argc != 2) 
    {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }
    //atoi是把字符串转换成整型数的一个函数，包含在stdlib.h头文件中
    port = atoi(argv[1]);

    //在port端口上打开并返回一个监听套接字描述符
    listenfd = Open_listenfd(port);

    //无限循环接受来自客户端的连接请求，执行事物完后主动关闭连接。在没有连接请求时，服务器阻塞在Accept函数
    while (1) 
    {
        clientlen = sizeof(clientaddr);
        //Accept函数调用accept函数来等待凯子客户端的连接请求
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen); 
        //执行HTTP事物
        doit(connfd);
        //关闭连接                                          
        Close(connfd); 
    }
}
