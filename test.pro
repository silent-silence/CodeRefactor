TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    ASTOperation/Corrector.cpp \
    ASTOperation/Refactor.cpp \
    Basic/IdentifierTable.cpp \
    Decl/DeclBase.cpp \
    Decl/DeclContextHolder.cpp \
    Decl/DeclGroup.cpp \
    main.cpp \
    Basic/SourceLocation.cpp \
    AST/Expr.cpp \
    AST/Type.cpp \
    AST/Stmt.cpp \
    AST/ASTContext.cpp

HEADERS += \
    ASTOperation/Corrector.h \
    ASTOperation/Refactor.h \
    Basic/IdentifierTable.h \
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
    Decl/DeclGroup.h \
    Decl/DeclName.h \
    Errors/SymbolError.hpp
