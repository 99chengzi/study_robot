
![alt text](image.png)
# <center>系统架构</center>
## 一. ROS/ROS2是什么？
ROS（ROS1和ROS2的总称）在自身的设计上尽量做到了模块化，由通信机制、开发工具、应用功能、生态系统四大部分组成。
ROS的社区：其实就是ROS相关资源的整合方式，比如wiki说明、问答网站、应用源码、论坛讨论等都算是社区中的元素。
> ROS全球社区有几个重要网站：

1. answers.ros.org，这是一个ROS问答网站，大家可以在上边提出任何关于ROS的问题，全球很多开发者都很乐意回答我们的问题；
2. wiki.ros.org，这是ROS的维基百科，记录了ROS教程和各种功能包的使用；
3. discourse.ros.org，这是ROS论坛，关于ROS开发的新鲜事都可以在这里发表和查看，比如ROS的活动、新功能包的发布等等。
4. index.ros.org，是ROS各种资源的一个索引网站；
5. packages.ros.org，是ROS功能包存储的数据库。
## 二. ROS2对比ROS1
+ 节点干掉了Master
+ 通信换成了DDS
+ 核心概念没变化
+ 编程难度有上升
## 三. ROS2命令行操作