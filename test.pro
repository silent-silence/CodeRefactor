TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    Decl/DeclBase.cpp \
    Decl/DeclContextHolder.cpp \
    Decl/DeclGroupRef.cpp \
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
    AST/ASTContext.h \ \
    Decl/Decl.h \
    Decl/DeclBase.h \
    Decl/DeclContextHolder.h \
    Decl/DeclGroupRef.h \
    Decl/DeclName.h \
    Errors/SymbolError.hpp
