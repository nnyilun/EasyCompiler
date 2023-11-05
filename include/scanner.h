#pragma once

/*
    词法分析器的扫描器
*/

#include "common.h"

class Scanner {
private:
    static const int BUFF_SIZE = 256;

    const std::string _fileName;
    std::ifstream file;

    char buff[BUFF_SIZE];
    std::queue<char> chrBuff;
    int row;
    int column;
    char lastChr;

    // 判断缓冲区是否为空，如果为空则再次读入
    bool updateBuff();

    // 在显示字符模式下，输出该字符的原始表示
    void printRawChr();

    // 判断文件是否读完
	
public:
    Scanner() = delete;
    Scanner(const char* fileName);
    Scanner(const std::string& fileName);
    virtual ~Scanner();

    // 从缓冲区读入一个字符
    char getChr();
    // 返回当前字符
    const char& preChr() const;
    // 输出当前字符转义前的符号
    const std::string preChrRaw() const;
	
    const int& getRow() const;
    const int& getColumn() const;
    const std::string& getFileName() const;
};