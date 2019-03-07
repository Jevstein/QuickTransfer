一、说明  
    实现快速文件传输。其中demo用于测试UDP分片（可忽略）

二、技术  
    1.c语言：面向对象  
    2.udp协议: UDP分片传输，UDP分片重组，应用层保证可靠  
    3.反应堆(reactor): 支持epoll、kequeue  
    4.文件映射(mmap)    
    5.环形缓冲(circular buffer)  

三、使用  
    1.执行./src/makefile编译，将会在./bin目录下生成server、client文件  
    2.依次启动server和client：  
        1）运行过程中，将看到文件内容的传输状态  
        2）运行完毕，会输出文件的传输速率，且在./bin目录下产生下载来自./resource/下的“git_cmd.jpg”文件  

四、性能

五、不足  
    1.协议包加解密待完善  
    2.reactor队列可进一步完善  

六、建议(lee哥)  
    1.“不建议加上公司的名称或者个人的名称，尤其是在项目当中，不利于代码复用”  
      ：接口加前缀(项目名缩写)防止命名冲突，黑盒细节部分不加  
    2.在应用程序的log中打印崩溃前的日志
    3.zero copy: 弃用memcpy，直接将mmap的数据send出去  
    4.在IO的效率上还可以想很多办法，这还没有发挥出极致的性能  
