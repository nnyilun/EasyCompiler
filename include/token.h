#pragma once

/*
    词法分析器标记
*/

#include "common.h"

// 启用复杂的数据类型
// #define COMPLEX_DATA

// 词法标记枚举变量
// 设计词法标记的时候试图尽可能囊括相同符号的不同含义，如*既是乘号，也是间接访问操作符，&既是按位与运算，也是取地址。
// 可是这些含义在词法分析和语法分析中不能完全区分开来，所以部分变量用不到
enum class Tag
{
	ERR,						    // 错误，异常
	END,					        // 文件结束标记
	ID,							    // 标识符
	NUM,CH,STR,					    // 字面量
	
	KW_INT, KW_CHAR, KW_VOID,		// 数据类型 int char void

#ifdef COMPLEX_DATA
    // 数据类型 short, long, long long int, float, double, long double (未实现)
    KW_SHORT, KW_LONG, KW_LLINT, KW_FLOAT, KW_DOUBLE, KW_LDOUBLE, 
    KW_STRUCT, // 结构体
#endif

    KW_EXTERN,					    // extern
	COMMA, COLON, SEMICON,		    // 逗号,冒号,分号
	ASSIGN,						    // 赋值
	KW_IF, KW_ELSE,				    // if-else
	KW_SWITCH, KW_CASE, KW_DEFAULT,	// swicth-case-deault
	KW_WHILE, KW_DO, KW_FOR,		// 循环
	KW_BREAK, KW_CONTINUE, KW_RETURN,// break,continue,return
	
    FAO, IAO,                       // 取地址符和间接访问操作符
    
    BNOT, BAND, BOR,				// 位运算 ~ & |
	
    ADD, SUB, MUL, DIV, MOD,		// 算术运算符 + - * / % 
	INC, DEC,                       // 自增，自减
	
    GT, GE, LT, LE, EQU, NEQU,		// 比较运算符 > >= < <= == !=
	
    LNOT, LAND, LOR,				// 逻辑运算 ! && || 
	
    LPAREN, RPAREN,				    // ()
	LBRACK, RBRACK,				    // []
	LBRACE, RBRACE,				    // {}
};

/*
    以下为标识符词法记号的硬编码实现
*/

// 词法记号类
class Token {
public:
    Tag tag;
    Token(Tag t);
    virtual std::string toString();
    virtual ~Token() = default;
};

// 标识符记号类
class Id : public Token {
public:
    std::string name;
    Id(std::string n);
    virtual std::string toString();
};

// 数字记号类
class Num : public Token {
public:
    int val;
    Num(int v);
    virtual std::string toString();
};

// 字符记号类
class Char : public Token {
public:
    char ch;
    Char(char c);
    virtual std::string toString();
};

// 字符串记号类
class Str : public Token {
public:
    std::string str;
    Str(std::string s);
    virtual std::string toString();
};

// 关键字表类
class Keywords {
private:
    std::unordered_map<std::string, Tag> keywords;
public:
    Keywords(); // 构造时初始化关键字表
    Tag getTag(std::string name); // 测试是否是关键字
};