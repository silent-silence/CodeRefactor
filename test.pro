TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    main.cpp \
    Basic/SourceLocation.cpp \
    AST/Expr.cpp \
    AST/Type.cpp \
    AST/Stmt.cpp \
    AST/ASTContext.cpp

HEADERS += \
    Basic/SourceLocation.h \
    AST/Expr.h \
    AST/Type.h \
    AST/TypeNodes.def \
    AST/Stmt.h \
    AST/StmtNodes.def \
    AST/AST.hpp \
    AST/ASTContext.h
