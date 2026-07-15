## 一. 一些语法
+ \n 表示换行
+ 每个print默认另起一行
+ 三个"""可以实现多内容跨行操作
  > """文本"""

  要是前面不加任何东西，就是注释
  注释还可以用# 
+ Python 中 "+" 对字符串的作用是字符串拼接，只会把文本直接连在一起，不会自动添加空格。
`print("你好"+"这是一句代码"+"哈哈")`
   > 你好这是一句代码哈哈
+ 如果字符串用单引号包起来，想在字符串内写一个单引号，就要用反斜杠\转义，写成\'
  `print("I\'m student!")`
  >I'm student!
+ 字母全部小写，不同单词用下划线分隔
  >==下划线命名法==
  > user_name  user_age
## 二. 数学运算
+ 计算2的三次方 
  ```
  s=2**3;
  print(s)
  ```
+ python的math库函数
  ```
  import math
  print(math.sin(1))
  print(math.log2(8))
  ```
+ 计算平方根
  ```
  import math
  print(math.sqrt(x))
  ```
  或者直接写成
  `print(x**1/2)`
## 三. 数据类型
### 1.字符串str
   1. len('\n')字符串长度==1==
   2. 索引"lucky"[3]得到"k"
   + 索引是从0开始的
### 2.整数int 浮点数float
### 3.布尔类型bool
+ ==T==rue 和 ==F==alse首字母必须严格大写
### 4.空值类型NoneType
`my_love=None` ***==N==要大写***
```
print(type(10))          # <class 'int'> 整数
print(type(3.14))        # <class 'float'> 浮点数
print(type("abc"))       # <class 'str'> 字符串
print(type(True))        # <class 'bool'> 布尔值
print(type([1,2,3]))     # <class 'list'> 列表
print(type({"a":1}))     # <class 'dict'> 字典
```
## 四. Python 交互模式
> + 平时用的是<u>命令行模式</u>
> + <u>Python 交互模式</u>:输入一行代码，<u>立刻执行一行、马上出结果</u>，不用新建.py文件，适合临时测试小段代码、查语法。
 可以不用写==print==就可以输出
### 方法 1：命令行打开（Windows PowerShell / CMD / Linux 终端通用，最常用）
+ 打开终端（PowerShell、CMD、WSL、Mac 终端都行）
+ 输入命令，回车：
python
+ 出现 >>> 符号，代表进入交互模式成功
+ 示例界面：
![alt text](image-4.png)
### 方法 2：Windows 自带 Python 终端
+ 开始菜单搜索 Python，点击程序直接打开，自带交互窗口。
## 五. 输入
input一律返回字符串
可以使用int()float()之类的进行转型
>==example==
```
user_weight=float(input("请输入你的体重：(单位:kg)"))
user_height=float(input("请输入你的身高：单位:m"))
user_BMI=user_weight/user_height**2
print("您的BMI值为:"+str(user_BMI))
```
## 六. 条件语句
### 1.仅一个条件判断
> if条件：
    [执行语句]
    [执行语句]
else
    [执行语句]

>==example==
```
is_happy=int(input("今天你开心吗？"))
if is_happy>=60:
    print("开心")
else:
    print("不开心")
```
### 2.嵌套/多条件判断
1.嵌套
<u>python的缩进为4个空格</u>

> if条件1：
      if条件2：
          [语句a]
  else
      [语句b]

2.多条件判断
> if条件一:
      [语句A]
  elif条件二:
      [语句B]
  elif条件三:
      [语句C]
  else:
      [语句D]

>==example==
```
user_weight=float(input("请输入你的体重：(单位:kg)"))
user_height=float(input("请输入你的身高：单位:m"))
user_BMI=user_weight/user_height**2
print("您的BMI值为:"+str(user_BMI))

if user_BMI<=18.5:
    print("此BMI值属于偏瘦范围")
elif 18.5<user_BMI<=25:
    print("此BMI值属于正常范围")
elif 25<user_BMI<=30:
    print("此BMI值属于偏胖范围")
else:
    print("此BMI值属于肥胖范围")
```
> python中可以正常用数学公式的写法来写：==18.5<user_BMI<=25==