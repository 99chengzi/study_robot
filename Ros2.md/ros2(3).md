# <center>常用工具</center>
## 一. Launch
Launch启动文件，它是ROS系统中==多节点启动与配置的一种脚本==。
### 1.命令行参数配置
运行一个Rviz可视化上位机:
`$ ros2 run rviz2 rviz2`
并且加载某一个配置文件，使用命令行的话，是这样的：
`$ ros2 run rviz2 rviz2 -d <PACKAGE-PATH>/rviz/turtle_rviz.rviz`
如果放在launch文件里，启动就优雅很多了：
`$ ros2 launch learning_launch rviz.launch.py`
## 二. TF
==机器人坐标系管理神器==
### 1.查看TF树
`$ ros2 run tf2_tools view_frames `
默认在当前终端路径下生成了一个frames.pdf文件，打开之后，就可以看到<u>系统中各个坐标系的关系</u>。
### 2.查询坐标变换信息
`$ ros2 run tf2_ros tf2_echo turtle2 turtle1`
如果我们想要知道某两个坐标系之间的具体关系，可以通过tf2_echo这个工具查看。
终端中会循环打印坐标系的变换数值，由平移和旋转两个部分组成，还有旋转矩阵。
`ros2 run turtlesim turtle_teleop_key`控制键盘