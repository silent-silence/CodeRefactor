# CodeRefactor
---
1. ## 简介
    本项目以C语言代码结构及流程重构为设计方向，使用Lex和Yacc进行词法和语法分析，使用gtest库进行程序测试，程序核心以C++为主。
2. ## 目录介绍
    + ### AST：
        该目录包含了语法结构树中结点的实体类型以及结点的关联方法。
    + ### ASTOperation
        该目录包含语法结构树重构方法（Refactor）、符号表重构方法（Corrector）和格式控制输出类（Printer）。
    + ### Basic
        该目录包含词法单元位置信息类（SourceLocation）和标识符变量名（IdentifierTable）。
    + ### Decl
        该目录为符号表构成类的集合，包含符号表名字、上下文、相关连接。
    + ### Errors
        该目录为程序错误异常类的集合，包含有类型错误、语法错误、符号错误。
    + ### OpenHelper
        该目录为程序输入输出流的协助类，程序可以从标准输入输出、字符串、文件来进行代码重构。
    + ### Parser
        该目录为词法和语法分析驱动，由Lex、Yacc和边界交互类构成。
    + ### test
        该目录为针对于单次迭代开发的测试代码，包含所有单个测试案例。
3. ## 快速开始
    + ### 构建环境
        1. #### 开发环境：
            Linux（ArchLinux）
        2. #### 相关依赖  
            ```
                sudo pacman -Sy gcc cmake flex bison
            ```
            + ##### Version:
                + gcc : 8.3.0
                + flex：2.6.4
                + bison：3.3.2
                + cmake：3.13
            + GitHub  
                ```
                    git clone https://github.com/silent-silence/CodeRefactor.git
                ```
    + ### 编译方法
        ```
            cd CodeRefactor
            mkdir build-dir
            cd build-dir
            cmake ..
            make
        ```
4. ## 测试  
    ```
        mv testfile ./
        cat testfile
        ./CodeRefactor testfile -o outputfile
        cat outputfile
    ```
5. ## 开发者  
    赵亚伟 & 高健 & 余诗雨
6. ## 讨论  
    + 项目后期可添加C++支持
    + 项目中有关于宏定义问题
    + 项目引入标准库相关数据
