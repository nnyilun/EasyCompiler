#pragma once

#include "common.h"
#include "symbol.h"

/*
    符号表
*/

class SymTab {
private:
    // 内部数据结构
    std::unordered_map<std::string, std::shared_ptr<std::vector<std::shared_ptr<Var>>>> varTab; // 变量表
    std::unordered_map<std::string, std::shared_ptr<Var>> strTab; // 字符串常量表(串表)
    std::unordered_map<std::string, std::shared_ptr<Func>> funcTab; // 函数表

    // 辅助分析数据记录
    std::shared_ptr<Func> currentFunc; // 当前分析的函数
    int scopeId; // 作用域编号
    std::vector<int> scopePath; // 作用域路径

    // 中间代码生成器
    // GenIR* ir;

public:
    
};

