#include "CodeRefactor.h"
#include "Parser/Driver.h"
#include "AST/ASTContext.h"
#include "Parser/YaccAdapter.h"
#include "ASTOperation/Printer.h"

#include "OpenHelper/OpenHelper.h"
#include "OpenHelper/FileOpenHelper.h"
#include "OpenHelper/StdioOpenHelper.h"

#include "Decl/DeclContextHolder.h"

using std::weak_ptr;				using Printer::ASTPrinter;
using std::shared_ptr;
using std::make_shared;

CodeRefactor::CodeRefactor()
{
    cin_open_helper_ = make_shared<StdioOpenHelper>();
    config();
}

CodeRefactor::CodeRefactor(std::string input)
{
    cin_open_helper_ = make_shared<FileOpenHelper>(input);
    config();
}

void CodeRefactor::run()
{
    driver_->parse();
}

void CodeRefactor::show()
{
    cout_open_helper_ = make_shared<StdioOpenHelper>();
    printer_ = make_shared<ASTPrinter>(*cout_open_helper_);
	printer_->printAST(context_.getRoot().lock());
}

void CodeRefactor::show(std::string output)
{
    cout_open_helper_ = make_shared<FileOpenHelper>(output);
    printer_ = make_shared<ASTPrinter>(*cout_open_helper_);
	printer_->printAST(context_.getRoot().lock());
}

void CodeRefactor::config()
{
    adapter_ = make_shared<YaccAdapter>(context_, context_holder_, *cin_open_helper_);
    driver_ = make_shared<Driver>(*cin_open_helper_, *adapter_);
}

