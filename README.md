# Tiny-Client-Server

这个项目代码是一个小型的却功能基本齐全的Web服务器。 

## Getting Started

这份指引将告诉你需要配置的环境，以及如何在你的本地计算机上编译运行，并呈现简单的客户端-服务器模型。

### Prerequisites

1. Linux系统：对版本没有什么限制，我使用的是Ubuntu16.04，也可在Linux虚拟机上运行。

2. GCC编译器：一般Linux预装了GCC编译器，不同的Linux版本安装的GCC版本也不同，你可以在终端输入以下命令查看GCC版本：
```
gcc -v
```

### Clone the repository
```
git clone https://github.com/Ruofei-Li/Tiny-Client-Server.git
```

### Compile
```
cd Tiny-Client-Server
gcc server.c helper.c -o server
```

## Running

在当前目录下放一张jpg格式的图片，加入命名为test.jpg

在终端运行编译好的主程序，设定一个端口号（介于1024-65535之间）：
```
./server 1028
```

打开浏览器，在地址栏输入：
```
http://localhost:1028/test.jpg
```
可以看到浏览器成功加载出了事先放入的图片。除此以外，你也可以放入png、gif格式的图片，或者html、无格式文件等。

除了可以在浏览器里访问服务器的内容，你也可以在终端访问。


服务器就启动成功了，再打开一个终端，输入：
```
telnet localhost 1028
```

出现连接到本地的信息，再输入HTTP请求：
```
get 
```

### Break down into end to end tests

Explain what these tests test and why

```
Give an example
```

### And coding style tests

Explain what these tests test and why

```
Give an example
```

## Deployment

Add additional notes about how to deploy this on a live system

## Built With

* [Dropwizard](http://www.dropwizard.io/1.0.2/docs/) - The web framework used
* [Maven](https://maven.apache.org/) - Dependency Management
* [ROME](https://rometools.github.io/rome/) - Used to generate RSS Feeds

## Contributing

Please read [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) for details on our code of conduct, and the process for submitting pull requests to us.

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/your/project/tags). 

## Authors

* **Billie Thompson** - *Initial work* - [PurpleBooth](https://github.com/PurpleBooth)

See also the list of [contributors](https://github.com/your/project/contributors) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Hat tip to anyone whose code was used
* Inspiration
* etc

