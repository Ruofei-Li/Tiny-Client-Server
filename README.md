# Tiny-Client-Server

这个项目是一个小型的、功能基本齐全的Web服务器。在简单的编译运行之后，便可在本地创建一个小型的服务器，支持通过浏览器和终端访问服务器，服务器提供简单的静态服务和动态服务。

## Getting Started

这份指引将告诉你需要配置的环境，以及如何在你的本地计算机上编译运行，并呈现简单的客户端-服务器模型。

### Prerequisites

1. Linux系统：对版本没有什么限制，我使用的是Ubuntu16.04，也可在Linux虚拟机上运行。

2. GCC编译器：一般Linux预装了GCC编译器，不同的Linux版本安装的GCC版本也不同，你可以在终端输入以下命令查看GCC版本：
```
gcc -v
```
3. 如果提示没有安装GCC，请参考[Liunx安装GCC](https://www.cnblogs.com/dybk/p/8085685.html)

### Clone the repository
```
git clone https://github.com/Ruofei-Li/Tiny-Client-Server.git
```

### Compile
```
cd Tiny-Client-Server
gcc server.c helper.c -o server
cd cgi-bin
gcc adder.c -o adder
```

## Running server

### Prepare documents

在当前目录下放一张jpg格式的图片，命名为test.jpg

### Start the server

在终端运行编译好的主程序，设定一个端口号（介于1024-65535之间）：
```
cd ..
./server 1028
```

## Testing on browser

服务器提供两种服务：静态服务和动态服务，可以通过浏览器请求提供服务。

### static service

* 打开浏览器，在地址栏输入：
```
http://localhost:1028/test.jpg
```
* 可以看到浏览器成功加载出了事先放入的图片。除此以外，你也可以访问png、gif格式的图片，或者html、无格式文件等，前提是你的服务器主目录下有相应的文件。


### dynamic service

* 在浏览器地址栏输入：
```
http://localhost:1028/cgi-bin/adder?10&20 
```
* 服务器提供一个简单的加法CGI程序，浏览器传入参数，服务器计算出结果并将信息显示在浏览器中。



## Testing on terminal

除了可以在浏览器里访问服务器的内容，你也可以在终端访问。

### static service

* 再打开一个终端，输入：
```
telnet localhost 1028
```
* 输入HTTP请求报头，并敲两次回车：
```
get /test.jpg HTTP/1.0
```
* 由于终端不能显示图片，所以输出图片编码，看起来是乱码，你也可以测试html或无格式文件以正常显示文件内容。


### dynamic service

* 终端输入：
```
telnet localhost 1028
```
* 输入HTTP请求报头，并敲两次回车：
```
get /cgi-bin/adder?10&20 HTTP/1.0
```
* 输出HTTP响应信息和计算结果。


## Contact

任何关于此项目的问题，或者有兴趣交流的朋友，请联系liruofei0128@163.com
