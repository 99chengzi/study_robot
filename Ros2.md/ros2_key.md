# <center>ros2</center>
## 1.辨析
+ ros2 run
只启动单个节点，一次只能运行一个程序；适合调试单个代码。
`ros2 run 功能包名 可执行节点名`
+ ros2 launch
读取 launch 启动文件，一次性批量启动多个节点；
```
# 同时启动多个节点
ros2 launch 包名 xxx.launch.py
```
## 2.编译工作空间
> 编译前请确保处于工作空间的根目录下。
```
#回到工作空间根目录
cd ~/dev_ws
#编译
colcon build
```
## 3.刷新环境变量：
` source install/setup.bash`
### 知识点补充：
+ ros2 pkg list 只会列出编译成功、被环境变量注册的功能包；
+ 只要编译 aborted/failed，这个包不会被注册到 install，无论怎么 source 都识别不到；
+ source 不修复编译错误，仅加载已成功编译的包。
```
colcon build --packages-select cyj_pkg1 --event-handlers console_direct+
#只编译你出问题的包，输出完整报错细节，专门用来调试编译失败的包。
```
## 4.在功能包目录下创建 launch 目录：
```
# 在功能包目录下执行
mkdir launch
```
## 5.安装系统依赖库：
`sudo apt install`

==ctrl+shift+5==可以在VS code原有终端的基础上再新开一个终端