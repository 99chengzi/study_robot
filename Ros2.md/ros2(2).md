![alt text](image-3.png)
# <center>核心概念</center>
## 一. 工作空间
### 1.工作空间是什么
工作空间是一个存放项目开发相关文件的文件夹，也是开发过程中存放所有资料的大本营。
+ src，代码空间，未来编写的代码、脚本，都需要人为的放置到这里；
+ build，编译空间，保存编译过程中产生的中间文件；
+ install，安装空间，放置编译得到的可执行文件和脚本；
+ log，日志空间，编译和运行过程中，保存各种警告、错误、信息等日志。
> 在你当前这个 Ubuntu 终端里直接输入：
`explorer.exe .`
回车，会自动弹出 Windows 窗口，这里就能看到 build、install、log、src 四个文件夹。

针对Ubuntu24.04版本：
1. 创建工作空间
```
mkdir -p ~/dev_ws/src
cd ~/dev_ws/src
git clone shturl.cc/q3mLqDLNAnnbq95OD3zx7iBKiCIp3UUJgcCIAvj
```
2. 自动安装依赖
+ 安装 pip
  `sudo apt install -y python3-pip`
+ 正确安装 rosdep
  `sudo apt install ros-dev-tools -y`
+ 初始化 + 更新 rosdep
  `sudo rosdep init
rosdep update`
+ 一键安装所有代码依赖（回到工作空间根目录）
`cd ~/dev_ws
rosdep install -i --from-path src --rosdistro humble -y`
3. 编译工作空间
+ 安装编译工具 colcon
  `sudo apt install python3-colcon-ros -y`
+ 编译
  `cd ~/dev_ws
colcon build`
## 二. 功能包
### 1. 创建功能包
如何在ROS2中创建一个功能包呢？我们可以使用这个指令：
`$ ros2 pkg create --build-type <build-type> <package_name>`
ros2命令中：
+ pkg：表示功能包相关的功能；
+ create：表示创建功能包；
+ build-type：表示新创建的功能包是C++还是Python的，如果使用C++或者C，那这里就跟ament_cmake，如果使用Python，就跟ament_python；
+ package_name：新建功能包的名字。
`cd ~/dev_ws/src`  ==一定要先在这个目录下==
`ros2 pkg create --build-type ament_cmake learning_pkg_c  # C++`
`ros2 pkg create --build-type ament_python learning_pkg_python # Python`
### 2.编译功能包
在创建好的功能包中，我们可以继续完成代码的编写，之后需要编译和配置环境变量，才能正常运行：
```
$ cd ~/dev_ws
$ colcon build   # 编译工作空间所有功能包
$ source install/local_setup.bash
```
## 三. 节点
### 创建节点流程：
想要实现一个节点，代码的实现流程是这样做：
+ 编程接口初始化
+ 创建节点并初始化
+ 实现节点功能
+ 销毁节点并关闭接口
```
import rclpy                                   # ROS2 Python接口库
from rclpy.node import Node                    # ROS2 节点类


class HelloNode(Node):
    def __init__(self):
        super().__init__('hello_node')           # ROS2节点父类初始化
        self.get_logger().info('Hello World!')   # ROS2日志输出


def main(args=None):                                # ROS2节点主入口main函数
    rclpy.init(args=args)                            # ROS2 Python接口初始化
    node = HelloNode()                              # 创建ROS2节点对象并进行初始化
    rclpy.spin(node)                                 # 循环等待ROS2退出
    node.destroy_node()                            # 销毁节点对象
    rclpy.shutdown()                               # 关闭ROS2 Python接口


if __name__ == '__main__':
    main()
```
<u>节点命令的常用操作如下：</u>
```
$ ros2 node list               # 查看节点列表
$ ros2 node info <node_name>   # 查看节点信息
```