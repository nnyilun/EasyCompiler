#pragma once

#include "common.h"

#define NAME(variable) (#variable)

/*
    命令行参数解析
    暂时只能解析两种参数：
    1. 固定参数位置的值
    2. 类似-x --xx的、不带值的参数
*/

// 命令行参数基类
class ArgsBase {
protected:
    std::string _comment; // 命令注释
    const std::string _tag; // 标签

    bool _order;  // 是否有序
    bool necessary; // 该参数是否为必须

public:
    ArgsBase(const std::string& tag);
    // ArgsBase(const ArgsBase&) = default;
    virtual ~ArgsBase() = default;

    virtual bool input(std::vector<std::string>& cmdArgs);

    const bool& getNeed() const;
    const std::string& getDoc() const;
    const std::string& getTag() const;
};

class Value : public ArgsBase {
private:
    std::optional<std::reference_wrapper<std::string>> _value;
    std::string __value;

public:
    // 接收命令行参数并储存到内置变量
    Value();

    virtual ~Value() = default;

    Value& set(std::string& value);
    Value& doc(const std::string& comment);
    Value& isNeed(bool need = false);

    // 传入参数并记录，不带参数获取值必须按顺序
    bool input(std::vector<std::string>& cmdArgs) override;
    // 默认为空
    const std::string& get() const;    
};

// 命令行选项
class Option : public ArgsBase {
private:
    std::vector<std::string> _args; // 相同功能的命令行参数

    // 如果需要修改某变量的值，则将该变量储存引用
    std::optional<std::reference_wrapper<bool>> _flag;
    bool __flag; // 判断结果会储存在这里

public:
    Option() = delete;

    // 接收命令行参数
    template<typename... Args>
    Option(Args... args) requires (std::is_same_v<const char*, Args> && ...);

    virtual ~Option() = default;

    Option& set(bool& flag);
    Option& doc(const std::string& comment);
    Option& isNeed(bool need = false);
    Option& isOrder(bool order = true);
    Option& removeDuplicates();

    // 传入参数，判断是否符合要求，如果符合则从字符串列表中删除该项
    bool input(std::vector<std::string>& cmdArgs) override;
    // 如果没有进行判断，那么将默认为false
    const bool& get() const;
    const std::vector<std::string>& getArgs() const;
};

// 必须将该函数定义放到和声明同文件，否则会找不到定义
template<typename... Args>
Option::Option(Args... args) requires (std::is_same_v<const char*, Args> && ...) : 
    ArgsBase("Option"), _args({std::string(args)...}), _flag(std::nullopt), __flag(false) { }


// 解析器
class CLI {
private:
    std::vector<std::shared_ptr<ArgsBase>> _options;
    std::vector<std::string> _cmdIn;
    std::string fileName;
    int _needNum;
    bool _order;
    bool _success;
    int _commentLength;

public:
    CLI() = delete;

    template<typename ...Args>
    CLI(Args ...options) requires (std::is_base_of_v<ArgsBase, Args> && ...);
    
    virtual ~CLI() = default;

    // 是否按照参数构造顺序判断
    // 如果存在Value则必须按照顺序
    CLI& isOrder(bool order = true);

    // 解析命令行参数
    const bool& parse(const int& argc, const char** &argv);

    const int& getNeed() const; // 获取必须判断的参数数量
    void printComment() const; // 输出注释
};

template<typename ...Args>
CLI::CLI(Args ...options) requires (std::is_base_of_v<ArgsBase, Args> && ...) :
    _options({ std::make_shared<Args>(options)... }), _needNum(0), _order(true), _success(false), _commentLength(16) { 

    // 计算必须处理的参数数量
    for(const auto& opt : _options) {
        if(opt->getNeed()) ++_needNum;

        if(opt->getTag() == "Value") { // Value
           _order = true; // 如果存在Value的获取，必须要按顺序
        } 
    }
}