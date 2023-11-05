#pragma once

/*
    报错和警告信息
*/

#include "common.h"
#include "scanner.h"
#include "token.h"

// 词法错误
enum class LexError
{
	STR_NO_R_QUTION,		// 字符串没有右引号
	NUM_BIN_TYPE,				// 2进制数没有实体数据
	NUM_HEX_TYPE,				// 16进制数没有实体数据
	CHAR_NO_R_QUTION,		// 字符没有右引号
	CHAR_NO_DATA,				// 空字符
	COMMENT_NO_END,			// 多行注释没有正常结束
	TOKEN_NO_EXIST			// 不存在的词法记号
};

// 语法错误
enum class SynError
{
	TYPE_LOST,					//类型
	TYPE_WRONG,					
	ID_LOST,						//标志符
	ID_WRONG,						
	NUM_LOST,						//数组长度
	NUM_WRONG,
	LITERAL_LOST,				//常量
	LITERAL_WRONG,
	COMMA_LOST,					//逗号
	COMMA_WRONG,
	SEMICON_LOST,				//分号
	SEMICON_WRONG,
	ASSIGN_LOST,				//=
	ASSIGN_WRONG,
	COLON_LOST,					//冒号
	COLON_WRONG,
	WHILE_LOST,					//while
	WHILE_WRONG,
	LPAREN_LOST,				//(
	LPAREN_WRONG,
	RPAREN_LOST,				//)
	RPAREN_WRONG,
	LBRACK_LOST,				//[
	LBRACK_WRONG,
	RBRACK_LOST,				//]
	RBRACK_WRONG,
	LBRACE_LOST,				//{
	LBRACE_WRONG,
	RBRACE_LOST,				//}
	RBRACE_WRONG
};


const std::string FATAL = RED + "<fatal>" + RESET;
const std::string ERROR = YELLOW + "<error>" + RESET;
const std::string WARN = BLUE + "<warn>" + RESET;

class Error {
private:
    static Scanner* scannerPtr;

public:
    static int errorNum; // 错误error和致命错误fatal都会累加到errorNum
    static int warnNum;

    Error() = delete;
    Error(Scanner& scanner);
    virtual ~Error() = default;

    static int getErrorNum();
    static int getWarnNum();

    // 程序异常结束
    static void terminateHandler();

    // 输出信息，并累加对应变量
    static void Fatal(std::string msg);
    static void Err(std::string msg);
    static void Warn(std::string msg);

    // 错误接口
	static void lexError(LexError errCode);// 打印词法错误
	static void synError(SynError errCode, std::shared_ptr<Token> t);// 打印语法错误
	static void semError(int errCode, std::string name="");// 打印语义错误
	static void semWarn(int errCode, std::string name="");// 打印语义警告
};
