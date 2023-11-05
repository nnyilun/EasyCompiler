#pragma once

/*
    词法分析器
*/

#include "common.h"
#include "token.h"
#include "scanner.h"
#include "error.h"

class Lexer {
private:
    static Keywords keywords; // 关键字表
    Scanner& _scanner; // 扫描器
    std::shared_ptr<Token> tokenPtr; // 共享的token变量，记录扫描到的词法记号
    char chr; // 储存当前扫描到的字符

    // 符号表，用于判断界符 (在构造函数中初始化)
    const std::unordered_map<char, std::function<Tag()>> symbolList;

    void scan(); // 扫描字符

    // 获取词法记号并创建相关的Token储存到tokenPtr
    // 返回的值可能为识别错误后返回的值，使用前需判断是否有Tag::ERR
    int getValue(); // 数字常量
    char getChar(); // 字符常量
    std::string getStr(); // 字符串常量
    std::string getTag(); // 标识符或关键字
    Tag getSymbol(); // 界符

    // 在显示词法记号模式下，输出识别到的词法记号 
    void printToken();
    
public:
    Lexer() = delete;
    Lexer(Scanner& scanner);
    virtual ~Lexer() = default;

    std::shared_ptr<Token> tokenize(); // 解析词法记号，调用子状态机函数
};