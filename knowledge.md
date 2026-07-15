# <center><font face ="仿宋" font color=red size=10>Markdown 入门</font>
# 一.基本语法
## 1. **标题**
># 一级标题
>## 二级标题
>### 三级标题
>#### 四级标题
>##### 五级标题
>###### 六级标题
***
## 2. **引用**
>文本前添加一个大于号即可引用
>>几级就在前面加几个大于号
>>>可层层嵌套
***
## 3. **列表**
   1. 无序列表（ +  -  * 都可用，前面是·）
   + 列表1
   - 列表2
   * 列表3
   2. 有序列表（前面有序号）
   3. Todo list
    - [x] 待办1
    - [ ] 待办2
    - [ ] 待办3
***
## 4. **表格**
>Markdown 规则：
   标题 / 有序列表 和 表格之间必须空一行，否则渲染器会把表格识别成普通文本，不会生成表格。

  | 人数 | 年龄 | 性别 |
  | :-- | :--: | --: |
  |左对齐|居中对齐|右对齐|
  | a | b | c |
  >在表格的第二行写对齐方式的语法，整列即可按照相应方式对齐
***
## 5. **段落**
   + 换行：两个以上的空格再回车/空一行
   + 分割线：三个以上的星号
***
## 6. **字体**

| 字体  |  代码  |
| :---: | :----: |
| 斜体  | *文本* |
| 粗体|**文本**|
|斜粗体|***文本***|
|高亮|==文本==|
|下划线|<u>文本</u>|
|删除线|~~文本~~|

>### 1.上标
一次方：x<sup>1</sup>
平方：x<sup>2</sup>
立方：x<sup>3</sup>
>### 2.下标：
H<sub>2</sub>O
***
## 7.代码
`print("hello world!")`  (单句代码左右各一个英文小点括起来)
```
#include <bits/stdc++.h>
using namespace std;
int main(){
    cout<<"hello world"<<endl;
}
```
(整段代码前后各三个英文小点括起来)
***
## 8.图片
  > [路过图床]https://imgse.com/

![示例图片](https://example.com/image.png)
+ 图片尺寸控制
<div align=center><img src="https://example.com/image.png" width="300" height="200" alt="示例图片"></div>

## 9.超链接
[GitHub](https://github.com)
[百度搜索](https://www.baidu.com)
## 10.脚注
这是一个脚注示例[^1]，用于添加额外说明。

[^1]: 这是脚注的内容，会显示在文档末尾。
Markdown 由 John Gruber 创建[^gruber]，并在 GitHub 等平台得到广泛应用[^github]。

[^gruber]: John Gruber 是一位作家和软件开发者，也是 Daring Fireball 博客的创始人。
[^github]: GitHub 是全球最大的代码托管平台，全面支持 Markdown 语法。
# 二.其他操作
## 1.插入latex公式
+ 行内公式
  $f(x)=ax+b$
(公式左右两边用$括起来)
+ 块级公式：
$$
\int_{a}^{b} f(x) \, dx = F(b) - F(a)
$$
(左右各用两个$$)
## 2.html/css语法
(可见标题样式)
>对一三级标题居中对齐
![alt text](image-1.png)
![alt text](image.png)
# 三.导出为PDF
点击预览右下角的三条横线那里的==Open in Browser==到浏览器，再点击右键另存为，右键点击另存为pdf即可
![alt text](image-3.png)
+ 浏览器没有另存为PDF解决方法：右键点击“==打印==”，打印机选择“另存为PDF”，点击“保存”即可