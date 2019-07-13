#ifndef CODEREFACTOR_H
#define CODEREFACTOR_H

#include <memory>
#include "AST/ASTContext.h"
#include "Decl/DeclContextHolder.h"

class Driver;
class Printer;

class Refactor;
class OpenHelper;
class YaccAdapter;
class DeclContextHolder;
class NameRefactor;

class CodeRefactor
{
public:
    CodeRefactor();
    //CodeRefactor(std::string input);
    void run(std::string code);
    std::string show();
	//std::string show(std::string output);
    void setMCIf_SCMIf(bool value);
    void setSCMIf_MCIf(bool value);
    void setMIf_Switch(bool value);
    void setSwitch_MIf(bool value);
    void setFor_While(bool value);
    void setWhile_For(bool value);
    void setRefactorName(bool refactorName);
    void setDetectNullPointer(bool detect);

private:
    void config();

    bool MCIf_SCMIf;
    bool SCMIf_MCIf;
    bool MIf_Switch;
    bool Switch_MIf;
    bool For_While;
    bool While_For;
    bool doRefactorName;
    bool doDetectNullPointer;

    std::shared_ptr<Driver> driver_;
    std::shared_ptr<Printer> printer_;
    std::shared_ptr<Refactor> refactor_;
	std::shared_ptr<NameRefactor> nameRefactor;
	std::shared_ptr<YaccAdapter> adapter_;
	std::shared_ptr<OpenHelper> cin_open_helper_;
	std::shared_ptr<OpenHelper> cout_open_helper_;

    ASTContext context_;
    DeclContextHolder context_holder_;
};

#endif // CODEREFACTOR_H
