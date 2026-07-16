# <center><font face ="仿宋" font color=orange size=10>git & github</font>
# 一. 概念辨析
### 1.git
> 功能：版本控制———保存各种历史版本
#### local repository本地仓库-->remote repository远端仓库
> git管理本地的代码仓库

> github就相当于一个免费的远端仓库

存档、仓库、项目在git里是一个东西
# 二. 前期准备
#### 下载VS code、git
#### 注册github账号并创建一个仓库
##### 若打开github遇网络问题可使用==Watt Toolkit Installer==进行网络加速
# 三. 一些注意事项
`git remote remove origin`
> 这条命令可以删除错误的远程绑定

`git remote -v`
> 这条命令可以查看当前绑定的远程地址

`git remote add origin`+==要绑定的地址==
> 重新绑定正确的 study 仓库地址

==要绑定的地址==
![alt text](image-5.png)
==每次更改后要ctrl+s(保存)才能上传到github==
#### 更改处的提交信息输入框可以这么填：
> 新增内容：feat: 新增git远程管理笔记
修改完善：docs: 完善github绑定、推送步骤
修正错误：fix: 修正git add命令写法错误