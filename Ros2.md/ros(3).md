# <center>常用工具</center>
## 一. Launch
Launch启动文件，它是ROS系统中多节点启动与配置的一种脚本。
### 1.命令行参数配置
运行一个Rviz可视化上位机:
`$ ros2 run rviz2 rviz2`
并且加载某一个配置文件，使用命令行的话，是这样的：
`$ ros2 run rviz2 rviz2 -d <PACKAGE-PATH>/rviz/turtle_rviz.rviz`
如果放在launch文件里，启动就优雅很多了：
`$ ros2 launch learning_launch rviz.launch.py`
