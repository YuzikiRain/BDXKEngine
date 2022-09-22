# BDXKEngine
我在试图自己开发一个游戏引擎，当然纯造轮子玩的那种。

## 开发范围：
软件之内，标准之上。

## 开发规则：
### 1. 不要造标准库的轮子
能用C++标准库就用标准库
### 2. 不要造行业标准的轮子
举例：
* 与硬件交互，如图形，声音，设备输入输出
* 与操作系统交互，如窗口，COM组件
* 标准数据的读写交换，如fbx，json，http
### 3. 第三方库除了以上情况一律禁用

## 外部依赖：
* rapidjson
* Window 10 SDK
* ATL

## 注意事项：
由于Github有上传大小限制，而部分第三方库过于庞大，因此需要手动配置  
