# <center>代码解答</center>
  
首先，这份代码是一个 **足球机器人的控制程序**。它运行在 **ROS（机器人操作系统）** 框架下，好比是机器人的"大脑"。
  
---
  
## 一、整体上是干什么的？
  
**一句话：这个机器人是一个足球队的一员，代码负责它"看清场上情况→思考该干啥→执行动作"的整个循环。**
  
场上最多5个机器人（编号1-5），1号是守门员，2~5号是场上队员。它们通过一个"教练"（Coach）发来的比赛模式指令来知道现在是开球、罚球、还是正常比赛。
  
---
  
## 二、核心类 `NuBotControl`
  
```cpp
class NuBotControl
```
  
这个类就是机器人的大脑。它握有 4 样东西：
  
| 组件 | 作用 |
|---|---|
| `World_Model_Info` | 机器人对世界的认知——自己和队友在哪、球在哪、对手在哪 |
| `Strategy` | 战术决策模块 |
| `Plan` | 路径规划 |
| `StaticPass` | 静态传球逻辑 |
  
除了这几样"大脑"模块，它还有一堆 ROS 的 **发布器(publisher)** 和 **订阅器(subscriber)**，专门用来和机器人其他部分通信。
  
---
  
## 三、构造函数：机器人的"出生"
  
```cpp
NuBotControl(int argc, char **argv)
```
  
初始化时主要做这几件事：
  
1. **读取机器人编号** —— 通过环境变量 `AGENT` 知道自己是几号（1~5）
2. **创建三个发布器**，用于告诉底层硬件：
   - 速度指令 (`velcmd`)
   - 战术信息 (`strategy`)
   - 动作指令 (`actioncmd`)
3. **订阅两个话题**，接收信息：
   - 世界模型信息（队友位置、球位置、对手位置等）
   - 球是否被控住
4. **创建定时器**：每 **0.015 秒（15毫秒）** 调用一次主循环
5. **初始化各种参数**：最大速度、动作初始化为"无动作"等
  
---
  
## 四、主循环：`loopControl`——每15ms跑一次
  
这个函数是代码的"心脏"，它每次被调用时做这几步：
  
```
拿到当前比赛模式
拿到自己位置、朝向、球位置、球速度
     │
     ├── 如果比赛模式是 STOP（停止）→ 啥也不做
     ├── 如果比赛模式是开球/罚球/界外球等准备阶段 → 跑位（跳到 positioning）
     ├── 如果比赛模式是 PARKING（停车） → 开到指定停车点
     └── 否则 → 正常比赛（跳到 normalGame）
     │
然后 → 处理吸球/放球（handleball）
然后 → 发出底层指令（setEthercatCommand）
然后 → 广播自己的战术信息（pubStrategyInfo）
```
  
---
  
## 五、比赛中的核心逻辑：`normalGame`
  
这是最精华的部分。当一个场上队员（非守门员）且自己是距离球最近的机器人时，它变"活跃"（`isactive=true`）。
  
活跃机器人的行为分三个阶段：
  
### 阶段1：还没控到球 → 去抢球
```cpp
if(!已经控球) {
    打开吸球装置
    转向球的方向
    朝球移动
    动作模式 = CatchBall（接球）
}
```
  
### 阶段2：控到球了，但还没到射门位置 → 带球跑
```cpp
else if(没到射门位置) {
    动作模式 = MoveWithBall（带球移动）
    转向目标点
    朝目标点移动
}
```
  
### 阶段3：到了位置 → 转身射门
```cpp
else {
    动作模式 = TurnForShoot（转身准备射门）
    面朝对方球门
    如果朝向在球门有效范围内：
        计算力量
        执行射门（shoot_flag = true）
}
```
  
射门后有 20 个循环（约 0.3 秒）的冷却时间，防止重复射门。
  
---
  
## 六、辅助函数说明
  
| 函数 | 作用 |
|---|---|
| `isNearestRobot()` | 检查自己是不是离球最近的非守门员 |
| `move2target()` | 设置目标位置和速度，距离<20cm算到达 |
| `move2ori()` | 设置目标朝向，误差小于8°算到达 |
| `positioning()` | 根据比赛模式（开球/罚球等）调用对应的站位函数 |
| `OurDefaultReady_()` | 我方的站位方案——1号守门，2号靠近球，3号在球后稍远，4/5号在左/右侧 |
| `OppDefaultReady_()` | 对手发球时的站位方案——退到自己半场防守 |
| `parking()` | 比赛结束/暂停时，机器人开到指定区域排好队 |
| `handleball()` | 控制吸球装置的开关——活跃时才吸球 |
| `setEthercatCommand()` | 把所有动作指令打包发送到底层电机去执行 |
  
---
  
## 七、main 入口
  
```cpp
int main(int argc, char **argv)
{
    ros::init(argc,argv,"nubot_control_node");  // 初始化ROS节点
    ros::Time::init();                          // 初始化时间
    nubot::NuBotControl nubotcontrol(argc,argv); // 创建"大脑"
    ros::spin();                                // 进入无限循环，接收/处理消息
    return 0;
}
```
  
`ros::spin()` 之后，程序就一直在跑——定时器每隔15ms自动触发 `loopControl`，循环往复。
  
---
  
## 八、小结——用一句话记住它
  
> **这个代码是一个足球机器人"边看边想边做"的主控程序：每15毫秒看一次场上情况，根据比赛模式决定是跑位防守还是正常比赛，正常比赛时如果自己离球最近就去抢球、带球、射门，最后把动作指令发到底层去执行。**
  
  
  
---
# 改动后代码：
## 一句话：它比上一版多了什么？
  
> 上一版是"单体机器人去抢球、带球、射门"的简单版本；
> **这一版是真正打了比赛的完整版——增加了多人配合（传球）、守门员、点球、测试模式、动态参数调整。**
  
---
  
## 一、初始化时的"新装备"
  
构造函数里新加了几样东西：
  
| 新东西 | 干什么用的 |
|---|---|
| `isRelocation_pub_` | 告诉视觉系统"比赛开始了，重新定位我在场上的位置" |
| 守门员专用订阅 | 接收左右两个3D相机传来的球信息，帮守门员判断球的轨迹 |
| 3D障碍物订阅 | 从 Kinect 深度相机获取场上障碍物的三维信息 |
| `reconfigureServer_` | 可以在程序运行时**动态调节参数**（比如速度系数），方便调试 |
| `NubotTest` | 测试模块，比如用来测试射门动作 |
  
另外它还多了一个标志 `_isOpposite`，当机器人被判定为"站反了"（朝向反了）时会重置状态。
  
---
  
## 二、信息更新：更周到了
  
`update_world_model_info` 除了上一版更新的位置、球、对手信息外，还多了：
  
- **教练(Coach)的测试模式信息** —— 在测试模式下有专门的参数
- **速度动态调节** —— 最大速度和角速度从教练指令里读，不再是固定值
- **传球状态管理** —— 当其他机器人说"我要传球了"，自己这边的状态也跟着更新，保证整个队伍信息一致
- **定位重置机制**：每当教练发 STOP 指令 → 通知视觉系统"重新定位哦"
  每当教练发 START 指令 → 通知视觉系统"好，定好了，开始比赛"
  
---
  
## 三、主循环：更清晰的流程
  
```cpp
loopControl() {
    读比赛模式、自己位置、球位置...
  
    非测试模式:
        如果是 STOPROBOT 或 机器人站反了 → 重置所有状态
        如果是开球/罚球等准备阶段 → 跑位 (positioning)
        如果是停车 → 停车 (parking)
        否则（正常比赛）:
            ├── 如果是点球 → 执行点球逻辑
            └── 否则 → 正常比赛 (normalGame)
  
        处理吸球放球 (handleBall)
        处理踢球/射门/传球 (kickBall)
        广播战术信息 (pubStrategyInfo)
        发送速度指令 (setEthercatCommond)
}
```
  
---
  
## 四、正常比赛 `normalGame`：最核心的变化
  
这一版不再是简单地"谁离球近谁上"，而是多了一层 **`IsMoveForStartCommand_`** 机制：
  
### 对方发球时（前几秒不能动）
```cpp
if (!IsMoveForStartCommand_) {
    选角色（但不动）
    清空动作 → 等着
}
```
  
### 我方发球或等待期过了以后
```cpp
else {
    计算球动了多远（判断是不是死球）
    如果球被传出:
        传球人 → 切换到普通角色
        检查自己是否挡住了接球人的射门角度
        如果挡住了 → 执行"让开"动作 (coorrect_target)
    否则:
        正常执行战术 (m_strategy_->process())
        驱动主动机器人去执行动作
}
```
  
这里有一个很关键的函数 `coorrect_target()`，它的作用是：
  
> **当传球路线上有队友恰好挡在中间时，自动调整那个队友的站位，让他让出一条通道，避免挡住传球线路。**
  
这个函数用到了**点与多边形的位置判断** (`pnpoly`)，判断机器人是否在传球线路的危险区域内。
  
---
  
## 五、踢球逻辑 `kickBall`：不只是射门
  
上一版只有射门，这一版区分了：
  
| 情况 | 决定 |
|---|---|
| 刚抢到球，球权不稳 | 传球（`PASS`），不射门 |
| 主动机器人正在转身准备传球 | 传球（`PASS`） |
| 静态传球模式且自己是传球手 | 传球（`PASS`） |
| 其他情况 | 射门（`SHOOT`） |
  
射门后还有 15 个循环（约 0.225 秒）的冷却期，防止连发。
  
---
  
## 六、新功能一览
  
| 函数 | 作用 |
|---|---|
| `positioning()` | 比赛开始前根据发球类型（我方开球/对方开球/界外球/角球...）跑位 |
| `parking()` | 比赛结束时排队停车，y坐标从 launch 文件读取 |
| `penaltyStart()` | 点球逻辑：我方点球 → 前锋测试射门；对方点球 → 守门员扑救 |
| `handleBall()` | 控制吸球装置开关：主动机器人/接球机器人才开，其他关闭；卡住时加大转速 |
| `kickBall()` | 判断当前该射门还是传球，计算力量，执行踢出 |
| `pubStrategyInfo()` | 把自己知道的战术信息（角色、站位编号、传球信息）发给全队 |
| `coorrect_target()` | 修正队友站位，避免挡住传球线路（非常重要！） |
| `pnpoly()` | 判断一个点是否在多边形内部，用于检测"是否挡在传球线路上" |
| `Opposite()` | 收到"你站反了"信号时触发状态重置 |
  
---
  
## 七、与上一版的核心区别
  
| 版本 | 上一版 | 这一版 |
|---|---|---|
| 分工 | 只有谁离球近谁上 | 有角色选择（主动/助攻/守门/中场） |
| 传球 | 无 | 支持静态传球和动态传球 |
| 守门员 | 只有一个简单的站位 | 用左右3D相机追踪球轨迹 |
| 点球 | 无 | 完整的点球逻辑 |
| 站反检测 | 无 | 有（`_isOpposite`） |
| 动态调参 | 无 | 有 dynamic_reconfigure |
| 跑位校正 | 固定写死 | 动态计算，防止挡住传球线 |
| 代码结构 | 简单粗暴 | 更模块化、工程化 |
  
---
  
## 八、一个完整的比赛流程
  
```
裁判发 STOP  → 所有机器人回到初始位置
裁判发 开球  → 机器人跑到指定站位
裁判发 START → 正常比赛开始
  
    ↓ 每15ms重复
  
每个机器人看场上局势
  ├── 如果离球最近且合适 → 变成"主动角色"（去抢球）
  ├── 如果别人更适合 → 变成助攻/防守角色（跑位、接应）
被抢到球后：
  ├── 能射门 → 射门
  ├── 队友位置更好 → 传球
  ├── 球权不稳 → 保险起见传球
  └── 射门/传球后冷却一小会儿
```
  
---
  
## 总结
  
> **这一版代码才是真正能打比赛的版本。它不再只是一个"个体机器人"的逻辑，而是包含完整的多人协作系统：角色分配、传球配合、站位校正、避让队友、点球处理——所有这些加在一起，才是一支真正的机器人足球队。**
  
如果你是初学者，建议先拿上一版的 "抢球→带球→射门" 流程打底，再看这一版的传球/配合逻辑，会更容易理解。
  
# 官方代码解析：
  
---
  
## 第一部分：头文件引入（第1-16行）
  
```cpp
#include "core.hpp"
```
  
引入项目核心头文件，里面定义了各种基础数据结构（DPoint点坐标、Angle角度等）和常量。
  
```cpp
#include <nubot_common/VelCmd.h>        // 速度指令消息类型
#include <nubot_common/WorldModelInfo.h> // 世界模型信息消息类型
#include <nubot_common/BallInfo3d.h>     // 球的三维信息消息类型
#include <nubot_common/ActionCmd.h>      // 动作指令消息类型
#include <nubot_common/BallIsHolding.h>  // 是否吸住球的消息类型
#include <nubot_common/StrategyInfo.h>   // 战术信息消息类型
#include <nubot_common/TargetInfo.h>     // 目标信息消息类型
```
  
这些都是 ROS 通信用的消息类型定义。机器人的不同模块之间通过"话题（topic）"收发消息，这些头文件定义的就是消息的格式——比如"速度指令"消息包含哪些字段。
  
```cpp
#include <ros/ros.h>                     // ROS核心库
#include <nubot/...> 系列               // 项目自己的模块：世界模型、战术、路径规划、静态传球
```
  
---
  
## 第二部分：宏定义和常量（第17-19行）
  
```cpp
#define RUN 1     // 贴地射门
#define FLY -1    // 挑射（高球射门）
```
  
这两个宏对应射门方式。规则**第5条"己方半场高球"**专门规定：如果机器人从己方半场踢出高球（高度>60cm），且在落地点3米内没有队友，会判给对方任意球。所以 `RUN`（贴地）和 `FLY`（高球）的选择是有规则依据的。
  
```cpp
const double DEG2RAD = 1.0/180.0*SINGLEPI_CONSTANT;
```
  
角度转弧度系数。`SINGLEPI_CONSTANT` 是 π 的自定义常量。为什么要转？因为机器人的数学计算（三角函数、角度差）全部基于弧度，而人脑习惯用角度。
  
---
  
## 第三部分：类定义与成员变量（第24-59行）
  
```cpp
namespace nubot { class NuBotControl { ... }; }
```
  
`NuBotControl`（Nubot Control 的缩写）是**控制核心类**，相当于机器人的"大脑"。
  
### ROS通信成员（第26-35行）
  
```cpp
ros::Subscriber  ballinfo3d_sub1_;   // （注释掉）3D球信息订阅
ros::Subscriber  odoinfo_sub_;       // 里程计信息订阅
ros::Subscriber  obstaclesinfo_sub_; // 障碍物信息订阅
ros::Subscriber  worldmodelinfo_sub_;// 世界模型信息订阅 ← 最重要的订阅
ros::Subscriber  ballisholding_sub_;// 吸球状态订阅
ros::Publisher   motor_cmd_pub_;    // 速度指令发布器（发给底层电机）
ros::Publisher   strategy_info_pub_;// 战术信息发布器（发给队友）
ros::Publisher   action_cmd_pub_;   // 动作指令发布器（发给底层控制器）
ros::Timer       control_timer_;    // 定时器，控制主循环频率
```
  
这些本质上就是机器人的"眼睛"和"嘴巴"——**订阅（sub）是接收别人的信息，发布（pub）是把自己算好的指令发出去**。
  
### 核心模块（第39-43行）
  
```cpp
World_Model_Info world_model_info_;  // 世界模型——存储场上所有信息
Strategy  * m_strategy_;             // 战术决策模块
Plan   m_plan_;                      // 路径规划模块
StaticPass m_staticpass_;            // 静态传球模块
```
  
这四个模块的关系：
- **世界模型**是"数据库"——谁在哪、球在哪、裁判说了什么
- **战术**是"教练"——根据数据决定现在该干什么（进攻/防守/跑位）
- **路径规划**是"导航仪"——怎么走到目标位置
- **静态传球**是"传球战术本"——死球时的固定传球配合方案
  
### 控制参数和状态变量（第45-59行）
  
```cpp
double kp_, kalpha_, kbeta_;       // PID控制参数（目前未在代码中使用）
char    match_mode_;                // 当前比赛模式（STOP/START/开球/罚球...）
char    pre_match_mode_;            // 上一场比赛模式
DPoint  robot_pos_;                 // 机器人当前位置（x,y坐标）
Angle   robot_ori_;                 // 机器人当前朝向（弧度）
double  robot_w;                    // 机器人当前角速度
DPoint  ball_pos_;                  // 球的位置
DPoint  ball_vel_;                  // 球的速度
bool    isactive = false;           // 是否"活跃"（即离球最近的那个）
bool    shoot_flag = false;         // 是否刚射过门（冷却标志）
int     shoot_count = 0;            // 射门后计数，用于控制冷却时长
```
  
这些变量每15ms被更新一次，是机器人"对自己当前状态的认识"。
  
---
  
## 第四部分：构造函数（第61-98行）
  
### 读取机器人编号
  
```cpp
const char * environment;
```
  
这里的环境变量 `AGENT` 对应规则中的**机器人编号**。在 RoboCup 仿真赛中，一支队伍可以有多个机器人，每个机器人通过编号（1~5）区分自己的角色——**1号通常是守门员，2~5号是场上队员**。
  
### 初始化 ROS 通信
  
```cpp
motor_cmd_pub_  = nh_->advertise<nubot_common::VelCmd>("nubotcontrol/velcmd", 1);
strategy_info_pub_ = nh_->advertise<nubot_common::StrategyInfo>("nubotcontrol/strategy", 10);
action_cmd_pub_ = nh_->advertise<nubot_common::ActionCmd>("nubotcontrol/actioncmd", 1);
```
  
创建三个"喇叭"（发布器），分别向不同话题喊话：
- `velcmd` → 告诉底层：我要以什么速度跑
- `strategy` → 告诉队友：我现在是什么角色、在做什么
- `actioncmd` → 告诉底层：我的完整动作（去哪、怎么转、射不射门）
  
### 订阅世界模型和吸球状态
  
```cpp
worldmodelinfo_sub_ = nh_->subscribe("worldmodel/worldmodelinfo", 1,
                                      &NuBotControl::update_world_model_info, this);
ballisholding_sub_ = nh_->subscribe("ballisholding/BallIsHolding", 1,
                                     &NuBotControl::update_ballisholding, this);
```
  
订阅两个话题，相当于给机器人装了两个"耳朵"：
1. 收听全队共享的**场上信息**（所有机器人的位置、球的位置、对手位置、裁判指令）
2. 收听底层硬件反馈的**吸球状态**（有没有吸住球）
  
### 创建定时器——心率的起点
  
```cpp
control_timer_ = nh_->createTimer(ros::Duration(0.015),
                                   &NuBotControl::loopControl, this);
```
  
**每0.015秒（15毫秒，约66Hz）执行一次 `loopControl`**。这就是整个机器人的"心跳"——所有决策和控制都在这个循环里完成。
  
为什么是15ms？这个频率既要足够快以应对高速球赛（球速可达几米/秒），又不能让CPU过载。
  
### 初始化各模块
  
```cpp
world_model_info_.AgentID_ = atoi(environment);       // 设置机器人编号
world_model_info_.CoachInfo_.MatchMode = STOPROBOT;   // 初始为"停止"模式
m_plan_.world_model_ = &world_model_info_;             // 让plan和staticpass共享
m_plan_.m_subtargets_.world_model_ = &world_model_info_; // 同一个世界模型
m_staticpass_.world_model_ = &world_model_info_;
m_strategy_ = new Strategy(world_model_info_, m_plan_);// 创建战术模块
```
  
这里有一个重要的设计：**`world_model_info_` 是唯一的"真相源"**，Plan、StaticPass、Strategy 都共享指向它的指针，保证所有模块看到的是同一份数据。
  
### 初始化动作指令
  
```cpp
action_cmd_.maxvel = MAXVEL;           // 最大线速度
action_cmd_.maxw = MAXW;               // 最大角速度
action_cmd_.move_action = No_Action;   // 移动动作：无
action_cmd_.rotate_acton = No_Action;  // 旋转动作：无
action_cmd_.rotate_mode = 1;           // 旋转模式
action_cmd_.shootPos = 0;              // 射门位置：无
action_cmd_.strength = 0;              // 射门力量：0
action_cmd_.handle_enable = 0;         // 吸球装置：关闭
```
  
这里初始化为空动作，等每15ms的主循环来更新这些值。
  
---
  
## 第五部分：析构函数（第100-108行）
  
```cpp
~NuBotControl()
{
    m_plan_.m_behaviour_.app_vx_ = 0;   // 停止移动
    m_plan_.m_behaviour_.app_vy_ = 0;
    m_plan_.m_behaviour_.app_w_  = 0;
    ball_holding_.BallIsHolding = 0;     // 关闭吸球
    action_cmd_.shootPos = 0;            // 取消射门
    action_cmd_.strength = 0;
    action_cmd_.handle_enable = 0;
    action_cmd_pub_.publish(action_cmd_);// 发出去，让机器人完全停止
}
```
  
程序退出前，发一条"停止一切动作"的指令，确保机器人安全停车。
  
---
  
## 第六部分：`update_world_model_info`——"眼睛"更新（第110-165行）
  
这是**最重要的回调函数之一**，每当有新的世界模型信息发布过来，它就被调用。
  
### 更新所有机器人的信息
  
```cpp
for(std::size_t i = 0 ; i < OUR_TEAM ; i++)
{
    world_model_info_.RobotInfo_[i].setID(_world_msg.robotinfo[i].AgentID);
    world_model_info_.RobotInfo_[i].setTargetNum(1~4, ...);
    world_model_info_.RobotInfo_[i].setpassNum(...);
    world_model_info_.RobotInfo_[i].setcatchNum(...);
    world_model_info_.RobotInfo_[i].setLocation(...);
    world_model_info_.RobotInfo_[i].setHead(...);
    world_model_info_.RobotInfo_[i].setVelocity(...);
    world_model_info_.RobotInfo_[i].setStuck(...);  // 是否被卡住
    world_model_info_.RobotInfo_[i].setKick(...);   // 是否踢到球了
    world_model_info_.RobotInfo_[i].setValid(...);  // 位置是否有效
    world_model_info_.RobotInfo_[i].setW(...);       // 角速度
}
```
  
这里遍历己方全部机器人（1~5号），更新每个队友的站位编号、位置、朝向、速度等。`setTargetNum(1~4)` 存储的是**静态传球时的站位偏好编号**，不同编号对应场上的不同站位点。
  
注意：**自己的战术信息（角色、是否带球）是不从世界模型更新的**，因为这些是自己算出来的，不需要覆盖。
  
规则关联：规则要求场上最多5个队员，每个有自己的编号。这里的 `OUR_TEAM` 就是5。
  
### 更新障碍物和对手
  
```cpp
world_model_info_.Obstacles_.clear();
for(nubot_common::Point2d point : _world_msg.obstacleinfo.pos)
    world_model_info_.Obstacles_.push_back(DPoint(point.x, point.y));
  
world_model_info_.Opponents_.clear();
for(nubot_common::Point2d point : _world_msg.oppinfo.pos)
    world_model_info_.Opponents_.push_back(DPoint(point.x, point.y));
```
  
障碍物和对手是两个概念：
- **障碍物（Obstacles）**：场上所有需要避开的物体
- **对手（Opponents）**：对方队员的已知位置
  
更新对手位置是为了判断**推人犯规**（规则第8条）和**非法防守/进攻**（规则第8-9条）。
  
### 更新球的信息
  
```cpp
for(std::size_t i = 0 ; i < OUR_TEAM ; i++)
{
    world_model_info_.BallInfo_[i].setGlobalLocation(...);  // 全局坐标
    world_model_info_.BallInfo_[i].setRealLocation(...);     // 相对于该机器人的极坐标
    world_model_info_.BallInfo_[i].setVelocity(...);          // 速度
    world_model_info_.BallInfo_[i].setVelocityKnown(...);     // 速度是否已知
    world_model_info_.BallInfo_[i].setLocationKnown(...);     // 位置是否已知
    world_model_info_.BallInfo_[i].setValid(...);             // 是否有效
}
world_model_info_.BallInfoState_ = ...;  // 球的状态（是否可见等）
```
  
每个机器人对球有独立的观测（因为不同位置看到的球不一样），所以这里有 `OUR_TEAM` 个 `BallInfo_`。
  
这个信息关系到**规则第4条"进球得分"**中判断射门是否在对方半场完成，以及**规则第5条"高球"**中判断球的轨迹。
  
### 更新裁判指令（Coach信息）
  
```cpp
world_model_info_.CoachInfo_.MatchMode = _world_msg.coachinfo.MatchMode;
world_model_info_.CoachInfo_.MatchType = _world_msg.coachinfo.MatchType;
```
  
==这里的 `MatchMode` 就是**自动裁判盒发出的比赛模式指令**==，它决定了机器人的行为。模式值包括：
  
| 模式 | 含义 | 规则依据 |
|---|---|---|
| `STOPROBOT` | 比赛停止 | 规则3（争球）、规则12-14 |
| `STARTROBOT` | 比赛开始 | 规则2 |
| `OUR_KICKOFF` | 我方开球 | 规则2 |
| `OPP_KICKOFF` | 对方开球 | 规则2 |
| `OUR_FREEKICK` | 我方任意球 | 犯规判罚 |
| `DROPBALL` | 争球 | 规则3 |
| `PARKINGROBOT` | 停车（比赛结束）| |
  
==`MatchType` 存储的是进入 `START` 之前的**上一个模式**==，用于在开始后区分具体该执行哪种战术。
  
### 更新传球信息
  
```cpp
world_model_info_.pass_cmds_.catchrobot_id  = _world_msg.pass_cmd.catch_id;
world_model_info_.pass_cmds_.passrobot_id   = _world_msg.pass_cmd.pass_id;
...
world_model_info_.pass_cmds_.is_dynamic_pass   = _world_msg.pass_cmd.is_dynamic_pass;
world_model_info_.pass_cmds_.is_static_pass    = _world_msg.pass_cmd.is_static_pass;
```
  
传球信息记录了：谁要传球、谁要接球、传到哪。`is_static_pass` 是静态传球（死球时的固定配合），`is_dynamic_pass` 是动态传球（活球时的灵活传球）。
  
### 守门员策略数据
  
```cpp
m_strategy_->goalie_strategy_.robot_info_   = _world_msg.robotinfo[...];
m_strategy_->goalie_strategy_.ball_info_2d_ = _world_msg.ballinfo[...];
```
  
专门把当前机器人自身的信息和球的信息传给守门员策略模块。这个模块是给1号机器人（守门员）用的。
  
---
  
## 第七部分：`update_ballisholding`——吸球状态（第166-169行）
  
```cpp
void update_ballisholding(const nubot_common::BallIsHolding & ball_holding)
{
    ball_holding_.BallIsHolding = ball_holding.BallIsHolding;
}
```
  
这个回调超简单：当底层告诉"吸到球了"，就记下这个状态。后面 `handleball()` 会根据这个状态决定是否继续吸球。
  
---
  
## 第八部分：主循环 `loopControl`——大脑每15ms思考一次（第174-206行）
  
这是**整个代码的核心**，所有决策都在这里。让我画个流程图：
  
```
loopControl 每 15ms 执行一次:
    │
    ├─ 1. 更新：比赛模式、自己位置、球位置
    │
    ├─ 2. 判断模式：
    │   ├── STOPROBOT      → 停止一切动作
    │   ├── 准备阶段  (STOP < mode ≤ DROPBALL)  → positioning() 跑位
    │   ├── PARKINGROBOT   → parking() 停车
    │   └── START          → normalGame() 正常比赛
    │
    ├─ 3. handleball()     → 控制吸球装置
    ├─ 4. setEthercatCommand() → 发指令给底层
    └─ 5. pubStrategyInfo()    → 广播战术信息给队友
```
  
### 第一步：更新状态
  
```cpp
match_mode_   = world_model_info_.CoachInfo_.MatchMode;       // 当前模式
pre_match_mode_ = world_model_info_.CoachInfo_.MatchType;     // 上一个模式
robot_pos_    = ...RobotInfo_[AgentID_-1].getLocation();      // 自己坐标
robot_ori_    = ...RobotInfo_[AgentID_-1].getHead();          // 自己朝向
ball_pos_     = ...BallInfo_[AgentID_-1].getGlobalLocation(); // 球坐标
ball_vel_     = ...BallInfo_[AgentID_-1].getVelocity();       // 球速度
```
  
这里把全局数据拷贝到成员变量中，方便后续函数使用。因为后面所有函数都要频繁读取这些值。
  
### 第二步：模式分发
  
```cpp
if(match_mode_ == STOPROBOT)
{
    // 规则关联：裁判发出"停止"信号后，所有机器人必须停止运动
    action_cmd_.move_action  = No_Action;//停止平移
    action_cmd_.rotate_acton = No_Action;//停止旋转
}
else if(match_mode_ > STOPROBOT && match_mode_ <= DROPBALL)
    positioning();  // 开球、任意球、边线球、角球、争球前的跑位
else if(match_mode_ == PARKINGROBOT)
    parking();      // 比赛结束，开去停车区
else
    normalGame();   // 正常比赛
```
  
规则关联：
- **`STOPROBOT`** 对应规则中说"裁判盒给出'停止'的信号→所有机器人必须停止运动"（规则3b、12b、13、14b等）
- **准备阶段（>STOP且≤DROPBALL）**：规则2中开球前（a-d）、规则12边线球（e-g）、规则14角球（e-g）都有"除发球者外其他机器人距离球2~3米"的要求，robot需要在死球期间跑到指定位置
- **`PARKINGROBOT`**：比赛结束后机器人开到指定区域等待
- **正常比赛**：裁判发出了"开始"信号，机器人用 `normalGame()` 执行完整的进攻/防守逻辑
  
---
  
## 第九部分：`positioning()`——死球跑位（第208-250行）
  
```cpp
void positioning()
{
    switch (match_mode_)
    {
    case OUR_KICKOFF:          // 我方开球
    case OUR_FREEKICK:         // 我方任意球
    case OUR_GOALKICK:         // 我方球门球
    case OUR_CORNERKICK:       // 我方角球
    case OUR_THROWIN:          // 我方边线球
    case OUR_PENALTY:          // 我方点球
    case DROPBALL:             // 争球
        OurDefaultReady_();    // 统一用"我方发球站位"
        break;
  
    case OPP_KICKOFF:          // 对方开球
    case OPP_FREEKICK:         // 对方任意球
    case OPP_GOALKICK:         // 对方球门球
    case OPP_CORNERKICK:       // 对方角球
    case OPP_THROWIN:          // 对方边线球
    case OPP_PENALTY:          // 对方点球
        OppDefaultReady_();    // 统一用"对方发球站位"
        break;
    }
}
```
  
这个函数区分两种情况：
  
1. **我方发球**（`OUR_*`）→ 采用进攻性站位——适当压上，准备接球配合
2. **对方发球**（`OPP_*`）→ 采用防守性站位——退回自己半场，保持安全距离
  
规则关联：
- 规则2a：开球时"除发球机器人外，所有机器人都在自己半场"
- 规则2b："非开球一方的机器人必须距离球3米以上"
- 规则2d："开球方其他机器人在球进入比赛状态之前，都必须距离球2米以上"
- 规则12g（边线球）和14g（角球）也有类似的2米/3米距离要求
  
---
  
## 第十部分：`OppDefaultReady_()`——对方发球时的防守站位（第252-273行）
  
```cpp
void OppDefaultReady_()
{
    DPoint target;
    DPoint br = ball_pos_ - robot_pos_;
    switch(world_model_info_.AgentID_)
    {
    case 1:  // 守门员
        target = DPoint(-1050.0, 0.0);   // 在自家球门前
        break;
    case 2:
        target = DPoint(-200.0, 100.0);  // 退到己方半场左侧
        break;
    case 3:
        target = DPoint(-200.0, -100.0); // 退到己方半场右侧
        break;
    case 4:
        target = DPoint(-550.0, 200.0);  // 更靠后防守
        break;
    case 5:
        target = DPoint(-550.0, -200.0); // 更靠后防守
        break;
    }
  
    // 如果目标站位离球太近（<300mm），且不在己方禁区，就往远离球的方向挪320mm
    if(target.distance(ball_pos_) < 300
       && !world_model_info_.field_info_.isOurPenalty(target))
        target = ball_pos_.pointofline(target, 320.0);
  
    // 先走到目标位置，如果到了就转向球的方向
    if(move2target(target, robot_pos_))
        move2ori(br.angle().radian_, robot_ori_.radian_);
  
    action_cmd_.move_action  = Positioned_Static;  // 静态定位移动
    action_cmd_.rotate_acton = Positioned_Static;  // 静态定位旋转
    action_cmd_.rotate_mode  = 0;
}
```
  
这是**对方发球时的防守站位方案**：
  
**各个机器人的位置**（坐标系：x正方向为对方半场，y正方向为上方）：
- **1号守门员**：站在球门中间（-1050, 0），守住球门
- **2号**：半场内左侧防守位置（-200, 100）
- **3号**：半场内右侧防守位置（-200, -100）
- **4号**：更深的后场左侧（-550, 200）
- **5号**：更深的后场右侧（-550, -200）
  
规则关联：规则2b要求"非开球一方的机器人必须距离球3米以上"，这里所有机器人目标站位都远离球。
  
`if(target.distance(ball_pos_)<300 && ...)` 这个判断就是**保证目标点不侵犯"距离球3米"这一规则**（单位是毫米，300mm = 0.3m，不过这里的逻辑有点奇怪，300mm远小于3米，疑似bug或单位换算问题）。
  
`!isOurPenalty(target)` 确保目标站**不在己方罚球区内**（规则第9条非法防守限制）。
  
---
  
## 第十一部分：`OurDefaultReady_()`——我方发球时的进攻站位（第274-292行）
  
```cpp
void OurDefaultReady_()
{
    DPoint br = ball_pos_ - robot_pos_;
    DPoint target;
    switch(world_model_info_.AgentID_)
    {
    case 1:  // 守门员
        target = DPoint(-1050.0, 0.0);      // 在球门前
        break;
    case 2:
        target = ball_pos_.pointofline(robot_pos_, 100.0);  // 在球后方100mm
        break;
    case 3:
        target = ball_pos_.pointofline(robot_pos_, 200.0);  // 在球后方200mm
        break;
    case 4:
        target = DPoint(-550.0, 200.0);
        break;
    case 5:
        target = DPoint(-550.0, -200.0);
        break;
    }
  
    // 走到目标位置，到了就转向球
    if(move2target(target, robot_pos_))
        move2ori(br.angle().radian_, robot_ori_.radian_);
    action_cmd_.move_action  = Positioned_Static;
    action_cmd_.rotate_acton = Positioned_Static;
    action_cmd_.rotate_mode  = 0;
}
```
  
我方发球时：
- **1号守门员**：依然守门
- **2号**：紧贴在球后方100mm，准备接球配合
- **3号**：在球后方200mm，稍远一点策应
- **4/5号**：在中场两侧，保持阵型
  
规则关联：规则2d要求"开球方的其他机器人在球进入比赛状态之前，都必须距离球2米以上"。这里的站位并不遵守这个2米距离要求，开发者在注释中自己也写了"站位还需要考虑是否犯规，但是现在这个程序没有考虑"——说明这是一个简化的实现，实际比赛中需要更精确地遵守规则距离。
  
---
  
## 第十二部分：`parking()`——比赛结束停车（第294-309行）
  
```cpp
void parking()
{
    static double parking_y = -680.0;  // 停车线的y坐标
    DPoint parking_target;
    float tar_ori = SINGLEPI_CONSTANT / 2.0;  // 朝向正上方
  
    // 每个机器人按编号排开
    parking_target.x_ = FIELD_XLINE7 + 150.0 * world_model_info_.AgentID_;
    parking_target.y_ = parking_y;
  
    // 如果还没到，就开车过去；到了就转向
    if(move2target(parking_target, robot_pos_))
        move2ori(tar_ori, robot_ori_.radian_);
  
    action_cmd_.move_action  = Positioned_Static;
    action_cmd_.rotate_acton = Positioned_Static;
    action_cmd_.rotate_mode  = 0;
}
```
  
比赛结束后，机器人按编号排成一列开到边线附近。`FIELD_XLINE7` 是场地上某条x坐标线，每个机器人偏移150mm实现间隔排列。
  
---
  
## 第十三部分：`isNearestRobot()`——判断谁离球最近（第311-327行）
  
```cpp
bool isNearestRobot()
{
    float distance_min = 2000.0;
    float distance = distance_min;
    int robot_id = -1;
  
    for(int i = 1; i < OUR_TEAM; i++)   // 从1开始，排除守门员(i=0)
        if(world_model_info_.RobotInfo_[i].isValid())
        {
            distance = ball_pos_.distance(
                world_model_info_.RobotInfo_[i].getLocation());
            if(distance < distance_min)
            {
                distance_min = distance;
                robot_id = i;
            }
        }
  
    // 机器人编号从1开始，数组下标从0开始，所以要+1
    if(robot_id + 1 == world_model_info_.AgentID_)
        return true;
    else
        return false;
}
```
  
这个函数判断**当前机器人是不是场上离球最近的（排除守门员）**。
  
逻辑：
1. 遍历编号2~5的机器人（数组下标1~4）
2. 对每个有效的机器人，计算它到球的欧几里得距离
3. 找到距离最小的那个
4. 如果最小的那个就是自己 → 返回 true
  
规则关联：规则没有直接规定"离球最近就去抢球"，但在 RoboCup 比赛中，普遍采用这种"最近者负责"的角色分配策略，可以有效避免多个机器人争抢同一个球造成碰撞犯规（规则8b）。
  
---
  
## 第十四部分：`normalGame()`——正常比赛的核心逻辑（第329-385行）
  
这是**整段代码最核心的部分**，实现了"抢球→带球→射门"的完整链条。
  
```cpp
void normalGame()
{
    static bool last_dribble = 0;  // 上次的带球状态，用于调试
    isactive = false;
  
    // 如果我不是守门员且离球最近 → 我是活跃角色
    if(world_model_info_.AgentID_ != 1 && isNearestRobot())
        isactive = true;
```
  
首先决定"谁是主角"。规则第8-9条关于非法防守/进攻规定了机器人不能长时间在禁区内，所以这里合理地将1号（守门员）排除出进攻任务。
  
### 情况A：我是活跃角色且没有射门冷却
  
```cpp
if(isactive && !shoot_flag)
{
    DPoint b2r = ball_pos_ - robot_pos_;     // 球到机器人的向量
    DPoint tmp(200.0, 300.0);                 // 目标射门位置
    DPoint t2r = tmp - robot_pos_;            // 目标点到机器人的向量
    DPoint shoot_line = oppGoal_[GOAL_MIDDLE] - robot_pos_; // 射门线
```
  
准备阶段：计算球相对自己的方向、预设的射门位置、射门朝向。
  
#### 阶段1：还没控到球 → 抢球
  
```cpp
if(!world_model_info_.RobotInfo_[...].getDribbleState())
{
    action_cmd_.handle_enable = 1;     // 打开吸球装置
    if(move2ori(b2r.angle().radian_, robot_ori_.radian_))  // 先转向球
        move2target(ball_pos_, robot_pos_);                // 再走向球
    action_cmd_.move_action  = CatchBall;
    action_cmd_.rotate_acton = CatchBall;
    action_cmd_.rotate_mode  = 0;
}
```
  
策略很直观：**先转向球的方向，然后朝球移动**。注意 `if(move2ori(...))` 这个设计——只有方向对准了（误差小于8°），才做平移运动。这避免了"一边转一边走"导致走歪。
  
`handle_enable = 1` 打开吸球装置，这对应机器人实际硬件上的真空泵或电磁铁，用于吸住球。
  
#### 阶段2：控到球了但没到射门位置 → 带球跑
  
```cpp
else if(robot_pos_.distance(tmp) > 30.0)
{
    action_cmd_.move_action  = MoveWithBall;
    action_cmd_.rotate_acton = MoveWithBall;
    action_cmd_.rotate_mode  = 0;
    if(move2ori(t2r.angle().radian_, robot_ori_.radian_))
        move2target(tmp, robot_pos_);
}
```
  
控到球后，机器人朝预设射门位置（200, 300）移动。同样遵循"先转向目标点，再走直线"的策略。
  
**规则第7条"关于带球"**：规则规定"带球距离不能超过3米"，这里带球到（200, 300），而球初始时可能在场地中央附近，所以带球距离在合理范围内。如果带球距离超过3米，裁判盒应该判犯规。
  
#### 阶段3：到达射门位置 → 转身射门
  
```cpp
else
{
    action_cmd_.move_action  = TurnForShoot;
    action_cmd_.rotate_acton = TurnForShoot;
    action_cmd_.rotate_mode  = 0;
    move2target(tmp, robot_pos_);
    move2ori(shoot_line.angle().radian_, robot_ori_.radian_);
  
    // 检查朝向是否在对方球门的有效角度范围内
    double up_radian_  = (oppGoal_[GOAL_MIDUPPER] - robot_pos_).angle().radian_;
    double low_radian_ = (oppGoal_[GOAL_MIDLOWER] - robot_pos_).angle().radian_;
    if(robot_ori_.radian_ > low_radian_ && robot_ori_.radian_ < up_radian_)
    {
        action_cmd_.shootPos = RUN;  // 贴地射门
        action_cmd_.strength = shoot_line.length() / 100;  // 力量与距离正比
        if(action_cmd_.strength < 3.0)
            action_cmd_.strength = 3.0;  // 最小力量3
        shoot_flag = true;
        std::cout << "shoot done" << std::endl;
    }
}
```
  
这一段对应了**规则第4条"进球得分"**——"在对方半场完成射门时才有效"。
  
- `GOAL_MIDDLE` 是球门中心，`GOAL_MIDUPPER` 和 `GOAL_MIDLOWER` 是球门上下门柱
- 用 `up_radian_` 和 `low_radian_` 计算从机器人位置看球门的角度范围
- 只有当机器人的朝向在球门的角度范围内时，才执行射门——**防止打歪**
- `shootPos = RUN`（贴地射门），如果是 `FLY` 则是挑射，但规则第5条对高球有限制
  
规则关联：规则4b要求"射门必须在对方半场完成"。这里没有显式检查是否在对方半场，说明这是一个简化版本。
  
### 情况B：我不是活跃角色 或 处于射门冷却期
  
```cpp
else
{
    action_cmd_.move_action  = No_Action;
    action_cmd_.rotate_acton = No_Action;
  
    if(shoot_flag)
        shoot_count++;
    if(shoot_count > 20)  // 20 × 15ms = 300ms 冷却
    {
        shoot_count = 0;
        shoot_flag = false;
    }
}
```
  
不是活跃角色的机器人先原地待命。如果刚射过门，冷却 300ms 后允许再次射门，防止重复射门。
  
---
  
## 第十五部分：`handleball()`——吸球控制（第387-394行）
  
```cpp
void handleball()
{
    if(isactive && match_mode_ == STARTROBOT && !shoot_flag)
        action_cmd_.handle_enable = 1;  // 活跃且比赛进行中 → 开吸球
    else
        action_cmd_.handle_enable = 0;  // 否则关闭
}
```
  
吸球装置只在以下**三个条件同时满足**时开启：
1. 我是活跃角色（离球最近）
2. 比赛正在进行（`STARTROBOT`）
3. 没有处于射门冷却（`!shoot_flag`）
  
比赛停止（`STOPROBOT`）时关闭吸球，是防止在死球时违规触球（规则2n、12o等规定"非发球机器人在球进入比赛状态之前不允许接触球"）。
  
---
  
## 第十六部分：`move2target()` 和 `move2ori()`——基础运动控制（第396-415行）
  
```cpp
bool move2target(DPoint target, DPoint pos, double distance_thres = 20.0)
{
    action_cmd_.target.x = target.x_;           // 设置目标位置
    action_cmd_.target.y = target.y_;
    action_cmd_.maxvel = pos.distance(target);  // 速度与距离成正比
    if(pos.distance(target) > distance_thres)   // 离目标 > 20mm → 还没到
        return false;
    else
        return true;
}
  
bool move2ori(double target, double angle, double angle_thres = 8.0*DEG2RAD)
{
    action_cmd_.target_ori = target;           // 设置目标朝向
    action_cmd_.maxw = fabs(target - angle) * 2;  // 角速度与角度差成正比
    if(fabs(target - angle) > angle_thres)     // 误差 > 8° → 还没到
        return false;
    else
        return true;
}
```
  
这两个函数**同时设置动作指令和判断是否到达**，返回值用于给调用者提供"是否到达"的反馈。
  
关键设计：
- **速度随距离变化**：离目标越近速度越慢，到目标附近自然减速（类似简单的P控制）
- **先转向再移动**：`normalGame()` 中用 `if(move2ori(...)) move2target(...)` 确保转对方向才出发
- 位置精度 20mm，角度精度 8°——对于 RoboCup 足球场（12m×8m）来说，20mm 精度足够
  
---
  
## 第十七部分：`setEthercatCommand()`——发出最终指令（第417-454行）
  
```cpp
void setEthercatCommand()
{
    nubot_common::ActionCmd command;
    // 先初始化为空（安全默认值）
    command.move_action  = No_Action;
    command.rotate_acton = No_Action;
    command.rotate_mode  = 0;
    command.maxvel = 0;
    command.maxw   = 0;
    command.target_w = 0;
  
    // 填充机器人当前状态
    command.robot_pos = ...;
    command.robot_vel = ...;
    command.robot_ori = ...;
    command.robot_w   = ...;
  
    // 填充运动指令
    command.move_action  = action_cmd_.move_action;
    command.rotate_acton = action_cmd_.rotate_acton;
    command.target       = action_cmd_.target;
    command.target_ori   = action_cmd_.target_ori;
    command.maxvel       = action_cmd_.maxvel;
    command.maxw         = action_cmd_.maxw;
  
    // 速度限幅
    if(command.maxvel > MAXVEL) command.maxvel = MAXVEL;
    if(command.maxw   > MAXW)   command.maxw   = MAXW;
    if(fabs(command.target_ori) > 10000.0) command.target_ori = 0;
  
    // 吸球和射门
    command.handle_enable = action_cmd_.handle_enable;
    command.strength = action_cmd_.strength;
    command.shootPos = action_cmd_.shootPos;
  
    // 射完一次后力量清0，防止连续射门
    action_cmd_.strength = 0;
  
    action_cmd_pub_.publish(command);  // 发出！
}
```
  
这是**最后一步**：把所有计算好的动作指令打包成一个 `ActionCmd` 消息，通过 ROS 发出去，底层硬件（EtherCAT 总线连接的电机驱动器）收到后执行实际运动。
  
"EtherCAT" 是一种工业实时以太网通信协议，常用于机器人关节控制。
  
几个关键安全检查：
- **速度限幅**：保证不超过硬件最大速度
- **异常朝向清零**：如果奇怪的角度值超过10000弧度，直接清0
- **力量清0**：每次射门后把力量置0，防止下一周期重复射门
  
---
  
## 第十八部分：`pubStrategyInfo()`——广播战术信息（第456-464行）
  
```cpp
void pubStrategyInfo()
{
    nubot_common::StrategyInfo strategy_info;
    strategy_info.header.stamp = ros::Time::now();
    strategy_info.AgentID      = world_model_info_.AgentID_;
    strategy_info.is_dribble   = ball_holding_.BallIsHolding;
    strategy_info_pub_.publish(strategy_info);
}
```
  
每个机器人把自己的状态广播给全队，让队友知道：我是几号、我有没有控到球。
  
这个信息是**多机器人协作**的基础——比如2号可以知道3号已经控球了，就不用再跑过去抢球了。
  
---
  
## 第十九部分：`main()` 入口（第467-477行）
  
```cpp
int main(int argc, char **argv)
{
    ros::init(argc, argv, "nubot_control_node");  // 注册ROS节点
    ros::Time::init();                             // 初始化时间系统
    ROS_INFO("start control process");
    nubot::NuBotControl nubotcontrol(argc, argv);  // 创建大脑
    ros::spin();                                   // 进入事件循环
    return 0;
}
```
  
`ros::spin()` 之后程序不再往下走，而是进入无限循环——**等待消息到来（订阅回调）、等待定时器触发（主循环）**。整个过程完全由 ROS 的事件机制驱动。
  
---
  
## 总结：代码与规则的对应关系
  
| 代码位置 | 功能 | 对应规则 |
|---|---|---|
| `match_mode_` 分发 | 判断比赛模式 | 规则2-14的裁判信号 |
| `STOPROBOT` 处理 | 停止运动 | 规则3b |
| `positioning()` 跑位 | 发球/罚球前站位 | 规则2a-e, 12e-g, 14e-g |
| 球距离检查（`isOurPenalty`） | 避开己方罚球区 | 规则8（非法防守） |
| `isNearestRobot()` | 选离球最近的机器人 | 规则8b（减少碰撞） |
| 三段式进攻 | 抢球→带球→射门 | 规则7（带球≤3m）、规则4（射门有效） |
| `shootFlag` 冷却 | 防止重复射门 | 规则4c（需要另一队友触球后有效） |
| `handleball()` | 控制吸球装置 | 规则2n（死球时不能触球）|
| `RUN` vs `FLY` | 贴地/高球射门 | 规则5（高球限制） |
| 速度限幅 | 安全保护 | 规则8b（避免高速碰撞） |
  
整体来看，这份代码是一个**功能完整但策略简单**的机器人控制程序。它清楚地区分了比赛的不同阶段，并能执行基本的"抢球→带球→射门"行为，但在规则遵守上（如开球时2米距离、射门必须在对方半场等）还有待完善。正如开发者在注释中所说——**"十分简单的实现，固定的站位，建议动态调整站位"**。
  
  