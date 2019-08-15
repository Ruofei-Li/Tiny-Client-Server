#ifndef __HELPER_H__
#define __HELPER_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <setjmp.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>


//默认的文件模式是DEF_MODE & ~DEF_UMASK
#define DEF_MODE   S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH
#define DEF_UMASK  S_IWGRP|S_IWOTH

typedef struct sockaddr SA;

#define RIO_BUFSIZE 8192
typedef struct {
    int rio_fd;                //内部缓冲区描述符
    int rio_cnt;               //内部缓冲区未读的字节数
    char *rio_bufptr;          //内部缓冲区中下一个未读字节
    char rio_buf[RIO_BUFSIZE]; //内部缓冲区
} rio_t;

extern int h_errno;    //DNS错误/ 
extern char **environ; 

#define	MAXLINE	 8192  //最大的文本长度（字节为单位）
#define MAXBUF   8192  //最大I/O缓冲区长
#define LISTENQ  1024  //listen函数的第二

//错误处理函数
void unix_error(char *msg);
void posix_error(int code, char *msg);
void dns_error(char *msg);
void app_error(char *msg);

//进程控制包装函数
pid_t Fork(void);
void Execve(const char *filename, char *const argv[], char *const envp[]);
pid_t Wait(int *status);
pid_t Waitpid(pid_t pid, int *iptr, int options);
void Kill(pid_t pid, int signum);
unsigned int Sleep(unsigned int secs);
void Pause(void);
unsigned int Alarm(unsigned int seconds);
void Setpgid(pid_t pid, pid_t pgid);
pid_t Getpgrp();

//Unix I/O 包装函数
int Open(const char *pathname, int flags, mode_t mode);
ssize_t Read(int fd, void *buf, size_t count);
ssize_t Write(int fd, const void *buf, size_t count);
off_t Lseek(int fildes, off_t offset, int whence);
void Close(int fd);
int Select(int  n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, 
	   struct timeval *timeout);
int Dup2(int fd1, int fd2);
void Stat(const char *filename, struct stat *buf);
void Fstat(int fd, struct stat *buf) ;

//内存映射包装函数
void *Mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset);
void Munmap(void *start, size_t length);

//socket接口包装函数
int Socket(int domain, int type, int protocol);
void Setsockopt(int s, int level, int optname, const void *optval, int optlen);
void Bind(int sockfd, struct sockaddr *my_addr, int addrlen);
void Listen(int s, int backlog);
int Accept(int s, struct sockaddr *addr, socklen_t *addrlen);
void Connect(int sockfd, struct sockaddr *serv_addr, int addrlen);

//DNS包装函数
struct hostent *Gethostbyname(const char *name);
struct hostent *Gethostbyaddr(const char *addr, int len, int type);

//Robust I/O
ssize_t rio_readn(int fd, void *usrbuf, size_t n);
ssize_t rio_writen(int fd, void *usrbuf, size_t n);
void rio_readinitb(rio_t *rp, int fd); 
ssize_t	rio_readnb(rio_t *rp, void *usrbuf, size_t n);
ssize_t	rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);

//Robust I/O包装函数
ssize_t Rio_readn(int fd, void *usrbuf, size_t n);
void Rio_writen(int fd, void *usrbuf, size_t n);
void Rio_readinitb(rio_t *rp, int fd); 
ssize_t Rio_readnb(rio_t *rp, void *usrbuf, size_t n);
ssize_t Rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);

//客户端-服务器辅助函数
int open_clientfd(char *hostname, int portno);
int open_listenfd(int portno);

//客户端-服务器辅助包装函数
int Open_clientfd(char *hostname, int port);
int Open_listenfd(int port); 

//doit函数处理一个HTTP事物
void doit(int fd);

//读取但忽略请求报头
void read_requesthdrs(rio_t *rp);

//将URI解析为一个文件名和一个可选的CGI参数字符串
int parse_uri(char *uri, char *filename, char *cgiargs);

//为客户端提供静态服务，并发送一个HTTP响应
void serve_static(int fd, char *filename, int filesize);

//根据文件名推导出文件类型
void get_filetype(char *filename, char *filetype);

//web服务器通过派生一个子进程并在子进程的上下文中运行一个CGI程序，为客户端提供动态服务
void serve_dynamic(int fd, char *filename, char *cgiargs);

//简单的错误处理函数，它会检查一些明显的错误，并报告给客户端
void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg);

#endif
