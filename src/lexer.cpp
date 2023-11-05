#include "../include/lexer.h"
#include <cctype>
#include <error.h>
#include <memory>

Keywords Lexer::keywords;

Lexer::Lexer(Scanner& scanner) : _scanner(scanner), chr(' '), 
    symbolList({ // 初始化符号表
        {'+', [&]() { scan(); return (chr == '+') ? Tag::INC : Tag::ADD; }},
        {'-', [&]() { scan(); return (chr == '-') ? Tag::DEC : Tag::SUB; }},
        {'*', [&]() { scan(); return Tag::MUL; }},
        {'/', [&]() 
            { 
                scan();
                if(chr == '/') { // 单行注释
                    while(!(chr == '\n' || chr == EOF)) scan();
                    return Tag::ERR;
                } else if(chr == '*') { // 多行注释
                    for(scan(); chr != EOF; scan()) {
                        if(chr == '*') {
                            for(scan(); chr == '*'; scan()) ; // 跳过连续出现的*
                            if(chr == '/') {
                                return Tag::ERR;
                            }
                        }
                    }
                    if(chr == EOF) {
                        Error::lexError(LexError::COMMENT_NO_END);
                        return Tag::ERR;
                    }
                } 
                return Tag::DIV; // 除号
             }},
        {'%', [&]() { scan(); return Tag::MOD; }},
        {'>', [&]() { scan(); return (chr == '=') ? Tag::GE : Tag::GT; }},
        {'<', [&]() { scan(); return (chr == '=') ? Tag::LE : Tag::LT; }},
        {'=', [&]() { scan(); return (chr == '=') ? Tag::EQU : Tag::ASSIGN; }},
        {'&', [&]() { scan(); return (chr == '&') ? Tag::LAND : Tag::BAND; }},
        {'|', [&]() { scan(); return (chr == '|') ? Tag::LOR : Tag::BOR; }},
        {'~', [&]() { scan(); return Tag::BNOT; }},
        {'!', [&]() { scan(); return (chr == '=') ? Tag::NEQU : Tag::LNOT; }},
        {',', [&]() { scan(); return Tag::COMMA; }},
        {':', [&]() { scan(); return Tag::COLON; }},
        {';', [&]() { scan(); return Tag::SEMICON; }},
        {'(', [&]() { scan(); return Tag::LPAREN; }},
        {')', [&]() { scan(); return Tag::RPAREN; }},
        {'[', [&]() { scan(); return Tag::LBRACK; }},
        {']', [&]() { scan(); return Tag::RBRACK; }},
        {'{', [&]() { scan(); return Tag::LBRACE; }},
        {'}', [&]() { scan(); return Tag::RBRACE; }},
        {EOF, [&]() { scan(); return Tag::END; }},
    }) {
    tokenPtr.reset();
}

void Lexer::scan() {
    chr = _scanner.getChr();
}

int Lexer::getValue() {
    // 如果要能处理复杂数据类型，这个函数需要调整
    int val = 0;

    // 使用状态机判断，状态机目前只能处理正整数
    if(chr != '0') { // 十进制
        do {
            val = val * 10 + (chr - '0');
            scan();
        } while(std::isdigit(chr));
    } else {
        scan();
        if(chr == 'x' || chr == 'X') { // 十六进制
            scan();
            if(std::isalnum(chr)) { 
                do {
                    if(std::isdigit(chr)) val = val * 16 + (chr - '0');
                    else if(std::isupper(chr)) val = val * 16 + (chr - 'A');
                    else if(std::islower(chr)) val = val * 16 + (chr - 'a');
                } while(std::isalnum(chr));
            } else { // 0x后没有有效十六进制数字
                Error::lexError(LexError::NUM_HEX_TYPE);
                tokenPtr = std::make_shared<Token>(Tag::ERR);
            }
        } else if(chr == 'b' || chr == 'B') { // 二进制
            scan();
            if(chr == '0' || chr == '1') {
                do {
                    val = val * 2 + (chr - '0');
                    scan();
                } while(chr == '0' || chr == '1');
            } else { // 0b后没有有效的二进制数字
                Error::lexError(LexError::NUM_BIN_TYPE);
                tokenPtr = std::make_shared<Token>(Tag::ERR);
            }
        } else if(chr >= '0' && chr <= '7') { // 八进制
            do {
                val = val * 8 + (chr - '0');
                scan();
            } while(chr >= '0' && chr <= '7');
        }
    }

    tokenPtr = std::make_shared<Num>(val);
    return val; // 返回的val可能为不合法的值
}

std::string Lexer::getTag() {
    std::string name = "";

    do {
        name.push_back(chr);
        scan();
    } while(std::isalnum(chr) || chr == '_');

    if(Tag tag = keywords.getTag(name); tag == Tag::ID) {
        tokenPtr = std::make_shared<Id>(name);
    } else {
        tokenPtr = std::make_shared<Token>(tag);
    }

    return name;
}

char Lexer::getChar() {
    // 字符常量会存在转义字符
    char c;
    scan();
    if(chr == '\\') { // 转义字符
        scan();
        if(chr == 'n') c = '\n';
        else if(chr == '\\') c = '\\';
        else if(chr == 't') c = '\t';
        else if(chr == '0') c = '\0';
        else if(chr == '\'') c = '\'';
        else if(chr == '\"') c = '\"';
        else if(chr == EOF || chr == '\n') { // 转义不完整，文件结束或者换行
            Error::lexError(LexError::CHAR_NO_R_QUTION);
            tokenPtr = std::make_shared<Token>(Tag::ERR);
        }
    } else if(chr == EOF || chr == '\n') { // 仅有左单引号，文件结束或换行
        Error::lexError(LexError::CHAR_NO_R_QUTION);
        tokenPtr = std::make_shared<Token>(Tag::ERR);
    } else if(chr == '\'') { // 空字符，没有数据
        Error::lexError(LexError::CHAR_NO_DATA);
        tokenPtr = std::make_shared<Token>(Tag::ERR);
    } else {
        c = chr;
    }
    
    scan();
    if(chr != '\'') { // 没有右引号
        Error::lexError(LexError::CHAR_NO_R_QUTION);
        tokenPtr = std::make_shared<Token>(Tag::ERR);
    } else {
        scan();
        tokenPtr = std::make_shared<Char>(c);
    }

    return c;
}

std::string Lexer::getStr() {
    std::string str = "";
    for(scan(); chr != '\"'; scan()) {
        if(chr == '\\') { // 转义字符
            scan();
            if(chr == 'n') str.push_back('\n');
            else if(chr == '\\') str.push_back('\\');
            else if(chr == 't') str.push_back('\t');
            else if(chr == '\"') str.push_back('\"');
            else if(chr == '0') str.push_back('\0');
            else if(chr == '\n') ; // 字符串换行
            else if(chr == EOF) { // 转义不完整，文件结束或者换行
                Error::lexError(LexError::STR_NO_R_QUTION);
                tokenPtr = std::make_shared<Token>(Tag::ERR);
                break;
            } else {
                str.push_back(chr);
            }
        } else if(chr == EOF || chr == '\n') { // 文件结束
            Error::lexError(LexError::STR_NO_R_QUTION);
            tokenPtr = std::make_shared<Token>(Tag::ERR);
            break;
        } else {
            str.push_back(chr);
        }
    }
    
    if(tokenPtr->tag != Tag::ERR) scan();
    tokenPtr = std::make_shared<Str>(str);
    return str;
}

Tag Lexer::getSymbol() {
    Tag tag = Tag::ERR;
    for(const auto& _ : symbolList) {
        if(chr == _.first) tag = _.second();
    }
    if(tag == Tag::ERR) Error::lexError(LexError::TOKEN_NO_EXIST);
    tokenPtr = std::make_shared<Token>(tag);
    return tag;
}

std::shared_ptr<Token> Lexer::tokenize() {
    while(chr != EOF) {
        // 忽略无效词法记号
        while(chr == ' ' || chr == '\n' || chr == '\t') scan();

        if(std::isalpha(chr) || chr == '_') getTag(); // 标识符和关键字
        else if(chr == '\"') getStr(); // 字符串
        else if(std::isdigit(chr)) getValue(); // 数字
        else if(chr == '\'') getChar(); // 字符
        else getSymbol(); // 界符

        if(tokenPtr && tokenPtr->tag != Tag::ERR) {
            printToken();
            return tokenPtr; // token有效
        }
    }
    tokenPtr = std::make_shared<Token>(Tag::END);
    printToken();
    return tokenPtr;
}

void Lexer::printToken() {
    if(Args::showToken) {
        std::cout << std::format("[词法记号]{}\n", tokenPtr->toString());
    }
}