TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    main.cpp \
    Basic/SourceLocation.cpp \
    AST/Expr.cpp \
    AST/Type.cpp \
    AST/Stmt.cpp

HEADERS += \
    Basic/SourceLocation.h \
    AST/Expr.h \
    AST/Type.h \
    AST/TypeNodes.def \
    AST/Stmt.h \
    AST/StmtNodes.def \
    AST/AST.hpp
