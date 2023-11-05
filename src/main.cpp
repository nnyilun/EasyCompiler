#include "../include/cli.h"
#include "../include/compiler.h"

/*
    命令格式：Easycompiler [源文件] [选项]
    选项：
        -o --optimize			# 执行优化
        -c --char					# 显示文件字符
        -t --token				# 显示词法记号
        -s --symbol				# 显示符号表信息
        -ic --intercode		# 显示中间代码
        -oc --optcode			# 显示优化后的中间代码
        -b --block				# 显示基本块和流图关系
        -h --help					# 显示帮助信息
*/

int main(const int argc, const char* argv[]) {
    // 需要按照顺序输入参数
    CLI cli = CLI(
        Value().set(Args::fileName).doc("源文件").isNeed(true),
        Option("-h", "--help").set(Args::showHelp).doc("显示帮助信息"),
        Option("-o", "--optimize").set(Args::opt).doc("执行优化"),
        Option("-c", "--char").set(Args::showChar).doc("显示文件字符"),
        Option("-t", "--token").set(Args::showToken).doc("显示词法记号"),
        Option("-s", "--symbol").set(Args::showSym).doc("显示符号表信息"),
        Option("-ic", "--intercode").set(Args::showIC).doc("显示中间代码"),
        Option("-oc", "--optcode").set(Args::showOC).doc("显示优化后的中间代码"),
        Option("-b", "--block").set(Args::showBlock).doc("显示基本块和流图关系")
    );

    auto _ = cli.parse(argc, argv);
    if(Args::showHelp || !_) cli.printComment();

    // 初始化编译器并启动编译
    Compiler compiler(Args::fileName);
    compiler.compiler();
    return 0;
}