#ifndef ASTCONTEXT_H
#define ASTCONTEXT_H

#include <queue>
#include <memory>
#include <variant>
#include <stack>
#include "AST/AST.hpp"

class ASTContext
{
public:
	enum Marker {
		Marker_1 = 0x01,
		Marker_2 = 0x02,
		Marker_3 = 0x04,
		Marker_4 = 0x08
	};
	typedef std::variant<
			unsigned,
			unsigned long,
			int,
			double,
			float,
			bool,
			char *,
			SourceLocation,
			QualType,
			UnaryOperator::Opcode,
			BinaryOperator::Opcode,
			PredefinedExpr::IdentType,
			BuiltinType::Kind,
			Type::TypeClass,
			CastExpr::CastKind,
			ArrayType::ArraySizeModifier,
			std::string,
			std::vector<std::shared_ptr<Stmt>>,
			std::vector<std::shared_ptr<Expr>>,
			std::shared_ptr<Expr>,
			std::shared_ptr<Stmt>,
			std::shared_ptr<QualType>,
			std::shared_ptr<NamedDecl>
	> var_t;
    /*template<auto type, typename... Args>
	std::shared_ptr<Stmt> create(Args... args){
        std::vector<var_t> value;
        (value.push_back(var_t{args}), ...);
        if(typeid (type)==typeid (Stmt::StmtClass)){
            return createStmt(static_cast<Stmt::StmtClass>(type), value);
        }
        else if(typeid (type)==typeid (Type::TypeClass)){
            createType(static_cast<Type::TypeClass>(type), value);
        }
        else {
            throw std::string("Unable to identify identifier!");
        }
    }*/
	template<typename... Args>
	std::shared_ptr<Stmt> createStmt(Stmt::StmtClass type, Args... args)
	{
		std::vector<var_t> value;
		(value.push_back(var_t{args}), ...);
		std::shared_ptr<Stmt> stmt;
		switch (type) {
			case Stmt::DeclStmtClass:				return createDeclStmt(value);
			case Stmt::NullStmtClass:				return createNullStmt(value);
			case Stmt::CompoundStmtClass:			return createCompoundStmt(value);
			case Stmt::CaseStmtClass:				return createCaseStmt(value);
			case Stmt::DefaultStmtClass:			return createDefaultStmt(value);
			case Stmt::LabelStmtClass:				return createLabelStmt(value);
			case Stmt::IfStmtClass:					return createIfStmt(value);
			case Stmt::SwitchStmtClass:				return createSwitchStmt(value);
			case Stmt::WhileStmtClass:				return createWhileStmt(value);
			case Stmt::DoStmtClass:					return createDoStmt(value);
			case Stmt::ForStmtClass:				return createForStmt(value);
			case Stmt::GotoStmtClass:				return createGotoStmt(value);
			case Stmt::IndirectGotoStmtClass:		return createIndirectGotoStmt(value);
			case Stmt::ContinueStmtClass:			return createContinueStmt(value);
			case Stmt::BreakStmtClass:				return createBreakStmt(value);
			case Stmt::ReturnStmtClass:				return createReturnStmt(value);
			case Expr::DeclRefExprClass:			return createDeclRefExpr(value);
			case Expr::PredefinedExprClass:			return createPredefinedExpr(value);
			case Expr::IntegerLiteralClass:			return createIntegerLiteral(value);
			case Expr::CharacterLiteralClass:		return createCharacterLiteral(value);
			case Expr::FloatingLiteralClass:		return createFloatingLiteral(value);
			case Expr::ImaginaryLiteralClass:		return createImaginaryLiteral(value);
			case Expr::StringLiteralClass:			return createStringLiteral(value);
			case Expr::ParenExprClass:				return createParenExpr(value);
			case Expr::UnaryOperatorClass:			return createUnaryOperator(value);
			case Expr::SizeOfAlignOfExprClass:		return createSizeOfAlignOfExpr(value);
			case Expr::ArraySubscriptExprClass:		return createArraySubscriptExpr(value);
			case Expr::CallExprClass:				return createCallExpr(value);
			case Expr::MemberExprClass:				return createMemberExpr(value);
			case Expr::CompoundLiteralExprClass:	return createCompoundLiteralExpr(value);
			case Expr::ImplicitCastExprClass:		return createImplicitCastExpr(value);
			case Expr::CStyleCastExprClass:			return createCStyleCastExpr(value);
			case Expr::BinaryOperatorClass:			return createBinaryOperator(value);
			case Expr::CompoundAssignOperatorClass:	return createCompoundAssignOperator(value);
			case Expr::ConditionalOperatorClass:	return createConditionalOperator(value);
				/*case Expr::AddrLabelExprClass:
					createAddrLabelExpr(value);
					break;*/
				/*case Expr::StmtExprClass:
					createStmtExpr(value);
					break;*/
				/*case Expr::TypesCompatibleExprClass:
					createTypesCompatibleExpr(value);
					break;*/
				/*case Expr::ShuffleVectorExprClass:
					createShuffleVectorExpr(value);
					break;*/
				/*case Expr::ChooseExprClass:
					createChooseExpr(value);
					break;*/
				/*case Expr::GNUNullExprClass:
					createGNUNullExpr(value);
					break;*/
			case Expr::VAArgExprClass:				return createVAArgExpr(value);
			case Expr::InitListExprClass:			return createInitListExpr(value);
			case Expr::ParenListExprClass:			return createParenListExpr(value);
		}

		// silent no return statement warnings
		return nullptr;
	}

	template<typename... Args>
	std::shared_ptr<Type> createType(Type::TypeClass type, Args... args)
	{
		std::vector<var_t> value;
		(value.push_back(var_t{args}), ...);

		switch (type) {
			case Type::ExtQual:break;
			case Type::Builtin:					return createBuiltinType(value);
			case Type::FixedWidthInt:break;
			case Type::Complex:break;
			case Type::Pointer:					return createPointerType(value);
			case Type::BlockPointer:break;
			case Type::LValueReference:break;
			case Type::RValueReference:break;
			case Type::MemberPointer:break;
			case Type::ConstantArray:break;
			case Type::ConstantArrayWithExpr:break;
			case Type::ConstantArrayWithoutExpr:break;
			case Type::IncompleteArray:			return createIncompleteArrayType(value);
			case Type::VariableArray:break;
			case Type::DependentSizedArray:break;
			case Type::DependentSizedExtVector:break;
			case Type::Vector:break;
			case Type::ExtVector:break;
			case Type::FunctionProto:break;
			case Type::FunctionNoProto:break;
			case Type::Typedef:break;
			case Type::TypeOfExpr:break;
			case Type::TypeOf:break;
			case Type::Decltype:break;
			case Type::Record:break;
			case Type::Enum:break;
			case Type::TemplateTypeParm:break;
			case Type::TemplateSpecialization:break;
			case Type::QualifiedName:break;
			case Type::Typename:break;
			case Type::ObjCInterface:break;
			case Type::ObjCObjectPointer:break;
		}

		// silent no return statement warnings
		return nullptr;
	}

	void updateASTRoot(std::shared_ptr<Stmt> root);

#ifdef ENV_TEST
    std::weak_ptr<Stmt> getRoot() const;
    void cleanAST();
#endif

    /// @brief Not allow copy
    ASTContext &operator =(ASTContext &) = delete;

private:
	std::shared_ptr<Stmt> createDeclStmt(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createNullStmt(std::vector<var_t> &value);//done
	std::shared_ptr<Stmt> createCompoundStmt(std::vector<var_t> &value);//done
	std::shared_ptr<Stmt> createCaseStmt(std::vector<var_t> &value);//done
	std::shared_ptr<Stmt> createDefaultStmt(std::vector<var_t> &value);//done
	std::shared_ptr<Stmt> createLabelStmt(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createIfStmt(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createSwitchStmt(std::vector<var_t> &value);//done
	std::shared_ptr<Stmt> createWhileStmt(std::vector<var_t> &value);//done
	std::shared_ptr<Stmt> createDoStmt(std::vector<var_t> &value);//done
	std::shared_ptr<Stmt> createForStmt(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createGotoStmt(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createIndirectGotoStmt(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createContinueStmt(std::vector<var_t> &value);//done
	std::shared_ptr<Stmt> createBreakStmt(std::vector<var_t> &value);//done
	std::shared_ptr<Stmt> createReturnStmt(std::vector<var_t> &value);//done

	std::shared_ptr<Stmt> createDeclRefExpr(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createPredefinedExpr(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createIntegerLiteral(std::vector<var_t> &value);//done
	std::shared_ptr<Stmt> createCharacterLiteral(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createFloatingLiteral(std::vector<var_t> &value);//done
	std::shared_ptr<Stmt> createImaginaryLiteral(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createStringLiteral(std::vector<var_t> &value);//done
	std::shared_ptr<Stmt> createParenExpr(std::vector<var_t> &value);//done
	std::shared_ptr<Stmt> createUnaryOperator(std::vector<var_t> &value);//done
	std::shared_ptr<Stmt> createSizeOfAlignOfExpr(std::vector<var_t> &value);//done
	std::shared_ptr<Stmt> createArraySubscriptExpr(std::vector<var_t> &value);//done
	std::shared_ptr<Stmt> createCallExpr(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createMemberExpr(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createCompoundLiteralExpr(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createImplicitCastExpr(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createCStyleCastExpr(std::vector<var_t> &value);//done
	std::shared_ptr<Stmt> createBinaryOperator(std::vector<var_t> &value);//done
	std::shared_ptr<Stmt> createCompoundAssignOperator(std::vector<var_t> &value);//done
	std::shared_ptr<Stmt> createConditionalOperator(std::vector<var_t> &value);//done
	//std::shared_ptr<Stmt> createAddrLabelExpr(std::vector<var_t> &value);///gun extension
	//std::shared_ptr<Stmt> createStmtExpr(std::vector<var_t> &value);///gun extension
	//std::shared_ptr<Stmt> createTypesCompatibleExpr(std::vector<var_t> &value);///gun extension
	//std::shared_ptr<Stmt> createShuffleVectorExpr(std::vector<var_t> &value);///clang extension
	//std::shared_ptr<Stmt> createChooseExpr(std::vector<var_t> &value);///gun extension
	//std::shared_ptr<Stmt> createGNUNullExpr(std::vector<var_t> &value);///gun extension
	std::shared_ptr<Stmt> createVAArgExpr(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createInitListExpr(std::vector<var_t> &value);
	std::shared_ptr<Stmt> createParenListExpr(std::vector<var_t> &value);//done

	//std::shared_ptr<Type> createExtQualType(std::vector<var_t> &value);
//    void createQualifierSet(std::vector<var_t> &value);
	std::shared_ptr<Type> createBuiltinType(std::vector<var_t> &value);//
	std::shared_ptr<Type> createFixedWidthIntType(std::vector<var_t> &value);
	std::shared_ptr<Type> createComplexType(std::vector<var_t> &value);
	std::shared_ptr<Type> createPointerType(std::vector<var_t> &value);
	std::shared_ptr<Type> createBlockPointerType(std::vector<var_t> &value);
	//std::shared_ptr<Type> createReferenceType(std::vector<var_t> &value);//
	//std::shared_ptr<Type> createLValueReferenceType(std::vector<var_t> &value);
	//std::shared_ptr<Type> createRValueReferenceType(std::vector<var_t> &value);
	//std::shared_ptr<Type> createMemberPointerType(std::vector<var_t> &value);
//    void createArrayType(std::vector<var_t> &value);      protected
	std::shared_ptr<Type> createConstantArrayType(std::vector<var_t> &value);//
	std::shared_ptr<Type> createConstantArrayWithExprType(std::vector<var_t> &value);//
	std::shared_ptr<Type> createConstantArrayWithoutExprType(std::vector<var_t> &value);//
	std::shared_ptr<Type> createIncompleteArrayType(std::vector<var_t> &value);//
	//std::shared_ptr<Type> createVariableArrayType(std::vector<var_t> &value);//
	//std::shared_ptr<Type> createDependentSizedArrayType(std::vector<var_t> &value);//


	//std::shared_ptr<Type> createVectorType(std::vector<var_t> &value);
	//std::shared_ptr<Type> createExtVectorType(std::vector<var_t> &value);

	std::shared_ptr<Type> createFunctionNoProtoType(std::vector<var_t> &value);
	std::shared_ptr<Type> createFunctionProtoType(std::vector<var_t> &value);
	//std::shared_ptr<Type> createTypeOfExprType(std::vector<var_t> &value);
	//std::shared_ptr<Type> createDependentTypeOfExprType(std::vector<var_t> &value);
	//std::shared_ptr<Type> createTypeOfType(std::vector<var_t> &value);
	//std::shared_ptr<Type> createDecltypeType(std::vector<var_t> &value);
	//std::shared_ptr<Type> createDependentDecltypeType(std::vector<var_t> &value);

	std::shared_ptr<Stmt> m_ASTRoot;
	/*std::stack<std::shared_ptr<Stmt>> queue;
	std::stack<std::shared_ptr<Type>> queue_type;

	template<typename T=Stmt>
	std::shared_ptr<T> pop_stmt(){
		if(queue.empty())
			throw std::range_error("Queue empty");
		std::shared_ptr<Stmt> ptr=queue.top();
		queue.pop();
		return std::dynamic_pointer_cast<T>(ptr);
	}
	template<typename T=Type>
	std::shared_ptr<T> pop_type(){
		if(queue_type.empty())
			return nullptr;
		std::shared_ptr<Type> ptr=queue_type.top();
		queue_type.pop();
		return std::dynamic_pointer_cast<T>(ptr);
	}*/
};

#endif // ASTCONTEXT_H
