#include "helper.h"

//错误处理函数
void unix_error(char *msg) 
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);
}

void posix_error(int code, char *msg)
{
    fprintf(stderr, "%s: %s\n", msg, strerror(code));
    exit(0);
}

void dns_error(char *msg)
{
    fprintf(stderr, "%s: DNS error %d\n", msg, h_errno);
    exit(0);
}

void app_error(char *msg) 
{
    fprintf(stderr, "%s\n", msg);
    exit(0);
}



//进程控制包装函数
pid_t Fork(void) 
{
    pid_t pid;

    if ((pid = fork()) < 0)
	unix_error("Fork error");
    return pid;
}

void Execve(const char *filename, char *const argv[], char *const envp[]) 
{
    if (execve(filename, argv, envp) < 0)
	unix_error("Execve error");
}

pid_t Wait(int *status) 
{
    pid_t pid;

    if ((pid  = wait(status)) < 0)
	unix_error("Wait error");
    return pid;
}

pid_t Waitpid(pid_t pid, int *iptr, int options) 
{
    pid_t retpid;

    if ((retpid  = waitpid(pid, iptr, options)) < 0) 
	unix_error("Waitpid error");
    return(retpid);
}

void Kill(pid_t pid, int signum) 
{
    int rc;

    if ((rc = kill(pid, signum)) < 0)
	unix_error("Kill error");
}

void Pause() 
{
    (void)pause();
    return;
}

unsigned int Sleep(unsigned int secs) 
{
    unsigned int rc;

    if ((rc = sleep(secs)) < 0)
	unix_error("Sleep error");
    return rc;
}

unsigned int Alarm(unsigned int seconds) {
    return alarm(seconds);
}
 
void Setpgid(pid_t pid, pid_t pgid) {
    int rc;

    if ((rc = setpgid(pid, pgid)) < 0)
	unix_error("Setpgid error");
    return;
}

pid_t Getpgrp(void) {
    return getpgrp();
}



//Unix I/O 包装函数
int Open(const char *pathname, int flags, mode_t mode) 
{
    int rc;

    if ((rc = open(pathname, flags, mode))  < 0)
	unix_error("Open error");
    return rc;
}

ssize_t Read(int fd, void *buf, size_t count) 
{
    ssize_t rc;

    if ((rc = read(fd, buf, count)) < 0) 
	unix_error("Read error");
    return rc;
}

ssize_t Write(int fd, const void *buf, size_t count) 
{
    ssize_t rc;

    if ((rc = write(fd, buf, count)) < 0)
	unix_error("Write error");
    return rc;
}

off_t Lseek(int fildes, off_t offset, int whence) 
{
    off_t rc;

    if ((rc = lseek(fildes, offset, whence)) < 0)
	unix_error("Lseek error");
    return rc;
}

void Close(int fd) 
{
    int rc;

    if ((rc = close(fd)) < 0)
	unix_error("Close error");
}

int Select(int  n, fd_set *readfds, fd_set *writefds,
	   fd_set *exceptfds, struct timeval *timeout) 
{
    int rc;

    if ((rc = select(n, readfds, writefds, exceptfds, timeout)) < 0)
	unix_error("Select error");
    return rc;
}

int Dup2(int fd1, int fd2) 
{
    int rc;

    if ((rc = dup2(fd1, fd2)) < 0)
	unix_error("Dup2 error");
    return rc;
}

void Stat(const char *filename, struct stat *buf) 
{
    if (stat(filename, buf) < 0)
	unix_error("Stat error");
}

void Fstat(int fd, struct stat *buf) 
{
    if (fstat(fd, buf) < 0)
	unix_error("Fstat error");
}



//内存映射包装函数
void *Mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset) 
{
    void *ptr;

    if ((ptr = mmap(addr, len, prot, flags, fd, offset)) == ((void *) -1))
	unix_error("mmap error");
    return(ptr);
}

void Munmap(void *start, size_t length) 
{
    if (munmap(start, length) < 0)
	unix_error("munmap error");
}



//socket接口包装函数
int Socket(int domain, int type, int protocol) 
{
    int rc;

    if ((rc = socket(domain, type, protocol)) < 0)
	unix_error("Socket error");
    return rc;
}

void Setsockopt(int s, int level, int optname, const void *optval, int optlen) 
{
    int rc;

    if ((rc = setsockopt(s, level, optname, optval, optlen)) < 0)
	unix_error("Setsockopt error");
}

void Bind(int sockfd, struct sockaddr *my_addr, int addrlen) 
{
    int rc;

    if ((rc = bind(sockfd, my_addr, addrlen)) < 0)
	unix_error("Bind error");
}

void Listen(int s, int backlog) 
{
    int rc;

    if ((rc = listen(s,  backlog)) < 0)
	unix_error("Listen error");
}

int Accept(int s, struct sockaddr *addr, socklen_t *addrlen) 
{
    int rc;
    //服务器通过调用accept函数等待来自客户端的请求
    //客户端的连接请求到达监听描述符s,然后在addr中填写客户端的套接字地址，
    //并返回一个已连接描述符，这个描述符可被用来利用Unix I/O函数与客户端通信。
    if ((rc = accept(s, addr, addrlen)) < 0)
	unix_error("Accept error");
    return rc;
}

void Connect(int sockfd, struct sockaddr *serv_addr, int addrlen) 
{
    int rc;
    if ((rc = connect(sockfd, serv_addr, addrlen)) < 0)
	unix_error("Connect error");
}



//DNS包装函数
struct hostent *Gethostbyname(const char *name) 
{
    struct hostent *p;

    if ((p = gethostbyname(name)) == NULL)
	dns_error("Gethostbyname error");
    return p;
}

struct hostent *Gethostbyaddr(const char *addr, int len, int type) 
{
    struct hostent *p;

    if ((p = gethostbyaddr(addr, len, type)) == NULL)
	dns_error("Gethostbyaddr error");
    return p;
}



//Robust I/O
ssize_t rio_readn(int fd, void *usrbuf, size_t n) 
{
    size_t nleft = n;
    ssize_t nread;
    char *bufp = usrbuf;

    while (nleft > 0) {
	if ((nread = read(fd, bufp, nleft)) < 0) {
	    if (errno == EINTR) 
		nread = 0;      
	    else
		return -1;      
	} 
	else if (nread == 0)
	    break;          
	nleft -= nread;
	bufp += nread;
    }
    return (n - nleft); 
}

//rio_writen从usrbuf处传送n个字节到fd处
ssize_t rio_writen(int fd, void *usrbuf, size_t n) 
{
    size_t nleft = n;
    ssize_t nwritten;
    char *bufp = usrbuf;

    while (nleft > 0) {
	if ((nwritten = write(fd, bufp, nleft)) <= 0) {
	    if (errno == EINTR)  
		nwritten = 0;   
	    else
		return -1;    
	}
	nleft -= nwritten;
	bufp += nwritten;
    }
    return n;
}

static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n)
{
    int cnt;

    while (rp->rio_cnt <= 0) 
    {  
        //read函数从描述符为rp->rio_fd的当前文件位置复制最多sizeof(rp->rio_buf)个字节到内存位置rp->rio_buf
        //返回值-1表示一个错误，返回值0表示EOF，否则返回实际传送的字节数量。
    	rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));
    	if (rp->rio_cnt < 0) 
        {
    	    if (errno != EINTR) 
        		return -1;
	    }
	    else if (rp->rio_cnt == 0)  //EOF
            return 0;
        else 
            rp->rio_bufptr = rp->rio_buf; 
    }

    //从rp->rio_bufptr中复制min(n, rp->rio_cnt)个字节到usrbuf
    cnt = n;          
    if (rp->rio_cnt < n)   
        cnt = rp->rio_cnt;
    memcpy(usrbuf, rp->rio_bufptr, cnt);
    rp->rio_bufptr += cnt;
    rp->rio_cnt -= cnt;
    return cnt;
}

void rio_readinitb(rio_t *rp, int fd) 
{
    rp->rio_fd = fd;  
    rp->rio_cnt = 0;  
    rp->rio_bufptr = rp->rio_buf;
}

ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n) 
{
    size_t nleft = n;
    ssize_t nread;
    char *bufp = usrbuf;
    
    while (nleft > 0) {
	if ((nread = rio_read(rp, bufp, nleft)) < 0) 
            return -1;         
	else if (nread == 0)
	    break;              
	nleft -= nread;
	bufp += nread;
    }
    return (n - nleft);     
}

ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen) 
{
    int n, rc;
    char c, *bufp = usrbuf;

    for (n = 1; n < maxlen; n++) { 
        if ((rc = rio_read(rp, &c, 1)) == 1) {
	    *bufp++ = c;
	    if (c == '\n') {
                n++;
     		break;
            }
	} else if (rc == 0) {
	    if (n == 1)
		return 0; 
	    else
		break;   
	} else
	    return -1;	 
    }
    *bufp = 0;
    return n-1;
}



//Robust I/O包装函数
ssize_t Rio_readn(int fd, void *ptr, size_t nbytes) 
{
    ssize_t n;
  
    if ((n = rio_readn(fd, ptr, nbytes)) < 0)
	unix_error("Rio_readn error");
    return n;
}

void Rio_writen(int fd, void *usrbuf, size_t n) 
{
    if (rio_writen(fd, usrbuf, n) != n)
	unix_error("Rio_writen error");
}

void Rio_readinitb(rio_t *rp, int fd)
{
    rio_readinitb(rp, fd);
} 

ssize_t Rio_readnb(rio_t *rp, void *usrbuf, size_t n) 
{
    ssize_t rc;

    if ((rc = rio_readnb(rp, usrbuf, n)) < 0)
	unix_error("Rio_readnb error");
    return rc;
}

ssize_t Rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen) 
{
    ssize_t rc;

    if ((rc = rio_readlineb(rp, usrbuf, maxlen)) < 0)
	unix_error("Rio_readlineb error");
    return rc;
} 



//客户端-服务器辅助函数
//客户端辅助函数：open_clientfd函数打开到服务<hostname,port>的连接，返回一个套接字描述符准备读写。
int open_clientfd(char *hostname, int port) 
{
    int clientfd;
    struct hostent *hp;
    struct sockaddr_in serveraddr;

    if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	   return -1; 

    if ((hp = gethostbyname(hostname)) == NULL)
	   return -2; 
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)hp->h_addr_list[0], (char *)&serveraddr.sin_addr.s_addr, hp->h_length);
    serveraddr.sin_port = htons(port);

    //向serveraddr地址的服务器请求建立连接
    if (connect(clientfd, (SA *) &serveraddr, sizeof(serveraddr)) < 0)
	   return -1;
    return clientfd;
}

//服务器辅助函数：open_listenfd函数在port端口上打开并返回一个监听套接字
int open_listenfd(int port) 
{
    int listenfd, optval=1;
    struct sockaddr_in serveraddr;
  
    //创建socket监听描述符，AF_INET表明使用32位IP地址，SOCK_STREAM表明这个套接字是连接的一个端点
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	   return -1;
 
    //消除绑定时“Address already in use”错误
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int)) < 0)
	   return -1;

    //将serveraddr所有位置0，然后对其赋值
    bzero((char *) &serveraddr, sizeof(serveraddr));

    //固定为AF_INET，表示使用32位IP地址
    serveraddr.sin_family = AF_INET; 

    //INADDR_ANY就是0.0.0.0，本机地址的意思，htonl函数是将主机字节序转化成网络字节序
    //不同的主机采用不同的字节存储顺序（大端法和小端法），网络字节序统一用大端法
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); 

    //设定端口号，htons函数与htonl函数作用类似，只不过l表示long型，s表示short型
    serveraddr.sin_port = htons((unsigned short)port);

    //bind函数告诉内核将serveraddr中的服务器套接字地址和套接字描述符listenfd联系起来
    if (bind(listenfd, (SA *)&serveraddr, sizeof(serveraddr)) < 0)
	   return -1;

    //listen函数将listenfd从一个主动套接字转化为一个监听套接字，准备接受连接请求
    if (listen(listenfd, LISTENQ) < 0)
	   return -1;
    return listenfd;
}



//客户端-服务器辅助包装函数
int Open_clientfd(char *hostname, int port) 
{
    int rc;

    if ((rc = open_clientfd(hostname, port)) < 0) {
	if (rc == -1)
	    unix_error("Open_clientfd Unix error");
	else        
	    dns_error("Open_clientfd DNS error");
    }
    return rc;
}

int Open_listenfd(int port) 
{
    int rc;

    if ((rc = open_listenfd(port)) < 0)
	unix_error("Open_listenfd error");
    return rc;
}



/*
    doit函数处理一个HTTP事物。首先，我们使用Rio_readlineb函数读和解析请求行。
    doit函数支持GET方法，如果客户端请求其他方法（比如POST），该程序会给它发送一个错误信息，并返回主程序，
    主程序随后关闭连接并等待下一个连接请求。否则，我们读并且忽略任何请求报头。
    然后，我们将URI解析为一个文件名和一个可能为空的CGI参数字符串，并设置一个标志，表明请求的是静态内容还是动态内容。
    如果文件在磁盘上不存在，我们立即发送一个错误信息给客户端并返回。
    最后，如果请求的是静态内容，我们就验证该文件是一个普通文件，而我们是有读权限的。如果是这样，
    我们就向客户端提供静态内容。如果请求的是动态内容，我们就验证该文件是可执行文件，如果是，就提供动态内容。
*/
void doit(int fd) 
{
    int is_static;
    struct stat sbuf;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char filename[MAXLINE], cgiargs[MAXLINE];
    rio_t rio;
  
    
    //Rio_readinitb将描述符fd和地址rio处的一个类型为rio_t的读缓冲区联系起来
    Rio_readinitb(&rio, fd);

    //Rio_readlineb从文件rio独处下一个文本行，将它复制到内存位置buf，最多读取MAXLINE个字节，超出的被截断
    Rio_readlineb(&rio, buf, MAXLINE);                   

    //解析参数，只支持get方法，否则报错
    sscanf(buf, "%s %s %s", method, uri, version);       
    if (strcasecmp(method, "GET")) {                     
       clienterror(fd, method, "501", "Not Implemented",
                "Tiny does not implement this method");
        return;
    }                                                    
    read_requesthdrs(&rio);                              
    
    is_static = parse_uri(uri, filename, cgiargs); 
    //stat函数获取文件信息      
    if (stat(filename, &sbuf) < 0) {                     
    clienterror(fd, filename, "404", "Not found",
            "Tiny couldn't find this file");
    return;
    }                                                    

    if (is_static) { 
    if (!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode)) { 
        clienterror(fd, filename, "403", "Forbidden",
            "Tiny couldn't read the file");
        return;
    }
    serve_static(fd, filename, sbuf.st_size);        
    }
    else { 
    if (!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode)) { 
        clienterror(fd, filename, "403", "Forbidden",
            "Tiny couldn't run the CGI program");
        return;
    }
    serve_dynamic(fd, filename, cgiargs);            
    }
}


/*
    read_requesthdrs函数不使用请求报头中的任何信息，它只是用来读取并忽略这些报头。
    注意：终止请求报头的空文本行是由回车和换行符对组成的。
*/
void read_requesthdrs(rio_t *rp) 
{
    char buf[MAXLINE];

    Rio_readlineb(rp, buf, MAXLINE);
    //请求报头以回车和换行符结束
    while(strcmp(buf, "\r\n")) 
    {         
        Rio_readlineb(rp, buf, MAXLINE);
        printf("%s", buf);
    }
    return;
}


/*
    为了方便，Tiny-Client-Server假设静态内容的主目录就是它的当前目录，而可执行文件的主目录是./cgi-bin，
    任何包含字符串cgi-bin的URI都会被认为表示的是对动态内容的请求。默认的文件名是./home.html。
    parse_uri函数将URI解析为一个文件名和一个可选的CGI参数字符串。如果请求的是静态内容，我们将清除CGI参数字符串，
    然后将URI转换为一个Linux相对路径名。如果请求的是动态内容，我们就会抽取出所有的CGI参数，
    并将URI剩下的部分转换为一个Linux相对文件名。
*/
int parse_uri(char *uri, char *filename, char *cgiargs) 
{
    char *ptr;

    if (!strstr(uri, "cgi-bin")) { //静态请求
    strcpy(cgiargs, "");         //清除cgi参数                   
    strcpy(filename, ".");       //清除文件名                   
    strcat(filename, uri);       //将uri赋值给文件名                   
    if (uri[strlen(uri)-1] == '/')   //uri以/结尾，则默认访问home.html               
        strcat(filename, "home.html");              
    return 1;
    }
    else {  //动态请求  
    ptr = index(uri, '?');         
    if (ptr) {
        strcpy(cgiargs, ptr+1);
        *ptr = '\0';
    }
    else 
        strcpy(cgiargs, "");                         
    strcpy(filename, ".");                           
    strcat(filename, uri);                           
    return 0;
    }
}


/*
    Tiny-Client-Server提供五种常见类型的静态内容：HTML文件、无格式的文本文件、编码为GIF、PNG、JPG格式的图片。
    serve_static函数发送一个HTTP响应，其主体包含一个本地文件的内容。首先，我们通过检查文件名的后缀来判断文件类型，
    并且发送响应行和响应报头给客户端。接着，我们将被请求的文件的内容复制到已连接描述符fd来发送响应主体。
    这里调用mmap函数将被请求的文件映射到一个虚拟内存空间。
*/
void serve_static(int fd, char *filename, int filesize) 
{
    int srcfd;
    char *srcp, filetype[MAXLINE], buf[MAXBUF];
 
    get_filetype(filename, filetype);       
    sprintf(buf, "HTTP/1.0 200 OK\r\n");    
    sprintf(buf, "%sServer: Tiny Web Server\r\n", buf);
    sprintf(buf, "%sContent-length: %d\r\n", buf, filesize);
    sprintf(buf, "%sContent-type: %s\r\n\r\n", buf, filetype);
    Rio_writen(fd, buf, strlen(buf));      

    //只读的方式打开文件，返回描述符数字
    srcfd = Open(filename, O_RDONLY, 0);    
    //Mmap函数通过调用mmap函数将srcfd文件的前filesize个字节映射到一个从srcp开始的私有只读虚拟内存空间
    srcp = Mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);
    Close(srcfd);                          
    Rio_writen(fd, srcp, filesize);  
    //释放映射的虚拟内存空间      
    Munmap(srcp, filesize);               
}


/*
    根据文件名推导出文件类型
*/
void get_filetype(char *filename, char *filetype) 
{
    if (strstr(filename, ".html"))
    strcpy(filetype, "text/html");
    else if (strstr(filename, ".gif"))
    strcpy(filetype, "image/gif");
    else if (strstr(filename, ".jpg"))
    strcpy(filetype, "image/jpeg");
    else
    strcpy(filetype, "text/plain");
}  


/*
    Tiny-Client-Server通过派生一个子进程并在子进程的上下文中运行一个CGI程序，来提供各种类型的动态内容。
    serve_dynamic函数一开始就向客户端发送一个表明成功的响应行，同时还包括带有信息的server报头，
    CGI程序负责发送响应的剩余部分。在发送了响应的第一部分后，我们会派生一个新的子进程，
    子进程用来自请求的URI的CGI参数初始化QUERY_STRING环境变量。接下来，子进程重定向它的标准输出到已连接文件描述符，
    然后加载并运行CGI程序。因为CGI程序运行在子进程的上下文中，它能够访问所有在调用execve函数之前就存在的打开文件和环境变量。
    因此，CGI程序写到标准输出上的任何东西都将直接送到客户端进程，不会收到任何来自父进程的干涉。
    其间，父进程阻塞在对wait的调用中，等待当子进程终止的时候，回收操作系统分配给子进程的资源。
*/
void serve_dynamic(int fd, char *filename, char *cgiargs) 
{
    char buf[MAXLINE], *emptylist[] = { NULL };

    
    sprintf(buf, "HTTP/1.0 200 OK\r\n"); 
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Server: Tiny Web Server\r\n");
    Rio_writen(fd, buf, strlen(buf));
  
    if (Fork() == 0) { //孩子进程
    //增加或修改环境变量
    setenv("QUERY_STRING", cgiargs, 1); 
    //重定向输出到客户端
    Dup2(fd, STDOUT_FILENO);         
    //运行CGI程序
    Execve(filename, emptylist, environ); 
    }
    Wait(NULL); 
}


/*
    Tiny-Client-Server会检查一些明显的错误，并把它们报告给客户端。clienterror函数发送一个HTTP响应到客户端，
    在响应行中包含相应的状态码和状态消息，响应主体中包含一个HTML文件，向客户端用户解释这个错误。
*/
void clienterror(int fd, char *cause, char *errnum, 
         char *shortmsg, char *longmsg) 
{
    char buf[MAXLINE], body[MAXBUF];

    sprintf(body, "<html><title>Tiny Error</title>");
    sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
    sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
    sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
    sprintf(body, "%s<hr><em>The Tiny Web server</em>\r\n", body);

    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
    Rio_writen(fd, buf, strlen(buf));
    Rio_writen(fd, body, strlen(body));
}
