# 编译过程

## 预编译

处理宏定义和文件包含信息

头文件的引用和宏文本的替换将会在这一步进行。如果宏需要嵌套展开，预编译器将会展开这些宏，这个过程和编译器类似。

```
gcc -E helloworld.c -o helloworld.i
```

## 编译

```
gcc -S helloworld.i -o helloworld.s
```

##  汇编

将编译的汇编文件生成二进制目标文件

```
gcc -c helloworld.s -o helloworld.o
```

linux中使用objdump可以反汇编分析内容

```
objdump -sd helloworld.o
```

汇编器在处理当前文件的过程中无法获悉符号的虚拟地址，所以会设置默认值。在连接过程中将会确定真正的地址。

## 链接

```
gcc helloworld.o -o helloworld -static
```
使用static为静态链接，gcc默认使用动态链接

可以使用objdump指令查看静态链接后文件的信息，在汇编过程中无法确定的符号地址将会在这里被确定。