#pragma once

#include "common.h"
#include "lexer.h"
#include "token.h"
#include <error.h>

/*
    语法分析器
*/

class Parser {
private:
    // 储存lexer
    std::optional<std::reference_wrapper<Lexer>> _lexer;
    // 储存当前得到但未匹配的token
    std::shared_ptr<Token> unmatchToken;

    template <Tag TagValue>
    constexpr bool isTagEqual();

    template <Tag... Tags>
    constexpr bool anyTagEqual();

    void getNextToken(); // 获取下一个token
    bool matchTag(Tag need); // 匹配是否是需要的Tag，如果是则获取下一个token
    void recovery(bool condition, SynError lost, SynError wrong); // 错误恢复函数

    // 文法开始
    void program();
    void segment();

    // 声明与定义
    void init();

    // 函数 
    

    // 语句
    void statement();
    void whilestat(); // 循环语句文法
    void dowhilestat(); // dowhile语句
	void forstat(); // for语句
	void forinit(); // for初始化部分
	void ifstat(); // if语句
    void elsestat(); // else语句
	void switchstat(); // switch语句

    // 表达式
    void altexpr();
    void expr();

public:
    Parser() = delete;
    Parser(Lexer& lexer);
    virtual ~Parser() = default;

	void analyze(); // 语法分析主程序
};