﻿#pragma once
#include <functional>
#include <string>

//C++匈牙利表示法，允许组合使用
//W 宽，如wchar_t,以处理Unicode中utf-16,注意现在都用这个
//L 长，x16时代的产物，现在不带也是长
//P 指针
//H 句柄
//LP 长指针
//C 常量

//C++特殊函数后缀，代表函数的不同版本
// 
//函数中使用的字符类型
//a ASCII编码
//W 宽字符编码，咱中文就用这个
// 
//一般表明颜色存储方式
//U	无符号整数 如：0-255
//F 浮点数 如：0.0-1.0

//C++数据类型
//PCWSTR = 常量宽字符指针,标准字符串
//LRESULT = long int，表示函数的返回代码
//HWND = 窗口句柄
//BOOL = int
//WPARAM = uint
//LPARAM = int


namespace BDXKEngine
{
    inline std::vector<std::function<void()>> engineBeginEvents = {};
    inline std::vector<std::function<void()>> engineEndEvents = {};;

    void Run(const std::string& sceneName);
    void Run(const std::function<void()>& onStart);
    void Run();//加载或创建名为Main.scene的场景，该场景是游戏的入口，类似于代码中的main函数
    void Quit();//退出程序。编辑器模式下无效，因为编辑器通过AddQuitCondition函数，抑制了退出功能

    void AddQuitCondition(const std::function<bool()>& isOver);

    struct EngineEventRegister
    {
        EngineEventRegister(std::vector<std::function<void()>>& eventType, const std::function<void()>& event)
        {
            eventType.emplace_back(event);
        }
    };

#define CustomEngineBeginEvent(func) EngineEventRegister engineBeginEventRegister = {BDXKEngine::engineBeginEvents,func};
#define CustomEngineEndEvent(func) EngineEventRegister engineEndEventRegister = {BDXKEngine::engineEndEvents,func};
}
