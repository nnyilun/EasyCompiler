#include "../include/token.h"

// 标识符名称
const char* tokenName[] = {
	"error",// 错误，异常，结束标记等
	"文件结尾",// 文件结束
	"标识符",// 标识符
	"数字", "字符", "字符串", // 常量
	"int", "char", "void",// 数据类型

#ifdef COMPLEX_DATA
	"short", "long", "long long int", "float", "double", "long double", "struct", // 复杂数据类型
#endif

	"extern",// extern

	",", ":", ";", // 逗号、冒号、分号
	"=",
	"if", "else",
	"switch", "case", "default",
	"while", "do", "for",
	"break", "continue", "return",
	
	"取地址符", "间接访问操作符", // 取地址符和间接访问操作符

	"!", "&", "|", // 位运算
	
	"+", "-", "*", "/", "%",//算术运算符
	"++", "--",
	
	">", ">=", "<", "<=", "==", "!=",//比较运算符
	
	"!", "&&", "||", //逻辑运算
	
	"(", ")", //()
	"[", "]", //[]
	"{", "}", //{}
	
};

// 初始化关键字列表
Keywords::Keywords() {
    keywords["int"] = Tag::KW_INT;
	keywords["char"] = Tag::KW_CHAR;
	keywords["void"] = Tag::KW_VOID;

#ifdef COMPLEX_DATA
	keywords["short"] = Tag::KW_SHORT;
	keywords["long"] = Tag::KW_LONG;
	keywords["long long"] = Tag::KW_LLINT;
	keywords["long long int"] = Tag::KW_LLINT;
	keywords["float"] = Tag::KW_FLOAT;
	keywords["double"] = Tag::KW_DOUBLE;
	keywords["long double"] = Tag::KW_LDOUBLE;
	keywords["struct"] = Tag::KW_STRUCT;
#endif

	keywords["extern"] = Tag::KW_EXTERN;
	keywords["if"] = Tag::KW_IF;
	keywords["else"] = Tag::KW_ELSE;
	keywords["switch"] = Tag::KW_SWITCH;
	keywords["case"] = Tag::KW_CASE;
	keywords["default"] = Tag::KW_DEFAULT;
	keywords["while"] = Tag::KW_WHILE;
	keywords["do"] = Tag::KW_DO;
	keywords["for"] = Tag::KW_FOR;
	keywords["break"] = Tag::KW_BREAK;
	keywords["continue"] = Tag::KW_CONTINUE;
	keywords["return"] = Tag::KW_RETURN;
}

// 判断是关键字还是标识符
Tag Keywords::getTag(std::string name) {
    return keywords.find(name) != keywords.end() ? keywords[name] : Tag::ID;
}


// 词法记号
Token::Token(Tag t) : tag(t) {}

std::string Token::toString() {
	return tokenName[static_cast<int>(tag)];
}

// 标识符
Id::Id(std::string n) : Token(Tag::ID), name(n) {}

std::string Id::toString() {
	return "[" + Token::toString()+ "]:" + name;
}

// 数字
Num::Num(int v) : Token(Tag::NUM), val(v) {}

std::string Num::toString() {
	return "[" + Token::toString() + "]:" + std::to_string(val);
}

// 字符
Char::Char(char c) : Token(Tag::CH), ch(c) {}

std::string Char::toString() {
	return "[" + Token::toString() + "]:" + std::string(1, ch);
}

// 字符串
Str::Str(std::string s) : Token(Tag::STR), str(s) {}

std::string Str::toString() {
	const auto raw = [](const std::string& s){ 
		std::string tmp(s);
		for(int i = 0; i < tmp.length(); ++i) {
			switch (tmp[i]) {
				case '\t': tmp[i] = '\\'; tmp.insert(++i, 1, 't'); break;
				case '\n': tmp[i] = '\\'; tmp.insert(++i, 1, 'n'); break;
				case '\\': tmp[i] = '\\'; tmp.insert(++i, 1, '\\'); break;
				case '\'': tmp[i] = '\\'; tmp.insert(++i, 1, '\''); break;
				case '\"': tmp[i] = '\\'; tmp.insert(++i, 1, '\"'); break;
				default: break;
			}
		}
		return tmp;
	};
	return "[" +Token::toString() + "]:" + raw(str);
}