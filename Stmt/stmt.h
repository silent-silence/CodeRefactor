#ifndef STMT_H
#define STMT_H

#include <array>
#include <vector>
#include <memory>

//Stmt - This represents one statement.
class Stmt
{
public:
    enum StmtClass {
        NoStmtClass = 0,
#define STMT(CLASS, PARENT) CLASS##Class,
#define FIRST_STMT(CLASS) firstStmtConstant = CLASS##Class,
#define LAST_STMT(CLASS) lastStmtConstant = CLASS##Class,
#define FIRST_EXPR(CLASS) firstExprConstant = CLASS##Class,
#define LAST_EXPR(CLASS) lastExprConstant = CLASS##Class
#include "StmtNodes.def"
    };
    struct EmptyShell {};
    Stmt(StmtClass SC);
    explicit Stmt(StmtClass SC, EmptyShell);
protected:
private:
    const unsigned sClass : 8;//The statement class.
    unsigned RefCount : 24;//The reference count for this statement.
};

#endif // STMT_H
