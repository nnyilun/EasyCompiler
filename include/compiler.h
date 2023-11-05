#pragma once

#include "common.h"
#include "compiler.h"
#include "scanner.h"
#include "lexer.h"
#include "error.h"

// 编译器类
class Compiler {
private:
	const std::string _fileName;
	Scanner scanner;
	Lexer lexer;
	Error error;
public:
	Compiler() = delete;
    Compiler(const std::string& fileName);
	virtual ~Compiler() = default;

	void compiler();
};