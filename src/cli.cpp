#include "../include/cli.h"

ArgsBase::ArgsBase(const std::string& tag) : _tag(tag), _comment(""), necessary(false), _order(true) { }

bool ArgsBase::input(std::vector<std::string>& cmdArgs) { return false; }

const bool& ArgsBase::getNeed() const {
    return necessary;
}

const std::string& ArgsBase::getDoc() const {
    return _comment;
}

const std::string& ArgsBase::getTag() const {
    return _tag;
}

Option& Option::set(bool &flag) {
    _flag = flag;
    return *this;
}

Option& Option::doc(const std::string &comment) {
    _comment = comment;
    return *this;
}

Option& Option::isNeed(bool need) {
    necessary = need;
    return *this;
}

Option& Option::isOrder(bool order) {
    _order = order;
    return *this;
}

Option& Option::removeDuplicates() {
    std::sort(_args.begin(), _args.end());
    auto it = std::unique(_args.begin(), _args.end());
    _args.erase(it, _args.end());
    return *this;
}

bool Option::input(std::vector<std::string>& cmdArgs) {
    int cmdArgsNum = cmdArgs.size();

    if(_order) {
        auto it = std::find(_args.begin(), _args.end(), cmdArgs[0]);
        if (it != _args.end()) {
            cmdArgs.erase(cmdArgs.begin());
        }
    } else {
        cmdArgs.erase(std::remove_if(cmdArgs.begin(), cmdArgs.end(), 
            [&](const std::string& arg) {
                return std::find(_args.begin(), _args.end(), arg) != _args.end();
            }), cmdArgs.end());
    }

    __flag = (cmdArgs.size() != cmdArgsNum);
    return get();
}

const bool& Option::get() const {
    return _flag.has_value() ? (_flag.value().get() = __flag) : __flag;
}

const std::vector<std::string>& Option::getArgs() const {
    return _args;
}

Value::Value() : ArgsBase("Value"), _value(std::nullopt), __value("") { }

Value& Value::set(std::string &value) {
    _value = value;
    return *this;
}

Value& Value::doc(const std::string &comment) {
    _comment = comment;
    return *this;
}

Value& Value::isNeed(bool need) {
    necessary = need;
    return *this;
}

const std::string& Value::get() const {
    return _value.has_value() ? (_value.value().get() = __value) : __value;
}

bool Value::input(std::vector<std::string>& cmdArgs) {
    __value = cmdArgs[0];
    cmdArgs.erase(cmdArgs.begin());
    return !get().empty();
}

CLI& CLI::isOrder(bool order) {
    _order = order;

    for(const auto& opt : _options) {
        if(opt->getTag() == "Value") {
           _order = true; // 如果存在Value的获取，必须要按顺序
           break;
        }
    }

    return *this;
}

const bool& CLI::parse(const int& argc, const char** &argv) {
    for (int i = 1; i < argc; ++i) {
        _cmdIn.push_back(argv[i]);
    }
    fileName = argv[0];
    _success = true;
    
    for(const auto& _ : _options) {
        if(_->getTag() == "Value") {
            auto ptr = std::dynamic_pointer_cast<Value>(_);
            if(!_cmdIn.empty() && !(_cmdIn[0].substr(0, 1) == "-" || _cmdIn[0].substr(0, 2) == "--")) {
                bool result = ptr->input(_cmdIn);
            } else if(ptr->getNeed()) {
                std::cerr << std::format("必要参数 {} 无法识别\n", ptr->getDoc());
                _success = false;
            }
        } else if(_->getTag() == "Option") {
            auto ptr = std::dynamic_pointer_cast<Option>(_);
            bool result = false;
            if(!_cmdIn.empty()) {
                result = ptr->input(_cmdIn);
            }
            if(!result && ptr->getNeed()) {
                std::cerr << std::format("必要参数 {} 无法识别\n", ptr->getArgs()[0]);
                _success = false;
            }
        } else {
            std::cerr << std::format("参数设置异常!\n");
        }
    }

    return _success;
}

const int& CLI::getNeed() const {
    return _needNum;
}

void CLI::printComment() const {
    std::string comment = std::format("命令格式：{}", fileName);

    // 输出命令格式
    for(const auto& _ : _options) {
        if(_->getTag() == "Value") {
            auto ptr = std::dynamic_pointer_cast<Value>(_);
            if(ptr->getNeed()) {
                comment += std::format(" {}", ptr->getDoc());
            } else {
                comment += std::format(" [{}]", ptr->getDoc());
            }
        } else if(_->getTag() == "Option") {
            auto ptr = std::dynamic_pointer_cast<Option>(_);
            
            std::stringstream ss;
            std::copy(ptr->getArgs().begin(), ptr->getArgs().end() - 1,
                std::ostream_iterator<std::string>(ss, "|"));
            ss << ptr->getArgs().back();
            std::string tmp = ss.str();

            if(ptr->getNeed()) {
                comment += std::format(" {}", tmp);
            } else {
                comment += std::format(" [{}]", tmp);
            }
        } else {
            std::cerr << std::format("参数设置异常!\n");
        }
    }
    comment += "\n选项 : \n";

    // 输出参数信息
    for(const auto& _ : _options) {
        if(_->getTag() == "Value") {
            auto ptr = std::dynamic_pointer_cast<Value>(_);
            comment += std::format("\t{}\n", ptr->getDoc());
        } else if(_->getTag() == "Option") {
            auto ptr = std::dynamic_pointer_cast<Option>(_);
            
            std::stringstream ss;
            std::copy(ptr->getArgs().begin(), ptr->getArgs().end() - 1,
                std::ostream_iterator<std::string>(ss, ", "));
            ss << ptr->getArgs().back();
            std::string tmp = ss.str();

            comment += std::format("\t{}\t{}\n", tmp.append(_commentLength - tmp.length(), ' '), ptr->getDoc());
        } else {
            std::cerr << std::format("参数设置异常!\n");
        }
    }

    std::cout << comment << std::endl;
}