#include "../include/compiler.h"

// 初始化参数
std::string Args::fileName = "";
bool Args::showChar = false;
bool Args::showToken = false;
bool Args::showSym = false;
bool Args::showIC = false;
bool Args::showOC = false;
bool Args::showBlock = false;
bool Args::showHelp = false;
bool Args::opt = false;

Compiler::Compiler(const std::string& fileName) : _fileName(fileName), 
    scanner(_fileName), lexer(scanner), error(scanner) { }

void Compiler::compiler() {
    for(auto t = lexer.tokenize(); t->tag != Tag::END; t = lexer.tokenize());
}