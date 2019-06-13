#include "CodeRefactor.h"
#include "Parser/Driver.h"
#include "AST/ASTContext.h"
#include "Parser/YaccAdapter.h"
#include "ASTOperation/Printer.h"
#include "ASTOperation/Corrector.h"
#include "OpenHelper/OpenHelper.h"
#include "OpenHelper/FileOpenHelper.h"
#include "OpenHelper/StdioOpenHelper.h"

#include "Decl/DeclContextHolder.h"

using std::weak_ptr;				using Printer::ContextPrinter;
using std::shared_ptr;
using std::make_shared;

CodeRefactor::CodeRefactor()
{
    cin_open_helper_ = make_shared<StdioOpenHelper>();
    nameRefactor = make_shared<NameRefactor>();
    config();
}

CodeRefactor::CodeRefactor(std::string input)
{
    cin_open_helper_ = make_shared<FileOpenHelper>(input);
	nameRefactor = make_shared<NameRefactor>();
    config();
}

void CodeRefactor::run()
{
    driver_->parse();
    nameRefactor->rename(context_holder_.getContextRoot());
}

void CodeRefactor::show()
{
    cout_open_helper_ = make_shared<StdioOpenHelper>();
    printer_ = make_shared<ContextPrinter>(*cout_open_helper_);
	printer_->printContext(context_holder_.getContextRoot());
}

void CodeRefactor::show(std::string output)
{
    cout_open_helper_ = make_shared<FileOpenHelper>("" ,output);
    printer_ = make_shared<ContextPrinter>(*cout_open_helper_);
	printer_->printContext(context_holder_.getContextRoot());
}

void CodeRefactor::config()
{
    adapter_ = make_shared<YaccAdapter>(context_, context_holder_, *cin_open_helper_);
    driver_ = make_shared<Driver>(*cin_open_helper_, *adapter_);
}

