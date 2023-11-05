#pragma once

#include "common.h"
#include "token.h"
#include <memory>

/*
    变量和函数的结构
*/

// 变量
class Var {
    // 特殊标记
    bool literal; // 是否为常量
    std::vector<int> scopePath; // 作用域路径

    // 基本声明
    bool externed; // 是否extern声明
    Tag type; // 变量类型
    std::string name; // 变量名
    bool isPtr; // 是否是指针
    bool isArray; // 是否是数组
    int arraySize; // 数组长度

    // 变量初始值
    bool isLeftValue; // 是否是左值
    std::shared_ptr<Var> initData; // 初始值数据
    

};

// 函数
class Func {

};