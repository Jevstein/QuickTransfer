一、说明  
    实现快速文件传输。其中demo用于测试UDP分片（可忽略）

二、技术  
    1.c语言  
    2.udp协议: UDP分片传输，UDP分片重组，应用层保证可靠  
    3.反应堆(reactor): 支持epoll、kequeue  
    4.文件映射（mmap）  
    5.环形缓存(circular buffer)  

三、使用
    1.执行./src/makefile编译，将会在./bin目录下生成server、client文件  
    2.依次启动server和client：  
        1）运行过程中，将看到文件内容的传输状态  
        2）运行完毕，会输出文件的传输速率，且在./bin目录下产生下载来自./resource/下的“git_cmd.jpg”文件  

四、性能

五、不足  
    1.协议包加解密待完善  
    2.reactor队列可进一步完善  