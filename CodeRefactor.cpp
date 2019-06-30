#include "CodeRefactor.h"
#include "Parser/Driver.h"
#include "AST/ASTContext.h"
#include "Parser/YaccAdapter.h"
#include "ASTOperation/Printer.h"
#include "ASTOperation/Corrector.h"
#include "OpenHelper/OpenHelper.h"
#include "OpenHelper/FileOpenHelper.h"
#include "OpenHelper/StdioOpenHelper.h"
#include "OpenHelper/StringStreamOpenHelper.h"
#include "Decl/DeclContextHolder.h"
#include "Decl/Decl.h"
#include "ASTOperation/Refactor.h"

using std::weak_ptr;
using std::shared_ptr;              using std::dynamic_pointer_cast;
using std::make_shared;

CodeRefactor::CodeRefactor()
{
    cin_open_helper_ = make_shared<StringStreamOpenHelper>();
	cout_open_helper_ = make_shared<StringStreamOpenHelper>();
    nameRefactor = make_shared<NameRefactor>();
    refactor_ = make_shared<Refactor>();
	adapter_ = make_shared<YaccAdapter>(context_, context_holder_, *cin_open_helper_);
	driver_ = make_shared<Driver>(*cin_open_helper_, *adapter_);
	printer_ = make_shared<Printer>(*cout_open_helper_);
    config();
}

/*CodeRefactor::CodeRefactor(std::string input)
{
    cin_open_helper_ = make_shared<FileOpenHelper>(input);
	nameRefactor = make_shared<NameRefactor>();
    refactor_ = make_shared<Refactor>();
    config();
}*/

void CodeRefactor::run(std::string code)
{
	(*dynamic_pointer_cast<StringStreamOpenHelper>(cin_open_helper_)) << code;
    driver_->parse();
    nameRefactor->rename(context_holder_.getContextRoot());
    for(auto i=context_holder_.getContextRoot()->decl_begin(); i != context_holder_.getContextRoot()->decl_end(); ++i){
        if((*i)->getKind() == Decl::Kind::Function)
        {
            auto body = std::dynamic_pointer_cast<FunctionDecl>(*i)->getBody();
            if(MCIf_SCMIf){
                refactor_->refactor_MCIf(body);
            }
            if(SCMIf_MCIf){
                refactor_->refactor_SCMIf(body);
            }
            if(MIf_Switch){
                refactor_->refactor_MIf(body);
            }
            if(Switch_MIf){
                refactor_->refactor_Switch(body);
            }
            if(For_While){
                refactor_->refactor_For(body);
            }
            if(While_For){
                refactor_->refactor_While(body);
            }
        }
    }

}

std::string CodeRefactor::show()
{
    /*cout_open_helper_ = make_shared<StringStreamOpenHelper>();
    printer_ = make_shared<ContextPrinter>(*cout_open_helper_);*/
	printer_->print(context_holder_.getContextRoot());
	// output
	std::string outputString;
	(*dynamic_pointer_cast<StringStreamOpenHelper>(cout_open_helper_)) >> outputString;
	return outputString;
}

/*std::string CodeRefactor::show(std::string output)
{
    //cout_open_helper_ = make_shared<FileOpenHelper>("" ,output);
    cout_open_helper_ = make_shared<StringStreamOpenHelper>();
    printer_ = make_shared<ContextPrinter>(*cout_open_helper_);
	printer_->printContext(context_holder_.getContextRoot());
	std::string outputString;
	(*dynamic_pointer_cast<StringStreamOpenHelper>(cout_open_helper_)) >> outputString;
	return outputString;
}*/

void CodeRefactor::config()
{
    MCIf_SCMIf = false;
    SCMIf_MCIf = false;
    MIf_Switch = false;
    Switch_MIf = false;
    For_While = false;
    While_For = false;
    /*adapter_ = make_shared<YaccAdapter>(context_, context_holder_, *cin_open_helper_);
    driver_ = make_shared<Driver>(*cin_open_helper_, *adapter_);*/
}

void CodeRefactor::setWhile_For(bool value)
{
    While_For = value;
}

void CodeRefactor::setFor_While(bool value)
{
    For_While = value;
}

void CodeRefactor::setSwitch_MIf(bool value)
{
    Switch_MIf = value;
}

void CodeRefactor::setMIf_Switch(bool value)
{
    MIf_Switch = value;
}

void CodeRefactor::setSCMIf_MCIf(bool value)
{
    SCMIf_MCIf = value;
}

void CodeRefactor::setMCIf_SCMIf(bool value)
{
    MCIf_SCMIf = value;
}

