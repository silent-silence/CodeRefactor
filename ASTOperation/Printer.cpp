//
// Created by 17271 on 2019/5/26.
//

#include "Printer.h"
#include "AST/Stmt.h"
#include "AST/Expr.h"
#include "OpenHelper/OpenHelper.h"
#include "Decl/DeclGroup.h"
#include "Decl/DeclBase.h"
#include "Basic/IdentifierTable.h"
#include "Errors/NullPointerError.h"
#include <regex>

using std::string;					using std::dynamic_pointer_cast;
using std::to_string;				using std::make_shared;
using std::vector;					using std::regex;

/// @Printer
Printer::Printer(OpenHelper &openHelper)
	: m_openHelper{openHelper}, typePrinter{*this}, astPrinter{*this}, contextPrinter{*this}, indentNum{0}, m_detectNullPointer{false}
{}

void Printer::print(std::shared_ptr<DeclContext> context)
{
	m_openHelper.getOutputStream() << contextPrinter.printContext(context);
}

void Printer::print(std::shared_ptr<Stmt> root)
{
	m_openHelper.getOutputStream() << astPrinter.printAST(root);
}

void Printer::detectNullPointer(bool doDetect)
{
	m_detectNullPointer = doDetect;
}

std::string Printer::oneIndentLessIndent()
{
	return string(indentNum == 0 ? 0 : ((indentNum-1) * 2), indentCharacter);
}

std::string Printer::indent()
{
	return string(indentNum * 2, indentCharacter);
}

void Printer::forwardIndent()
{
	++indentNum;
}

void Printer::backwardIndent()
{
	if(indentNum != 0)
		--indentNum;
}

#ifdef ENV_TEST
void Printer::resetPrinter()
{
	indentNum = 0;
}
#endif

/// @ContextPrinter
ContextPrinter::ContextPrinter(Printer &p)
	: printer{p}
{}

std::string ContextPrinter::printContext(std::shared_ptr<DeclContext> context)
{
	string ret;
	// is top level context
	if(dynamic_pointer_cast<TranslationUnitDecl>(context))
		ret += printForwardDecleration(context);

	// if need to print main() function first
	/*if(dynamic_pointer_cast<FunctionDecl>(context->lookup("main").lock()))
		ret += printFunction(dynamic_pointer_cast<FunctionDecl>(context->lookup("main").lock()), true);*/

	for (auto it = context->decl_begin(); it != context->decl_end(); ++it)
	{
		switch ((*it)->getKind())
		{
			case Decl::Kind::Var:		ret += printVar((*it));		break;
			case Decl::Kind::Typedef:	ret += printTypedef(*it);	break;
			case Decl::Kind::Record:	ret += printRecord(*it);	break;
			case Decl::Kind::Function:	ret += printFunction(*it);	break;
			case Decl::Kind::Comment:	ret += printComment(*it);	break;
			default:					ret += "";					break;
		}
	}

	return ret;
}

std::string ContextPrinter::printForwardDecleration(std::shared_ptr<DeclContext> context)
{
	bool printedForward = false;
	string ret;
	for (auto it = context->decl_begin(); it != context->decl_end(); ++it)
	{
		switch ((*it)->getKind())
		{
			case Decl::Kind::Function:
				ret += printFunctionForwardDecl(*it);
				printedForward = true;
				break;
			default:
				ret += "";
				break;
		}
	}
	if(printedForward)
		ret += "\n";
	return ret;
}

std::string ContextPrinter::printFunctionForwardDecl(std::shared_ptr<Decl> decl)
{
	string ret;

	auto fun = dynamic_pointer_cast<FunctionDecl>(decl);
	if(fun->getNameAsString() == "main")
		return "";
	ret = printer.typePrinter.printTypePrefix(fun->getType().lock());
	ret += " " + fun->getNameAsString();
	ret += printer.typePrinter.printTypePostfix(fun->getType().lock());
	ret += ";\n";

	return ret;
}

std::string ContextPrinter::printVar(std::shared_ptr<Decl> decl)
{
	string ret;
	auto var = dynamic_pointer_cast<VarDecl>(decl);
	if(dynamic_pointer_cast<TypedefType>(var->getType().lock()->getTypePtr()))
		ret += dynamic_pointer_cast<TypedefType>(var->getType().lock()->getTypePtr())->getDecl().lock()->getNameAsString();
	else
		ret += printer.typePrinter.printTypePrefix(var->getType().lock());
	ret += " " + var->getNameAsString();
	ret += printer.typePrinter.printTypePostfix(var->getType().lock());
	ret += ";\n";

	return ret;
}

std::string ContextPrinter::printTypedef(std::shared_ptr<Decl> decl)
{
	string ret;

	auto typedefDecl = dynamic_pointer_cast<TypedefDecl>(decl);
	auto type = dynamic_pointer_cast<TypedefType>(typedefDecl->getTypeForDecl().lock()->getTypePtr());

	ret = "typedef ";
	ret += printer.typePrinter.printTypePrefix(type->getDeclForType().lock());
	ret += " " + typedefDecl->getNameAsString();
	ret += printer.typePrinter.printTypePostfix(type->getDeclForType().lock());
	ret += ";\n";

	return ret;
}

std::string ContextPrinter::printRecord(std::shared_ptr<Decl> decl)
{
	string ret;
	auto record = dynamic_pointer_cast<RecordDecl>(decl);
	if(!record->getIdentifier().lock()->isAnonymous())	// remove anonymous struct decl
	{
		ret = printer.typePrinter.printTypePrefix(record->getTypeForDecl().lock());
		ret += " " + record->getNameAsString();
		ret += ";\n";
	}
	return ret;
}

std::string ContextPrinter::printFunction(std::shared_ptr<Decl> decl)
{
	string ret;
	auto fun = dynamic_pointer_cast<FunctionDecl>(decl);

	ret = printer.typePrinter.printTypePrefix(fun->getType().lock());
	ret += " " + fun->getNameAsString();
	ret += printer.typePrinter.printTypePostfix(fun->getType().lock());

	if(fun->getBody().lock())
	{
		ret += "\n";
		ret += printer.astPrinter.printAST(fun->getBody().lock());
		ret += "\n";
	}
	else
		ret += ";";

	ret += "\n";
	return ret;
}

std::string ContextPrinter::printDecl(std::shared_ptr<Decl> decl)
{
	string ret;
	if(dynamic_pointer_cast<ValueDecl>(decl))
	{
		auto d = dynamic_pointer_cast<ValueDecl>(decl);
		auto type = d->getType().lock();

		if(dynamic_pointer_cast<TypedefType>(type->getTypePtr()))
			ret += dynamic_pointer_cast<TypedefType>(type->getTypePtr())->getDecl().lock()->getNameAsString();
		else
			ret += printer.typePrinter.printTypePrefix(type);

		//ret += printer.typePrinter.printTypePrefix(type);
		ret += " " + d->getNameAsString();
		ret += printer.typePrinter.printTypePostfix(type);
	}
	else if(dynamic_pointer_cast<TypeDecl>(decl))
	{
		auto d = dynamic_pointer_cast<TypeDecl>(decl);
		auto type = d->getTypeForDecl().lock();

		ret += printer.typePrinter.printTypePrefix(type);
		ret += " " + d->getNameAsString();
		ret += printer.typePrinter.printTypePostfix(type);
	}

	return ret;
	/*auto decl = dynamic_pointer_cast<TypedefDecl>(declRef->getSingleDecl().lock());

		m_openHelper.getOutputStream() << indent();
	m_openHelper.getOutputStream() << "typedef ";
	m_typePrinter.printTypePrefix(type->getDeclForType().lock());
	m_openHelper.getOutputStream() << " " << decl->getNameAsString();
	m_typePrinter.printTypePostfix(type->getDeclForType().lock());
	m_openHelper.getOutputStream() << ";";
		m_openHelper.getOutputStream() << "\n";*/
}

std::string ContextPrinter::printComment(std::shared_ptr<Decl> decl)
{
	return printer.astPrinter.printAST(dynamic_pointer_cast<CommentDecl>(decl)->getComment().lock()) + "\n";
}

/// @TypePrinter
TypePrinter::TypePrinter(Printer &p)
	: printer{p}
{}

std::string TypePrinter::printTypePrefix(std::shared_ptr<QualType> type)
{
	string ret = qualifierPrefix(type);
	switch(type->getTypePtr()->getTypeClass())
	{
		case Type::TypeClass::ExtQual:break;
		case Type::TypeClass::Builtin:					ret += formatBuiltinPrefix(builtinPrefix(type));	break;
		case Type::TypeClass::FixedWidthInt:break;
		case Type::TypeClass::Complex:break;
		case Type::TypeClass::Pointer:					ret += formatPointerPrefix(pointerPrefix(type));	break;
		case Type::TypeClass::BlockPointer:break;
		case Type::TypeClass::LValueReference:break;
		case Type::TypeClass::RValueReference:break;
		case Type::TypeClass::MemberPointer:break;
		case Type::TypeClass::ConstantArray:			ret += formatArrayPrefix(arrayPrefix(type));		break;
		case Type::TypeClass::ConstantArrayWithExpr:	ret += formatArrayPrefix(arrayPrefix(type));		break;
		case Type::TypeClass::ConstantArrayWithoutExpr:	ret += formatArrayPrefix(arrayPrefix(type));		break;
		case Type::TypeClass::IncompleteArray:			ret += formatArrayPrefix(arrayPrefix(type));		break;
		case Type::TypeClass::VariableArray:			ret += formatArrayPrefix(arrayPrefix(type));		break;
		case Type::TypeClass::DependentSizedArray:		ret += formatArrayPrefix(arrayPrefix(type));		break;
		case Type::TypeClass::DependentSizedExtVector:break;
		case Type::TypeClass::Vector:break;
		case Type::TypeClass::ExtVector:break;
		case Type::TypeClass::FunctionProto:			ret += formatFunctionPrefix(functionPrefix(type));	break;
		case Type::TypeClass::FunctionNoProto:			ret += formatFunctionPrefix(functionPrefix(type));	break;
		case Type::TypeClass::Typedef:					ret += formatTypedefPrefix(typedefPrefix(type));	break;
		case Type::TypeClass::TypeOfExpr:break;
		case Type::TypeClass::TypeOf:break;
		case Type::TypeClass::Decltype:break;
		case Type::TypeClass::Record:					ret += formatRecodePrefix(recordPrefix(type));		break;
		case Type::TypeClass::Enum:						ret += formatEnumPrefix(enumPrefix(type));			break;
		case Type::TypeClass::TemplateTypeParm:break;
		case Type::TypeClass::TemplateSpecialization:break;
		case Type::TypeClass::QualifiedName:break;
		case Type::TypeClass::Typename:break;
	}
	return ret;
}

std::string TypePrinter::printTypeInfix(std::shared_ptr<QualType> type)
{
	if(type->getTypePtr()->getTypeClass() == Type::TypeClass::Pointer)
		return "*";
	else
		return "";
}

std::string TypePrinter::printTypePostfix(std::shared_ptr<QualType> type)
{
	string ret;
	switch(type->getTypePtr()->getTypeClass())
	{
		case Type::TypeClass::ExtQual:					break;
		case Type::TypeClass::Builtin:					break;/*No postfix*/
		case Type::TypeClass::FixedWidthInt:			break;
		case Type::TypeClass::Complex:					break;
		case Type::TypeClass::Pointer:					break;/*No postfix*/
		case Type::TypeClass::BlockPointer:				break;
		case Type::TypeClass::LValueReference:			break;
		case Type::TypeClass::RValueReference:			break;
		case Type::TypeClass::MemberPointer:			break;
		case Type::TypeClass::ConstantArray:
		case Type::TypeClass::ConstantArrayWithExpr:
			ret = formatArrayWithExprPostfix(arrayWithExprPostfix(type));
			break;
		case Type::TypeClass::ConstantArrayWithoutExpr:
		case Type::TypeClass::IncompleteArray:
			ret =  formatArrayWithoutExprPostfix(arrayWithoutExprPostfix(type));
			break;
		case Type::TypeClass::VariableArray:			break;
		case Type::TypeClass::DependentSizedArray:		break;
		case Type::TypeClass::DependentSizedExtVector:	break;
		case Type::TypeClass::Vector:					break;
		case Type::TypeClass::ExtVector:				break;
		case Type::TypeClass::FunctionProto:
			ret =  formatFunctionProtoPostfix(functionProtoPostfix(type));
			break;
		case Type::TypeClass::FunctionNoProto:
			ret =  formatFunctionNoProtoPostfix(functionNoProtoPostfix(type));
			break;
		case Type::TypeClass::Typedef:					break;/*No postfix*/
		case Type::TypeClass::TypeOfExpr:				break;
		case Type::TypeClass::TypeOf:					break;
		case Type::TypeClass::Decltype:					break;
		case Type::TypeClass::Record:
			//recordPostfix(type);
			break;
		case Type::TypeClass::Enum:
			//enumPostfix(type);
			break;
		case Type::TypeClass::TemplateTypeParm:			break;
		case Type::TypeClass::TemplateSpecialization:	break;
		case Type::TypeClass::QualifiedName:			break;
		case Type::TypeClass::Typename:					break;
	}
	return ret;
}

std::string TypePrinter::qualifierPrefix(std::shared_ptr<QualType> type)
{
	string qualifiers;
	switch(type->getCVRQualifiers())
	{
		case QualType::Const | QualType::Restrict | QualType::Volatile:
			qualifiers = "const restrict volatile ";
			break;
		case QualType::Const | QualType::Restrict:
			qualifiers = "const restrict ";
			break;
		case QualType::Const | QualType::Volatile:
			qualifiers = "const volatile ";
			break;
		case QualType::Restrict | QualType::Volatile:
			qualifiers = "restrict volatile ";
			break;
		case QualType::Const:
			qualifiers = "const ";
			break;
		case QualType::Restrict:
			qualifiers = "restrict ";
			break;
		case QualType::Volatile:
			qualifiers = "volatile ";
			break;
		case QualType::None:
		default:
			qualifiers = "";
			break;
	}
	return qualifiers;
}

std::vector<std::string> TypePrinter::builtinPrefix(std::shared_ptr<QualType> type)
{
	vector<string> ret;
	std::string typeName;
	switch(dynamic_pointer_cast<BuiltinType>(type->getTypePtr())->getKind())
	{
		case BuiltinType::Void:			typeName = "void";					break;
		case BuiltinType::Bool:			typeName = "bool";					break;
		case BuiltinType::Char_U:		typeName = "unsigned char";			break;
		case BuiltinType::UChar:		typeName = "unsigned char";			break;
		case BuiltinType::Char16:		typeName = "void";					break;
		case BuiltinType::Char32:		typeName = "void";					break;
		case BuiltinType::UShort:		typeName = "unsigned short";		break;
		case BuiltinType::UInt:			typeName = "unsigned int";			break;
		case BuiltinType::ULong:		typeName = "unsigned long";			break;
		case BuiltinType::ULongLong:	typeName = "unsigned long long";	break;
		case BuiltinType::UInt128:		typeName = "void";break;
		case BuiltinType::Char_S:		typeName = "char";					break;
		case BuiltinType::SChar:		typeName = "char";					break;
		case BuiltinType::WChar:		typeName = "void";break;
		case BuiltinType::Short:		typeName = "short";					break;
		case BuiltinType::Int:			typeName = "int";					break;
		case BuiltinType::Long:			typeName = "long";					break;
		case BuiltinType::LongLong:		typeName = "long long";				break;
		case BuiltinType::Int128:		typeName = "void";break;
		case BuiltinType::Float:		typeName = "float";					break;
		case BuiltinType::Double:		typeName = "double";				break;
		case BuiltinType::LongDouble:	typeName = "long double";			break;
		case BuiltinType::NullPtr:		typeName = "void";break;
		case BuiltinType::Overload:		typeName = "void";break;
		case BuiltinType::Dependent:	typeName = "void";break;
		case BuiltinType::UndeducedAuto:typeName = "void";break;
	}
	ret.emplace_back(typeName);
	return ret;
}

std::vector<std::string> TypePrinter::pointerPrefix(std::shared_ptr<QualType> type)
{
	vector<string> ret;
	ret.emplace_back(printTypePrefix(dynamic_pointer_cast<PointerType>(type->getTypePtr())->getPointeeType().lock()));
	ret.emplace_back("*");
	return ret;
}

std::vector<std::string> TypePrinter::arrayPrefix(std::shared_ptr<QualType> type)
{
	vector<string> ret;
	ret.emplace_back(printTypePrefix(dynamic_pointer_cast<ArrayType>(type->getTypePtr())->getElementType().lock()));
	return ret;
}

std::vector<std::string> TypePrinter::functionPrefix(std::shared_ptr<QualType> type)
{
	vector<string> ret;
    ret.emplace_back(printTypePrefix(dynamic_pointer_cast<FunctionType>(type->getTypePtr())->getResultType().lock()));
	return ret;
}

std::vector<std::string> TypePrinter::typedefPrefix(std::shared_ptr<QualType> type)
{
	vector<string> ret;
	ret.emplace_back("typedef");
	ret.emplace_back(printTypePrefix(dynamic_pointer_cast<TypedefType>(type->getTypePtr())->getDeclForType().lock()));
	return ret;
}

std::vector<std::string> TypePrinter::recordPrefix(std::shared_ptr<QualType> type)
{
	vector<string> ret;
	ret.emplace_back("struct");
	ret.emplace_back("{");

	auto recordType = dynamic_pointer_cast<RecordType>(type->getTypePtr());
	auto decl = dynamic_pointer_cast<RecordDecl>(recordType->getDecl().lock());
	for(auto it = decl->decl_begin(); it != decl->decl_end(); ++it)
	{
		auto d = dynamic_pointer_cast<VarDecl>(*it);
		string declInStruct = printTypePrefix(d->getType().lock());
		declInStruct += " ";
		declInStruct += d->getNameAsString();
		declInStruct += printTypePostfix(d->getType().lock());
		declInStruct += ";";
		ret.emplace_back(declInStruct);
	}
	ret.emplace_back("}");

	return ret;
}

std::vector<std::string> TypePrinter::enumPrefix(std::shared_ptr<QualType> type)
{
	vector<string> ret;

	ret.emplace_back("enum");
	ret.emplace_back("{");
	auto enumType = dynamic_pointer_cast<EnumType>(type->getTypePtr());
	auto decl = dynamic_pointer_cast<EnumDecl>(enumType->getDecl().lock());
	unsigned enumConstantLocation = 0;
	for(auto it = decl->decl_begin(); it != decl->decl_end(); ++it, ++enumConstantLocation)
	{
		auto d = dynamic_pointer_cast<EnumConstantDecl>(*it);
		ret.emplace_back(d->getNameAsString());
		if(++it != decl->decl_end())
			ret.back() += ", ";
		--it;
	}
	ret.emplace_back("}");

	return ret;
}

std::string TypePrinter::formatBuiltinPrefix(std::vector<std::string> str)
{
	return str[0];
}

std::string TypePrinter::formatPointerPrefix(std::vector<std::string> str)
{
	string ret = str[0];
	for(int i = 1; i < str.size(); ++i)
		ret += str[i];
	return ret;
}

std::string TypePrinter::formatArrayPrefix(std::vector<std::string> str)
{
	return str[0];
}

std::string TypePrinter::formatFunctionPrefix(std::vector<std::string> str)
{
	return str[0];
}

std::string TypePrinter::formatTypedefPrefix(std::vector<std::string> str)
{
	return str[0] + " " + str[1];
}

std::string TypePrinter::formatRecodePrefix(std::vector<std::string> str)
{
	string ret = str[0] + " " + str[1] + "\n";
	printer.forwardIndent();
	for(int i = 2; i < str.size() - 1; ++i)
	{
		ret += printer.indent();
		ret += str[i] + "\n";
	}
	printer.backwardIndent();
	ret += printer.indent() + str.back();
	return ret;
}

std::string TypePrinter::formatEnumPrefix(std::vector<std::string> str)
{
	string ret = str[0] + " " + str[1] + "\n";
	printer.forwardIndent();
	ret += printer.indent();
	printer.backwardIndent();
	for(int i = 2; i < str.size() - 1; ++i)
		ret += str[i];
	ret += "\n" + str.back();
	return ret;
}

std::vector<std::string> TypePrinter::arrayWithExprPostfix(std::shared_ptr<QualType> type)
{
	vector<string> ret;
	auto arrayType = dynamic_pointer_cast<ConstantArrayWithExprType>(type->getTypePtr());
	ret.emplace_back("[" + printer.astPrinter.printAST(arrayType->getSizeSpecifier().lock()) + "]");
	ret.emplace_back(printTypePostfix(arrayType->getElementType().lock()));
	return ret;
}

std::vector<std::string> TypePrinter::arrayWithoutExprPostfix(std::shared_ptr<QualType> type)
{
	vector<string> ret;
	ret.emplace_back("[]");
	ret.emplace_back(printTypePostfix(dynamic_pointer_cast<ArrayType>(type->getTypePtr())->getElementType().lock()));
	return ret;
}

std::vector<std::string> TypePrinter::functionNoProtoPostfix(std::shared_ptr<QualType> type)
{
	vector<string> ret;
	ret.emplace_back("()");
	return ret;
}

std::vector<std::string> TypePrinter::functionProtoPostfix(std::shared_ptr<QualType> type)
{
	vector<string> ret;
	ret.emplace_back("(");
	auto fun = dynamic_pointer_cast<FunctionType>(type->getTypePtr());
	auto funDecl = fun->getFunDecl();
	for(int i = 0; i < funDecl->getNumParams(); ++i)
	{
		auto param = funDecl->getParamDecl(i).lock();
		string argStr;
		argStr = printTypePrefix(param->getType().lock());
		if(param->paramHasName())
			argStr += " " + param->getNameAsString();
		argStr += printTypePostfix(param->getType().lock());
		if(i != funDecl->getNumParams() - 1)
			argStr += ", ";
		ret.emplace_back(argStr);
	}
	ret.emplace_back(")");

	return ret;
}

std::string TypePrinter::formatArrayWithExprPostfix(std::vector<std::string> str)
{
	string ret;
	for(auto s : str)
		ret += s;
	return ret;
}

std::string TypePrinter::formatArrayWithoutExprPostfix(std::vector<std::string> str)
{
	string ret;
	for(auto s : str)
		ret += s;
	return ret;
}

std::string TypePrinter::formatFunctionProtoPostfix(std::vector<std::string> str)
{
	string ret;
	ret = str[0];
	for(int i = 1; i < str.size() - 1; ++i)
		ret += str[i];
	ret += str.back();
	return ret;
}

std::string TypePrinter::formatFunctionNoProtoPostfix(std::vector<std::string> str)
{
	return str[0];
}

/// @ASTPrinter
ASTPrinter::ASTPrinter(Printer &p)
	: printer{p}
{}

std::string ASTPrinter::printAST(std::shared_ptr<Stmt> root)
{
	string ret;
    if(!root)
        return "";
	switch(root->getStmtClass())
	{
		case Stmt::StmtClass::NoStmtClass:break;
		case Stmt::StmtClass::NullStmtClass:				ret = processNullStmt(root);				break;
		case Stmt::StmtClass::CompoundStmtClass:			ret = processCompoundStmt(root);			break;
		case Stmt::StmtClass::CaseStmtClass:				ret = processCaseStmt(root);				break;
		case Stmt::StmtClass::DefaultStmtClass:				ret = processDefaultStmt(root);				break;
		case Stmt::StmtClass::LabelStmtClass:				ret = processLabelStmt(root);				break;
		case Stmt::StmtClass::IfStmtClass:					ret = processIfStmt(root);					break;
		case Stmt::StmtClass::SwitchStmtClass:				ret = processSwitchStmt(root);				break;
		case Stmt::StmtClass::WhileStmtClass:				ret = processWhileStmt(root);				break;
		case Stmt::StmtClass::DoStmtClass:					ret = processDoStmt(root);					break;
		case Stmt::StmtClass::ForStmtClass:					ret = processForStmt(root);					break;
		case Stmt::StmtClass::GotoStmtClass:				ret = processGotoStmt(root);				break;
		case Stmt::StmtClass::IndirectGotoStmtClass:break;
		case Stmt::StmtClass::ContinueStmtClass:			ret = processContinueStmt(root);			break;
		case Stmt::StmtClass::BreakStmtClass:				ret = processBreakStmt(root);				break;
		case Stmt::StmtClass::ReturnStmtClass:				ret = processReturnStmt(root);				break;
		case Stmt::StmtClass::DeclStmtClass:				ret = processDeclStmt(root);				break;
		case Stmt::StmtClass::SwitchCaseClass:break;
		case Stmt::StmtClass::AsmStmtClass:break;
		case Stmt::StmtClass::CommentStmtClass:             ret = processCommentStmt(root);				break;
		case Stmt::StmtClass::CXXCatchStmtClass:break;
		case Stmt::StmtClass::CXXTryStmtClass:break;
		case Stmt::StmtClass::ExprClass:break;
		case Stmt::StmtClass::PredefinedExprClass:break;
		case Stmt::StmtClass::DeclRefExprClass:				ret = processRefExpr(root);					break;
		case Stmt::StmtClass::IntegerLiteralClass:			ret = processIntergerLiteral(root);			break;
		case Stmt::StmtClass::FloatingLiteralClass:			ret = processFloatingLiteral(root);			break;
		case Stmt::StmtClass::ImaginaryLiteralClass:break;
		case Stmt::StmtClass::StringLiteralClass:			ret = processStringLiteral(root);			break;
		case Stmt::StmtClass::CharacterLiteralClass:		ret = processCharacterLiteral(root);		break;
		case Stmt::StmtClass::ParenExprClass:				ret = processParenExpr(root);				break;
		case Stmt::StmtClass::UnaryOperatorClass:			ret = processUnaryOperator(root);			break;
		case Stmt::StmtClass::SizeOfAlignOfExprClass:		ret = processSizeOfAlignOfExpr(root);		break;
		case Stmt::StmtClass::ArraySubscriptExprClass:		ret = processArraySubscriptExpr(root);			break;
		case Stmt::StmtClass::CallExprClass:				ret = processCallExpr(root);				break;
		case Stmt::StmtClass::MemberExprClass:				ret = processMemberExpr(root);				break;
		case Stmt::StmtClass::CastExprClass:break;
		case Stmt::StmtClass::BinaryOperatorClass:			ret = processBinaryOperator(root);			break;
		case Stmt::StmtClass::CompoundAssignOperatorClass:	ret = processCompoundAssignOperator(root);	break;
		case Stmt::StmtClass::ConditionalOperatorClass:		ret = processConditionalOperator(root);		break;
		case Stmt::StmtClass::ImplicitCastExprClass:break;
		case Stmt::StmtClass::ExplicitCastExprClass:break;
		case Stmt::StmtClass::CStyleCastExprClass:			ret = processCStyleCastExpr(root);			break;
		case Stmt::StmtClass::CompoundLiteralExprClass:break;
		case Stmt::StmtClass::ExtVectorElementExprClass:break;
		case Stmt::StmtClass::InitListExprClass:break;
		case Stmt::StmtClass::DesignatedInitExprClass:break;
		case Stmt::StmtClass::ImplicitValueInitExprClass:break;
		case Stmt::StmtClass::ParenListExprClass:break;
		case Stmt::StmtClass::VAArgExprClass:break;
		case Stmt::StmtClass::AddrLabelExprClass:break;
		case Stmt::StmtClass::StmtExprClass:break;
		case Stmt::StmtClass::TypesCompatibleExprClass:break;
		case Stmt::StmtClass::ChooseExprClass:break;
		case Stmt::StmtClass::GNUNullExprClass:break;
		case Stmt::StmtClass::CXXOperatorCallExprClass:break;
		case Stmt::StmtClass::CXXMemberCallExprClass:break;
		case Stmt::StmtClass::CXXNamedCastExprClass:break;
		case Stmt::StmtClass::CXXStaticCastExprClass:break;
		case Stmt::StmtClass::CXXDynamicCastExprClass:break;
		case Stmt::StmtClass::CXXReinterpretCastExprClass:break;
		case Stmt::StmtClass::CXXConstCastExprClass:break;
		case Stmt::StmtClass::CXXFunctionalCastExprClass:break;
		case Stmt::StmtClass::CXXTypeidExprClass:break;
		case Stmt::StmtClass::CXXBoolLiteralExprClass:break;
		case Stmt::StmtClass::CXXNullPtrLiteralExprClass:break;
		case Stmt::StmtClass::CXXThisExprClass:break;
		case Stmt::StmtClass::CXXThrowExprClass:break;
		case Stmt::StmtClass::CXXDefaultArgExprClass:break;
		case Stmt::StmtClass::CXXZeroInitValueExprClass:break;
		case Stmt::StmtClass::CXXConditionDeclExprClass:break;
		case Stmt::StmtClass::CXXNewExprClass:break;
		case Stmt::StmtClass::CXXDeleteExprClass:break;
		case Stmt::StmtClass::UnresolvedFunctionNameExprClass:break;
		case Stmt::StmtClass::UnaryTypeTraitExprClass:break;
		case Stmt::StmtClass::QualifiedDeclRefExprClass:break;
		case Stmt::StmtClass::UnresolvedDeclRefExprClass:break;
		case Stmt::StmtClass::TemplateIdRefExprClass:break;
		case Stmt::StmtClass::CXXConstructExprClass:break;
		case Stmt::StmtClass::CXXBindTemporaryExprClass:break;
		case Stmt::StmtClass::CXXExprWithTemporariesClass:break;
		case Stmt::StmtClass::CXXTemporaryObjectExprClass:break;
		case Stmt::StmtClass::CXXUnresolvedConstructExprClass:break;
		case Stmt::StmtClass::CXXUnresolvedMemberExprClass:break;
		case Stmt::StmtClass::ShuffleVectorExprClass:break;
		case Stmt::StmtClass::BlockExprClass:break;
		case Stmt::StmtClass::BlockDeclRefExprClass:break;
	}
	return ret;
}

std::string ASTPrinter::formatExprAsStmt(std::string str, std::shared_ptr<Stmt> s)
{
	return dynamic_pointer_cast<Expr>(s) ? str + ";" : str;
}

std::string ASTPrinter::processCallExpr(std::shared_ptr<Stmt> &s)
{
	string ret;

	auto call = dynamic_pointer_cast<CallExpr>(s);
	ret = printAST(call->getCallee().lock()) + "(";
	auto args = call->getArgs();
	// TODO: use child_begin/child_end instead
	unsigned argPosition = 0;
	for(auto it = ++args.begin(); it != args.end(); ++it, ++argPosition)
	{
		ret += printAST(*it);
		// skip last ', '
		if(argPosition != call->getNumArgs() - 1)
			ret += ", ";
	}
	ret += ")";

	return ret;
}

std::string ASTPrinter::processNullStmt(std::shared_ptr<Stmt> &s)
{
	return ";";
}

std::string ASTPrinter::processCompoundStmt(std::shared_ptr<Stmt> &s)
{
	// TODO: update regex to remove "case" and "default:" in string
	regex caseRegex("case\\s.*:");
	regex defaultRegex("default:");

	bool inSwitch = false;
	string ret;
	printer.forwardIndent();
	ret += printer.oneIndentLessIndent() + "{\n";
	for(auto stmt = s->child_begin(); stmt != s->child_end(); ++stmt)
	{
		string str = formatExprAsStmt(printAST(*stmt), *stmt);
		// do not decorate self
		if(CompoundStmt::classof(*stmt))
			ret += str + "\n";
		else
		{
			// is switch
			if (std::regex_match(str, caseRegex) || std::regex_match(str, defaultRegex))
				inSwitch = true;
			// format switch
			if (inSwitch)
			{
				if (std::regex_match(str, caseRegex) || std::regex_match(str, defaultRegex))
					ret += printer.indent() + str + "\n";
				else
					ret += printer.indent() + printer.indent() + str + "\n";
			} else // others
				ret += printer.indent() + str + "\n";
		}
	}
	printer.backwardIndent();
	ret += printer.indent() + "}";

	return ret;
}

std::string ASTPrinter::processUnaryOperator(std::shared_ptr<Stmt> &s)
{
	string ret;
	auto unary = dynamic_pointer_cast<UnaryOperator>(s);
	auto subExpr = unary->getSubExpr().lock();
	switch (unary->getOpcode())
	{
		case UnaryOperator::PostInc:
			ret = printAST(subExpr);
			ret += "++";
			break;
		case UnaryOperator::PostDec:
			ret = printAST(subExpr);
			ret += "--";
			break;
		case UnaryOperator::PreInc:
			ret = "++";
			ret += printAST(subExpr);
			break;
		case UnaryOperator::PreDec:
			ret = "--";
			ret += printAST(subExpr);
			break;
		case UnaryOperator::AddrOf:
			ret = "&";
			ret += printAST(subExpr);
			break;
		case UnaryOperator::Deref:
			ret = "*";
			ret += printAST(subExpr);
			break;
		case UnaryOperator::Plus:
			ret = "+";
			ret += printAST(subExpr);
			break;
		case UnaryOperator::Minus:
			ret = "-";
			ret += printAST(subExpr);
			break;
		case UnaryOperator::Not:
			ret = "~";
			ret += printAST(subExpr);
			break;
		case UnaryOperator::LNot:
			ret = "!";
			ret += printAST(subExpr);
			break;
		case UnaryOperator::Real:break;
		case UnaryOperator::Imag:break;
		case UnaryOperator::Extension:break;
		case UnaryOperator::OffsetOf:break;
	}
	return ret;
}

std::string ASTPrinter::processIntergerLiteral(std::shared_ptr<Stmt> &s)
{
	auto integer = dynamic_pointer_cast<IntegerLiteral>(s);
	return to_string(integer->getValue());
}

std::string ASTPrinter::processFloatingLiteral(std::shared_ptr<Stmt> &s)
{
	auto floating = dynamic_pointer_cast<FloatingLiteral>(s);
	return toString(floating->getValue());
}

std::string ASTPrinter::processBinaryOperator(std::shared_ptr<Stmt> &s)
{
	string ret;

	auto binary = dynamic_pointer_cast<BinaryOperator>(s);
	ret = printAST(binary->getLHS().lock());
	switch (binary->getOpcode())
	{
		case BinaryOperator::PtrMemD:	ret += ".";	break;
		case BinaryOperator::PtrMemI:	ret += "->";break;
		case BinaryOperator::Mul:		ret += "*";	break;
		case BinaryOperator::Div:		ret += "/";	break;
		case BinaryOperator::Rem:		ret += "%";	break;
		case BinaryOperator::Add:		ret += "+";	break;
		case BinaryOperator::Sub:		ret += "-";	break;
		case BinaryOperator::Shl:		ret += "<<";break;
		case BinaryOperator::Shr:		ret += ">>";break;
		case BinaryOperator::LT:		ret += "<";	break;
		case BinaryOperator::GT:		ret += ">";	break;
		case BinaryOperator::LE:		ret += "<=";break;
		case BinaryOperator::GE:		ret += ">=";break;
		case BinaryOperator::EQ:		ret += "==";break;
		case BinaryOperator::NE:		ret += "!=";break;
		case BinaryOperator::And:		ret += "&";	break;
		case BinaryOperator::Xor:		ret += "^";	break;
		case BinaryOperator::Or:		ret += "|";	break;
		case BinaryOperator::LAnd:		ret += "&&";break;
		case BinaryOperator::LOr:		ret += "||";break;
		case BinaryOperator::Comma:		ret += ",";	break;
	}
	ret += printAST(binary->getRHS().lock());

	return ret;
}

std::string ASTPrinter::processCompoundAssignOperator(std::shared_ptr<Stmt> &s)
{
	string ret;

	auto binary = dynamic_pointer_cast<BinaryOperator>(s);
	ret = printAST(binary->getLHS().lock());
	switch (binary->getOpcode())
	{
		case BinaryOperator::Assign:	ret += "=";		break;
		case BinaryOperator::MulAssign:	ret += "*=";	break;
		case BinaryOperator::DivAssign:	ret += "/=";	break;
		case BinaryOperator::RemAssign:	ret += "%=";	break;
		case BinaryOperator::AddAssign:	ret += "+=";	break;
		case BinaryOperator::SubAssign:	ret += "-=";	break;
		case BinaryOperator::ShlAssign:	ret += "<<=";	break;
		case BinaryOperator::ShrAssign:	ret += ">>=";	break;
		case BinaryOperator::AndAssign:	ret += "&=";	break;
		case BinaryOperator::XorAssign:	ret += "^=";	break;
		case BinaryOperator::OrAssign:	ret += "|=";	break;
	}
	ret += printAST(binary->getRHS().lock());
	return ret;
}

std::string ASTPrinter::processConditionalOperator(std::shared_ptr<Stmt> &s)
{
	string ret;
	auto condition = dynamic_pointer_cast<ConditionalOperator>(s);
	ret = printAST(condition->getCond().lock());
	ret += "?";
	ret += printAST(condition->getLHS().lock());
	ret += ":";
	ret += printAST(condition->getRHS().lock());
	return ret;
}

std::string ASTPrinter::processContinueStmt(std::shared_ptr<Stmt> &s)
{
	return "continue;";
}

std::string ASTPrinter::processBreakStmt(std::shared_ptr<Stmt> &s)
{
	return "break;";
}

std::string ASTPrinter::processReturnStmt(std::shared_ptr<Stmt> &s)
{
	string ret = "return ";
	auto retStmt = dynamic_pointer_cast<ReturnStmt>(s);
	if(retStmt->getRetValue().lock())
		ret += printAST(retStmt->getRetValue().lock());
	ret += ";";
	return ret;
}

std::string ASTPrinter::processParenExpr(std::shared_ptr<Stmt> &s)
{
	string ret;
	auto paren = dynamic_pointer_cast<ParenExpr>(s);
	ret = "(";
	ret += printAST(paren->getSubExpr().lock());
	ret += ")";
	return ret;
}

std::string ASTPrinter::processSizeOfAlignOfExpr(std::shared_ptr<Stmt> &s)
{
	string ret = "sizeof ";
	auto sizeofExp = dynamic_pointer_cast<SizeOfAlignOfExpr>(s);
	if(sizeofExp->isArgumentType())
		;		// TODO: sizeof type
	else
		ret += printAST(sizeofExp->getArgumentExpr().lock());
	return ret;
}

std::string ASTPrinter::processArraySubscriptExpr(std::shared_ptr<Stmt> &s)
{
	auto arr = dynamic_pointer_cast<ArraySubscriptExpr>(s);
	string ret = printAST(arr->getLHS().lock());
	ret += "[";
	ret += printAST(arr->getRHS().lock());
	ret += "]";
	return ret;
}

std::string ASTPrinter::processStringLiteral(std::shared_ptr<Stmt> &s)
{
	auto literal = dynamic_pointer_cast<StringLiteral>(s);
	return string(literal->getStrData());
}

std::string ASTPrinter::processCharacterLiteral(std::shared_ptr<Stmt> &s)
{
	auto character = dynamic_pointer_cast<CharacterLiteral>(s);
	string output;
	switch(character->getValue())
	{
		case '\a':	output = "\\a";	break;
		case '\b':	output = "\\b";	break;
		case '\f':	output = "\\f";	break;
		case '\n':	output = "\\n";	break;
		case '\r':	output = "\\r";	break;
		case '\t':	output = "\\t";	break;
		case '\v':	output = "\\v";	break;
		case '\'':	output = "\\\'";break;
		case '\"':	output = "\\\"";break;
		case '\\':	output = "\\\\";break;
		case '\0':	output = "\\0";	break;
		default:	output = static_cast<char>(character->getValue());	break;
	}
	return "'" + output + "'";
}

std::string ASTPrinter::processWhileStmt(std::shared_ptr<Stmt> &s)
{
	auto whileStmt = dynamic_pointer_cast<WhileStmt>(s);
	string ret = "while (";
	ret += printAST(whileStmt->getCond().lock());
	ret += ")\n";
	auto body = whileStmt->getBody().lock();
	if(CompoundStmt::classof(body))
		ret += printAST(body);
	else
	{
		printer.forwardIndent();
		ret += printer.indent() + formatExprAsStmt(printAST(body), body);
		printer.backwardIndent();
	}
	return ret;
}

std::string ASTPrinter::processIfStmt(std::shared_ptr<Stmt> &s)
{
	auto ifStmt = dynamic_pointer_cast<IfStmt>(s);
	string ret = "if (";
	ret += printAST(ifStmt->getCond().lock());
	ret += ")\n";
	auto thenBlock = ifStmt->getThen().lock();
	if(CompoundStmt::classof(thenBlock))
		ret += printAST(thenBlock);
	else
	{
		printer.forwardIndent();
		ret += printer.indent() + formatExprAsStmt(printAST(thenBlock), thenBlock);
		printer.backwardIndent();
	}

	auto elseBlock = ifStmt->getElse().lock();
	if(elseBlock)
	{
		ret += "\n" + printer.indent() + "else ";
		if(IfStmt::classof(elseBlock))
			ret += processIfStmt(elseBlock);
		else if(CompoundStmt::classof(elseBlock))
			ret += "\n" + printAST(elseBlock);
		else
			ret += printer.indent() + formatExprAsStmt(printAST(elseBlock), elseBlock);
	}
	return ret;
}

std::string ASTPrinter::processDoStmt(std::shared_ptr<Stmt> &s)
{
	auto doStmt = dynamic_pointer_cast<DoStmt>(s);
	string ret = "do\n";
	auto body = doStmt->getBody().lock();
	if(CompoundStmt::classof(body))
		ret += printAST(body) + " ";
	else
	{
		printer.forwardIndent();
		ret += printer.indent() + formatExprAsStmt(printAST(body), body) + "\n";
		printer.backwardIndent();
	}
	ret += "while (";
	ret += printAST(doStmt->getCond().lock());
	ret += ")";
	return ret;
}

std::string ASTPrinter::processForStmt(std::shared_ptr<Stmt> &s)
{
	auto forStmt = dynamic_pointer_cast<ForStmt>(s);
	string ret = "for (";
	ret += formatExprAsStmt(printAST(forStmt->getInit().lock()), forStmt->getInit().lock());
	ret += formatExprAsStmt(printAST(forStmt->getCond().lock()), forStmt->getCond().lock());
	if(forStmt->getInc().lock())
		ret += printAST(forStmt->getInc().lock());
	ret += ")\n";
	auto body = forStmt->getBody().lock();
	if(CompoundStmt::classof(forStmt->getBody().lock()))
		ret += printAST(body);
	else
	{
		printer.forwardIndent();
		ret += printer.indent() + formatExprAsStmt(printAST(body), body);
		printer.backwardIndent();
	}
	return ret;
}

std::string ASTPrinter::processCaseStmt(std::shared_ptr<Stmt> &s)
{
	auto caseStmt = dynamic_pointer_cast<CaseStmt>(s);
	string ret = "case ";
	ret += printAST(caseStmt->getLHS().lock());
	ret += ":";
	return ret;
}

std::string ASTPrinter::processRefExpr(std::shared_ptr<Stmt> &s)
{
	auto ref = dynamic_pointer_cast<DeclRefExpr>(s);
	// is pointer and not assigned
	if (printer.m_detectNullPointer &&
		ref->getType().lock()->getTypePtr()->getTypeClass() == Type::TypeClass::Pointer &&
		!ref->getDecl().lock()->isAssigned())
	{
		auto decl = ref->getDecl().lock();
		throw NullPointerError(
				"NULL pointer for " + decl->getNameAsString() +
				"(" + to_string(decl->getSourceLocation().line()) + "," +
				to_string(decl->getSourceLocation().bytes()) + ")");
	}
	return ref->getDecl().lock()->getNameAsString();
}

std::string ASTPrinter::processSwitchStmt(std::shared_ptr<Stmt> &s)
{
	auto switchStmt = dynamic_pointer_cast<SwitchStmt>(s);
	string ret = "switch(";
	ret += printAST(switchStmt->getCond().lock());
	ret += ")\n";
	ret += printAST(switchStmt->getBody().lock());
	return ret;
}

std::string ASTPrinter::processDefaultStmt(std::shared_ptr<Stmt> &s)
{
	auto defaultStmt = dynamic_pointer_cast<DefaultStmt>(s);
	return "default:";
}

std::string ASTPrinter::processCStyleCastExpr(std::shared_ptr<Stmt> &s)
{
	auto caseExpr = dynamic_pointer_cast<CStyleCastExpr>(s);
	auto type = caseExpr->getType().lock();
	string ret = "(";
	ret += printer.typePrinter.printTypePrefix(type);
	ret += ")";
	ret += printAST(caseExpr->getSubExpr().lock());
	return ret;
}

// TODO: move type print into TypePinter
std::string ASTPrinter::processDeclStmt(std::shared_ptr<Stmt> &s)
{
	auto declStmt = dynamic_pointer_cast<DeclStmt>(s);
	auto declRef = declStmt->getDeclGroup().lock();
	string ret;
	if(!declRef)
	{
		/*m_openHelper.getOutputStream() << ";";*/
	}
	else if(declRef->isSingleDecl())
	{
		// variables
		if(dynamic_pointer_cast<VarDecl>(declRef->getSingleDecl().lock()))
		{
			// print type and name
			auto decl = dynamic_pointer_cast<VarDecl>(declRef->getSingleDecl().lock());
			ret += printer.contextPrinter.printDecl(decl);
			if(decl->getInitExpr().lock())	// has init expr
				ret += " = " + printAST(decl->getInitExpr().lock());
		}
		else
			ret += printer.contextPrinter.printDecl(declRef->getSingleDecl().lock());
	}
	else if(declRef->isDeclGroup())
	{
		auto decls = declRef->getDeclGroup().lock();
		// variable
		if(dynamic_pointer_cast<VarDecl>((*decls)[0]))
		{
			auto type = dynamic_pointer_cast<VarDecl>((*decls)[0])->getType().lock();

			// print first decl
			ret += printer.contextPrinter.printDecl((*decls)[0]);
			if(dynamic_pointer_cast<VarDecl>((*decls)[0])->getInitExpr().lock())	// has init expr
				ret += " = " + printAST(dynamic_pointer_cast<VarDecl>((*decls)[0])->getInitExpr().lock());
			ret += ", ";

			for (unsigned i = 1; i < decls->size(); ++i)
			{
				auto varDecl = dynamic_pointer_cast<VarDecl>((*decls)[i]);
				type = varDecl->getType().lock();
				// in fix and post fix
				ret += printer.typePrinter.printTypeInfix(type);
				ret += varDecl->getNameAsString();
				ret += printer.typePrinter.printTypePostfix(type);
				if(varDecl->getInitExpr().lock())	// init expr
					ret += " = " + printAST(varDecl->getInitExpr().lock());
				// skip last ','
				if (i + 1 != decls->size())
					ret += ", ";
			}
		}
		else	// typedef
		{
			auto type = dynamic_pointer_cast<TypedefType>(
					dynamic_pointer_cast<TypedefDecl>((*decls)[0])->getTypeForDecl().lock()->getTypePtr()
			);

			// print first decl
			ret += printer.contextPrinter.printDecl((*decls)[0]);
			ret += ", ";

			for (unsigned i = 1; i < decls->size(); ++i)
			{
				type = dynamic_pointer_cast<TypedefType>(
						dynamic_pointer_cast<TypedefDecl>((*decls)[i])->getTypeForDecl().lock()->getTypePtr()
				);
				ret += printer.typePrinter.printTypeInfix(type->getDeclForType().lock());
				ret += dynamic_pointer_cast<TypedefDecl>((*decls)[i])->getNameAsString();
				ret += printer.typePrinter.printTypePostfix(type->getDeclForType().lock());
				if (i + 1 != decls->size())
					ret += ", ";
			}
		}
	}
	ret += ";";

	return ret;
}

std::string ASTPrinter::processCommentStmt(std::shared_ptr<Stmt> &s)
{
    auto ptr = dynamic_pointer_cast<CommentStmt>(s);
    return ptr->getComment();
}

std::string ASTPrinter::processMemberExpr(std::shared_ptr<Stmt> &s)
{
	auto member = dynamic_pointer_cast<MemberExpr>(s);
	string ret = printAST(member->getBase().lock());
	ret += member->isArrow() ? "->" : ".";
	ret += member->getMemberDecl().lock()->getNameAsString();
	return ret;
}

std::string ASTPrinter::processLabelStmt(std::shared_ptr<Stmt> &s)
{
	string ret;
	auto label = dynamic_pointer_cast<LabelStmt>(s);
	ret += dynamic_pointer_cast<GotoDecl>(label->getID().lock())->getNameAsString() + ":\n";
	printer.forwardIndent();
	ret += printer.indent();
	ret += formatExprAsStmt(printAST(label->getSubStmt().lock()), label->getSubStmt().lock());
	printer.backwardIndent();
	return ret;
}

std::string ASTPrinter::processGotoStmt(std::shared_ptr<Stmt> &s)
{
	string ret;
	auto gotoStmt = dynamic_pointer_cast<GotoStmt>(s);
	ret += "goto " + dynamic_pointer_cast<GotoDecl>(gotoStmt->getLabel().lock()->getID().lock())->getNameAsString() + ";";
	return ret;
}
