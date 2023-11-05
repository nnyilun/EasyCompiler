#pragma once

/*
    使用到的标准库和颜色常量
*/

#include "format"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <string>
#include <vector>
#include <array>
#include <queue>
#include <list>
#include <unordered_map>
#include <optional>
#include <functional>
#include <new>
#include <memory>
#include <cctype>
#include <concepts>
#include <ranges>
#include <string_view>
#include <algorithm>
#include <exception>

const std::string RESET = "\033[0m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";


// 编译参数，通过命令行传入
class Args {
public:
    static std::string fileName; // 文件名
	static bool showChar; // 显示字符
	static bool showToken; // 显示词法记号
	static bool showSym; // 显示符号表
	static bool showIC; // 显示中间代码
	static bool showOC; // 显示优化后的中间代码
	static bool showBlock; // 显示基本块和流图关系
	static bool showHelp; // 显示帮助
	static bool opt; // 是否执行优化
};