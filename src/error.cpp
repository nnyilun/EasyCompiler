#include "../include/error.h"

// 自动构造，全局设置终止处理函数
namespace __error_global_terminate_hanlder__ {
    struct TerminateSetter {
        TerminateSetter() {
            std::set_terminate(Error::terminateHandler);
        }
    };

    static TerminateSetter setter;
}

// 初始化错误数和警告数
int Error::errorNum=0;
int Error::warnNum=0;
Scanner* Error::scannerPtr = nullptr;

Error::Error(Scanner& scanner) {
    scannerPtr = &scanner;
}

int Error::getErrorNum() {
    return errorNum;
}

int Error::getWarnNum() {
    return warnNum;
}

void Error::terminateHandler() {
    std::cerr << std::format("编译异常终止！\n");
    std::abort();
}

void Error::Fatal(std::string msg) {
    std::cerr << std::format("{} {}\n", FATAL, msg);
    ++Error::errorNum;
}

void Error::Err(std::string msg) {
    std::cerr << std::format("{} {}\n", ERROR, msg);
    ++Error::errorNum;
}

void Error::Warn(std::string msg) {
    std::cerr << std::format("{} {}\n", WARN, msg);
    ++Error::warnNum;
}

// 输出词法错误
void Error::lexError(LexError errCode) {

    static constexpr std::array<std::string_view, 7> lexErrorTable {
        "字符串丢失右引号",
		"二进制数没有实体数据",
		"十六进制数没有实体数据",
		"字符丢失右单引号",
		"不支持空字符",
		"多行注释没有正常结束",
		"词法记号不存在"
    };
	++errorNum;
    std::cerr << std::format("{} <{}行,{}列> 词法错误 : {}. 字符 : \'{}\'.\n", 
                    scannerPtr->getFileName(), scannerPtr->getRow(), scannerPtr->getColumn(), 
                    lexErrorTable[static_cast<int>(errCode)], scannerPtr->preChrRaw());
}

// 输出语法错误
void Error::synError(SynError errCode, std::shared_ptr<Token> t) {
    static constexpr std::array<std::string_view, 15> synErrorTable {
        "类型",
		"标识符",
		"数组长度",
		"常量",
		"逗号",
		"分号",
		"=",
		"冒号",
		"while",
		"(",
		")",
		"[",
		"]",
		"{",
		"}"
    };
	errorNum++;
	if(static_cast<int>(errCode) % 2 == 0) { // 符号丢失
        std::cerr << std::format("{} <{}行,{}列> 语法错误 : 在 {} 之前丢失 {} .\n", 
                        scannerPtr->getFileName(), scannerPtr->getRow(), scannerPtr->getColumn(), 
                        t->toString(), synErrorTable[static_cast<int>(errCode) / 2]);
    } else { // 符号错误
        std::cerr << std::format("{} <{}行,{}列> 语法错误 : 在 {} 处没有正确匹配 {} .\n", 
                        scannerPtr->getFileName(), scannerPtr->getRow(), scannerPtr->getColumn(), 
                        t->toString(), synErrorTable[static_cast<int>(errCode) / 2]);
    }
}