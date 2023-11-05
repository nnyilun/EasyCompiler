#include "../include/scanner.h"
#include "../include/error.h"

Scanner::Scanner(const char* fileName) : _fileName(fileName), row(0), column(0), lastChr(0) {
    file = std::ifstream(_fileName, std::ios::in);

    if(!file.is_open()) {
        Error::Fatal(std::format("[扫描器] 文件 \"{}\" 打开失败! 请检查文件路径和文件名是否正确.", fileName));
        std::terminate();
    }
}

Scanner::Scanner(const std::string& fileName) : _fileName(fileName), row(0), column(0), lastChr(0) {
    file = std::ifstream(_fileName, std::ios::in);

    if(!file.is_open()) {
        Error::Fatal(std::format("[扫描器] 文件 \"{}\" 打开失败! 请检查文件路径和文件名是否正确.", fileName));
        std::terminate();
    }
}

Scanner::~Scanner() {
    // 析构时关闭文件
    if(file.is_open()) {
        file.close();
    }
}

bool Scanner::updateBuff() {
    // 判断缓冲区是否为空，如果为空且文件无法读入则返回false
    if(chrBuff.empty()) {
        if(!file.good()) return false;
        file.read(buff, BUFF_SIZE);
        for(int i = 0, cnt = file.gcount(); i < cnt; ++i) {
            chrBuff.push(buff[i]);
        }
    }
    return true;
}

void Scanner::printRawChr() {
    if(Args::showChar) {
        std::cout << std::format("<{}行, {}列>原始字符: {}\n", row, column, preChrRaw());
    }
}

char Scanner::getChr() {
    // 更新队列，如果文件读完则直接返回
    if(!updateBuff()) return EOF;

    // 判断是否需要换行
    if(lastChr == '\n') {
        ++row;
        column = 0;
    }

    lastChr = chrBuff.front();
    chrBuff.pop();
    ++column;

    printRawChr();
    return lastChr;
}

const char& Scanner::preChr() const {
    return lastChr;
}

const std::string Scanner::preChrRaw() const {
    switch (lastChr) {
        case '\t': return "\\t";
        case '\n': return "\\n";
        case '\\': return "\\\\";
        case '\'': return "\\\'";
        case '\"': return "\\\"";
        default: return std::string(1, lastChr);
    }
}

const int& Scanner::getRow() const{
    return row;
}

const int& Scanner::getColumn() const {
    return column;
}

const std::string& Scanner::getFileName() const {
    return _fileName;
}