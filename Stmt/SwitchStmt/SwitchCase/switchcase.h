#ifndef SWITCHCASE_H
#define SWITCHCASE_H

#include "../../stmt.h"

class SwitchCase : public Stmt
{
public:
    SwitchCase(StmtClass SC) : Stmt(SC), NextSwitchCase(nullptr) {}
private:
    std::shared_ptr<SwitchCase> NextSwitchCase;
};

#endif // SWITCHCASE_H
