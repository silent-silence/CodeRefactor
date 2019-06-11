#ifndef CODEREFACTOR_H
#define CODEREFACTOR_H

#include <memory>
#include "AST/ASTContext.h"
#include "Decl/DeclContextHolder.h"

class Driver;
namespace Printer {
	class ASTPrinter;
}
class Refactor;
class OpenHelper;
class YaccAdapter;
class DeclContextHolder;

class CodeRefactor
{
public:
    CodeRefactor();
    CodeRefactor(std::string input);
    void run();
    void show();
    void show(std::string output);
private:
    void config();

    bool MCIf_SCMIf;
    bool SCMIf_MCIf;
    bool MIf_Switch;
    bool Switch_MIf;
    bool For_While;
    bool While_For;

    std::shared_ptr<Driver> driver_;
    std::shared_ptr<Printer::ASTPrinter> printer_;
    std::shared_ptr<Refactor> refactor_;
    std::shared_ptr<YaccAdapter> adapter_;
    std::shared_ptr<OpenHelper> cin_open_helper_;
    std::shared_ptr<OpenHelper> cout_open_helper_;

    ASTContext context_;
    DeclContextHolder context_holder_;
};

#endif // CODEREFACTOR_H
