#ifndef ASTCONTEXT_H
#define ASTCONTEXT_H

#include <memory>
#include <variant>
#include <list>
#include <vector>
#include "AST/AST.hpp"
#include "Decl/Decl.h"
#include "Decl/DeclGroup.h"

class ASTContext
{
public:
	/// @brief Package all needed type together
	typedef std::variant<
			unsigned, unsigned long, int, double, float, bool, char *, std::string, SourceLocation,
			UnaryOperator::Opcode, BinaryOperator::Opcode, PredefinedExpr::IdentType, BuiltinType::Kind,
			Type::TypeClass, CastExpr::CastKind, ArrayType::ArraySizeModifier, QualType::TQ,
			std::vector<std::shared_ptr<Stmt>>, std::vector<std::shared_ptr<Expr>>,
			std::list<std::shared_ptr<Stmt>>, std::list<std::shared_ptr<Expr>>,
			std::shared_ptr<Expr>, std::shared_ptr<Stmt>, std::shared_ptr<QualType>, std::shared_ptr<NamedDecl>,
			std::shared_ptr<TypedefDecl>, std::shared_ptr<DeclGroupRef>, std::shared_ptr<Decl>, std::shared_ptr<RecordDecl>,
			std::vector<std::shared_ptr<QualType>>, std::shared_ptr<EnumDecl>
	> var_t;

	/// @brief Create stmt by type
	template<typename... Args>
	std::shared_ptr<Stmt> createStmt(Stmt::StmtClass type, Args... args)
	{
		std::vector<var_t> value;
		(value.push_back(var_t{args}), ...);
		std::shared_ptr<Stmt> stmt;
		switch (type) {
			case Stmt::StmtClass::DeclStmtClass:				return createDeclStmt(value);
			case Stmt::StmtClass::NullStmtClass:				return createNullStmt(value);
			case Stmt::StmtClass::CompoundStmtClass:			return createCompoundStmt(value);
			case Stmt::StmtClass::CaseStmtClass:				return createCaseStmt(value);
			case Stmt::StmtClass::DefaultStmtClass:				return createDefaultStmt(value);
			case Stmt::StmtClass::LabelStmtClass:				return createLabelStmt(value);
			case Stmt::StmtClass::IfStmtClass:					return createIfStmt(value);
			case Stmt::StmtClass::SwitchStmtClass:				return createSwitchStmt(value);
			case Stmt::StmtClass::WhileStmtClass:				return createWhileStmt(value);
			case Stmt::StmtClass::DoStmtClass:					return createDoStmt(value);
			case Stmt::StmtClass::ForStmtClass:					return createForStmt(value);
			case Stmt::StmtClass::GotoStmtClass:				return createGotoStmt(value);
			case Stmt::StmtClass::IndirectGotoStmtClass:		return createIndirectGotoStmt(value);
			case Stmt::StmtClass::ContinueStmtClass:			return createContinueStmt(value);
			case Stmt::StmtClass::BreakStmtClass:				return createBreakStmt(value);
			case Stmt::StmtClass::ReturnStmtClass:				return createReturnStmt(value);
			case Stmt::StmtClass::CommentStmtClass:				return createCommentStmt(value);
			case Expr::StmtClass::DeclRefExprClass:				return createDeclRefExpr(value);
			case Expr::StmtClass::PredefinedExprClass:			return createPredefinedExpr(value);
			case Expr::StmtClass::IntegerLiteralClass:			return createIntegerLiteral(value);
			case Expr::StmtClass::CharacterLiteralClass:		return createCharacterLiteral(value);
			case Expr::StmtClass::FloatingLiteralClass:			return createFloatingLiteral(value);
			case Expr::StmtClass::ImaginaryLiteralClass:		return createImaginaryLiteral(value);
			case Expr::StmtClass::StringLiteralClass:			return createStringLiteral(value);
			case Expr::StmtClass::ParenExprClass:				return createParenExpr(value);
			case Expr::StmtClass::UnaryOperatorClass:			return createUnaryOperator(value);
			case Expr::StmtClass::SizeOfAlignOfExprClass:		return createSizeOfAlignOfExpr(value);
			case Expr::StmtClass::ArraySubscriptExprClass:		return createArraySubscriptExpr(value);
			case Expr::StmtClass::CallExprClass:				return createCallExpr(value);
			case Expr::StmtClass::MemberExprClass:				return createMemberExpr(value);
			case Expr::StmtClass::CompoundLiteralExprClass:		return createCompoundLiteralExpr(value);
			case Expr::StmtClass::ImplicitCastExprClass:		return createImplicitCastExpr(value);
			case Expr::StmtClass::CStyleCastExprClass:			return createCStyleCastExpr(value);
			case Expr::StmtClass::CompoundAssignOperatorClass:	return createCompoundAssignOperator(value);
			case Expr::StmtClass::ConditionalOperatorClass:		return createConditionalOperator(value);
			case Stmt::StmtClass::BinaryOperatorClass:			return createBinaryOperator(value);
			/*case Expr::AddrLabelExprClass:createAddrLabelExpr(value);break;*/
			/*case Expr::StmtExprClass:createStmtExpr(value);break;*/
			/*case Expr::TypesCompatibleExprClass:createTypesCompatibleExpr(value);break;*/
			/*case Expr::ShuffleVectorExprClass:createShuffleVectorExpr(value);break;*/
			/*case Expr::ChooseExprClass:createChooseExpr(value);break;*/
			/*case Expr::GNUNullExprClass:createGNUNullExpr(value);break;*/
			case Expr::StmtClass::VAArgExprClass:				return createVAArgExpr(value);
			case Expr::StmtClass::InitListExprClass:			return createInitListExpr(value);
			case Expr::StmtClass::ParenListExprClass:			return createParenListExpr(value);
			case Stmt::StmtClass::NoStmtClass:break;
			case Stmt::StmtClass::SwitchCaseClass:break;
			case Stmt::StmtClass::AsmStmtClass:break;
			case Stmt::StmtClass::CXXCatchStmtClass:break;
			case Stmt::StmtClass::CXXTryStmtClass:break;
			case Stmt::StmtClass::ExprClass:break;
			case Stmt::StmtClass::CastExprClass:break;
			case Stmt::StmtClass::ExplicitCastExprClass:break;
			case Stmt::StmtClass::ExtVectorElementExprClass:break;
			case Stmt::StmtClass::DesignatedInitExprClass:break;
			case Stmt::StmtClass::ImplicitValueInitExprClass:break;
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

		// silent no return statement warnings
		return nullptr;
	}

	/// @brief Create type by given type
	template<typename... Args>
	std::shared_ptr<QualType> createType(Type::TypeClass type, Args... args)
	{
		std::vector<var_t> value;
		(value.push_back(var_t{args}), ...);

		switch (type) {
			case Type::TypeClass::ExtQual:break;
			case Type::TypeClass::Builtin:					return createBuiltinType(value);
			case Type::TypeClass::FixedWidthInt:break;
			case Type::TypeClass::Complex:break;
			case Type::TypeClass::Pointer:					return createPointerType(value);
			case Type::TypeClass::BlockPointer:break;
			case Type::TypeClass::LValueReference:break;
			case Type::TypeClass::RValueReference:break;
			case Type::TypeClass::MemberPointer:break;
			case Type::TypeClass::ConstantArray:break;
			case Type::TypeClass::ConstantArrayWithExpr:	return createConstantArrayWithExprType(value);
			case Type::TypeClass::ConstantArrayWithoutExpr:break;
			case Type::TypeClass::IncompleteArray:			return createIncompleteArrayType(value);
			case Type::TypeClass::VariableArray:break;
			case Type::TypeClass::DependentSizedArray:break;
			case Type::TypeClass::DependentSizedExtVector:break;
			case Type::TypeClass::Vector:break;
			case Type::TypeClass::ExtVector:break;
			case Type::TypeClass::FunctionProto:			return createFunctionProtoType(value);
			case Type::TypeClass::FunctionNoProto:			return createFunctionNoProtoType(value);
			case Type::TypeClass::Typedef:					return createTypedefType(value);
			case Type::TypeClass::TypeOfExpr:break;
			case Type::TypeClass::TypeOf:break;
			case Type::TypeClass::Decltype:break;
			case Type::TypeClass::Record:					return createRecordType(value);
			case Type::TypeClass::Enum:						return createEnumType(value);
			case Type::TypeClass::TemplateTypeParm:break;
			case Type::TypeClass::TemplateSpecialization:break;
			case Type::TypeClass::QualifiedName:break;
			case Type::TypeClass::Typename:break;
		}

		// silent no return statement warnings
		return nullptr;
	}

	void updateASTRoot(std::shared_ptr<Stmt> root);

    std::weak_ptr<Stmt> getRoot() const;
    void cleanAST();

    /// @brief Not allow copy
    ASTContext &operator =(ASTContext &) = delete;

private:
	/// @brief Do create stmts
	std::shared_ptr<Stmt> createDeclStmt(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createNullStmt(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createCompoundStmt(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createCaseStmt(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createDefaultStmt(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createLabelStmt(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createIfStmt(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createSwitchStmt(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createWhileStmt(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createDoStmt(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createForStmt(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createGotoStmt(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createIndirectGotoStmt(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createContinueStmt(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createBreakStmt(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createReturnStmt(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createCommentStmt(std::vector<var_t> &value);

	/// @brief Do create exprs
	std::shared_ptr<Stmt> createDeclRefExpr(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createPredefinedExpr(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createIntegerLiteral(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createCharacterLiteral(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createFloatingLiteral(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createImaginaryLiteral(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createStringLiteral(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createParenExpr(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createUnaryOperator(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createSizeOfAlignOfExpr(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createArraySubscriptExpr(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createCallExpr(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createMemberExpr(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createCompoundLiteralExpr(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createImplicitCastExpr(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createCStyleCastExpr(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createBinaryOperator(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createCompoundAssignOperator(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createConditionalOperator(std::vector<var_t> &value);
	//std::shared_ptr<Stmt> createAddrLabelExpr(std::vector<var_t> &value);///gun extension
	//std::shared_ptr<Stmt> createStmtExpr(std::vector<var_t> &value);///gun extension
	//std::shared_ptr<Stmt> createTypesCompatibleExpr(std::vector<var_t> &value);///gun extension
	//std::shared_ptr<Stmt> createShuffleVectorExpr(std::vector<var_t> &value);///clang extension
	//std::shared_ptr<Stmt> createChooseExpr(std::vector<var_t> &value);///gun extension
	//std::shared_ptr<Stmt> createGNUNullExpr(std::vector<var_t> &value);///gun extension
	std::shared_ptr<Stmt> createVAArgExpr(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createInitListExpr(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createParenListExpr(std::vector<var_t> &value);

	/// @brief Do create types
	//std::shared_ptr<Type> createExtQualType(std::vector<var_t> &value);
//    void createQualifierSet(std::vector<var_t> &value);
	std::shared_ptr<QualType> createBuiltinType(std::vector<var_t> &value);//
	std::shared_ptr<QualType> createFixedWidthIntType(std::vector<var_t> &value);
	std::shared_ptr<QualType> createComplexType(std::vector<var_t> &value);
	std::shared_ptr<QualType> createPointerType(std::vector<var_t> &value);
	std::shared_ptr<QualType> createBlockPointerType(std::vector<var_t> &value);
	//std::shared_ptr<Type> createReferenceType(std::vector<var_t> &value);//
	//std::shared_ptr<Type> createLValueReferenceType(std::vector<var_t> &value);
	//std::shared_ptr<Type> createRValueReferenceType(std::vector<var_t> &value);
	//std::shared_ptr<Type> createMemberPointerType(std::vector<var_t> &value);
//    void createArrayType(std::vector<var_t> &value);      protected
	std::shared_ptr<QualType> createConstantArrayType(std::vector<var_t> &value);//
	std::shared_ptr<QualType> createConstantArrayWithExprType(std::vector<var_t> &value);
	std::shared_ptr<QualType> createConstantArrayWithoutExprType(std::vector<var_t> &value);//
	std::shared_ptr<QualType> createIncompleteArrayType(std::vector<var_t> &value);
	//std::shared_ptr<Type> createVariableArrayType(std::vector<var_t> &value);//
	//std::shared_ptr<Type> createDependentSizedArrayType(std::vector<var_t> &value);//
	//std::shared_ptr<Type> createVectorType(std::vector<var_t> &value);
	//std::shared_ptr<Type> createExtVectorType(std::vector<var_t> &value);
	std::shared_ptr<QualType> createFunctionNoProtoType(std::vector<var_t> &value);
	std::shared_ptr<QualType> createFunctionProtoType(std::vector<var_t> &value);
	std::shared_ptr<QualType> createTypedefType(std::vector<var_t> &value);
	std::shared_ptr<QualType> createRecordType(std::vector<var_t> &value);
	std::shared_ptr<QualType> createEnumType(std::vector<var_t> &value);
	//std::shared_ptr<Type> createTypeOfExprType(std::vector<var_t> &value);
	//std::shared_ptr<Type> createDependentTypeOfExprType(std::vector<var_t> &value);
	//std::shared_ptr<Type> createTypeOfType(std::vector<var_t> &value);
	//std::shared_ptr<Type> createDecltypeType(std::vector<var_t> &value);
	//std::shared_ptr<Type> createDependentDecltypeType(std::vector<var_t> &value);

	/// @brief The root of ast
	std::shared_ptr<Stmt> m_ASTRoot;
};

#endif // ASTCONTEXT_H
