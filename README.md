# BDXKEngine
我在试图自己开发一个游戏引擎，当然纯造轮子玩的那种。
## 开发范围：
软件之内，标准之上。
## 开发规则：
1.能用C++标准库就用标准库，不要造标准库的轮子

2.第三方库除了以下情况一律禁用，即不要造行业标准的轮子
* 与硬件交互，如图形，声音，设备输入输出
* 与操作系统交互，如窗口，COM组件
* 标准数据的读写交换，如fbx，json，http

## 第三方依赖
由于Github有上传大小限制，而部分第三方库过于庞大，因为只能手动配置
目前使用的第三方库有：
* DirectX
* FBX SDK
