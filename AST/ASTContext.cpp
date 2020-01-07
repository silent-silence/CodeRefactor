#include "ASTContext.h"
#include "Expr.h"
#include "Decl.h"

using std::get;							using std::string;
using std::holds_alternative;			using std::list;
using std::vector;						using std::shared_ptr;
using std::make_shared;					using std::dynamic_pointer_cast;
using std::weak_ptr;

void ASTContext::updateASTRoot(std::shared_ptr<Stmt> root)
{
	m_ASTRoot = root;
}

std::weak_ptr<Stmt> ASTContext::getRoot() const
{
	return m_ASTRoot;
}

void ASTContext::cleanAST()
{
	m_ASTRoot = std::shared_ptr<Stmt>();
}


std::shared_ptr<Stmt> ASTContext::createDeclStmt(std::vector<var_t> &value)
{
	return make_shared<DeclStmt>(
					get<shared_ptr<DeclGroupRef>>(value[0]),
					get<SourceLocation>(value[1]),
					get<SourceLocation>(value[2])
	);
}

std::shared_ptr<Stmt> ASTContext::createNullStmt(std::vector<ASTContext::var_t> &value)
{
	return make_shared<NullStmt>(
					get<SourceLocation>(value[0])
	);
}

std::shared_ptr<Stmt> ASTContext::createCompoundStmt(std::vector<ASTContext::var_t> &value)
{
	return make_shared<CompoundStmt>(
					get<list<shared_ptr<Stmt>>>(value[0]),
					get<SourceLocation>(value[1]),
					get<SourceLocation>(value[2])
	);
}

std::shared_ptr<Stmt> ASTContext::createCaseStmt(std::vector<ASTContext::var_t> &value)
{
	// TODO: RHS not initialized
	return make_shared<CaseStmt>(
					get<shared_ptr<Expr>>(value[0]),
					nullptr,
					get<SourceLocation>(value[1]),
					get<SourceLocation>(value[2]),
					get<SourceLocation>(value[3])
	);
}

std::shared_ptr<Stmt> ASTContext::createDefaultStmt(std::vector<ASTContext::var_t> &value)
{
	return make_shared<DefaultStmt>(
					get<SourceLocation>(value[0]),
					get<SourceLocation>(value[1]),
					nullptr
	);
}

std::shared_ptr<Stmt> ASTContext::createLabelStmt(std::vector<ASTContext::var_t> &value)
{
	return make_shared<LabelStmt>(
					get<SourceLocation>(value[0]),
					get<shared_ptr<Stmt>>(value[1]),
					get<weak_ptr<Decl>>(value[2])
	);
}

std::shared_ptr<Stmt> ASTContext::createIfStmt(std::vector<ASTContext::var_t> &value)
{
	if (value.size() == 3)
		return make_shared<IfStmt>(
				get<SourceLocation>(value[0]),
				get<shared_ptr<Expr>>(value[1]),
				get<shared_ptr<Stmt>>(value[2])
		);
	else
		return make_shared<IfStmt>(
				get<SourceLocation>(value[0]),
				get<shared_ptr<Expr>>(value[1]),
				get<shared_ptr<Stmt>>(value[2]),
				get<SourceLocation>(value[3]),
				get<shared_ptr<Stmt>>(value[4])
		);
}

std::shared_ptr<Stmt> ASTContext::createSwitchStmt(std::vector<ASTContext::var_t> &value)
{
	auto switchStmt = make_shared<SwitchStmt>(
			get<SourceLocation>(value[0]),
			get<shared_ptr<Expr>>(value[1])
	);
    auto body = get<shared_ptr<Stmt>>(value[2]);
    if(CompoundStmt::classof(body)){
        auto stmts = std::dynamic_pointer_cast<CompoundStmt>(body);
        shared_ptr<CaseStmt> stmt=nullptr;
        for(auto i = stmts->body_begin(); i!=stmts->body_end(); ++i){
            if(CaseStmt::classof(*i)){
                auto ptr = std::dynamic_pointer_cast<CaseStmt>(*i);
                if(stmt != nullptr){
                    stmt->setNextSwitchCase(ptr);
                }
                else{
                    switchStmt->setSwitchCaseList(ptr);
                }
                stmt = ptr;
            }
        }
    }
    switchStmt->setBody(body);
	return switchStmt;
}

std::shared_ptr<Stmt> ASTContext::createWhileStmt(std::vector<ASTContext::var_t> &value)
{
	return make_shared<WhileStmt>(
					get<shared_ptr<Expr>>(value[0]),
					get<shared_ptr<Stmt>>(value[1]),
					get<SourceLocation>(value[2])
	);
}

std::shared_ptr<Stmt> ASTContext::createDoStmt(std::vector<ASTContext::var_t> &value)
{
	return make_shared<DoStmt>(
					get<shared_ptr<Stmt>>(value[0]),
					get<shared_ptr<Expr>>(value[1]),
					get<SourceLocation>(value[2]),
					get<SourceLocation>(value[3]),
					get<SourceLocation>(value[4])
	);
}

std::shared_ptr<Stmt> ASTContext::createForStmt(std::vector<ASTContext::var_t> &value)
{
	bool haveInc = get<bool>(value[0]);
	return make_shared<ForStmt>(
					get<shared_ptr<Stmt>>(value[1]),
					get<shared_ptr<Stmt>>(value[2]),
					haveInc ? get<shared_ptr<Expr>>(value[7]) : nullptr,
					get<shared_ptr<Stmt>>(value[3]),
					get<SourceLocation>(value[4]),
					get<SourceLocation>(value[5]),
					get<SourceLocation>(value[6])
	);
}

std::shared_ptr<Stmt> ASTContext::createGotoStmt(std::vector<ASTContext::var_t> &value)
{
	return make_shared<GotoStmt>(
			dynamic_pointer_cast<LabelStmt>(get<shared_ptr<Stmt>>(value[0])),
			get<SourceLocation>(value[1]),
			get<SourceLocation>(value[2])
	);
}

std::shared_ptr<Stmt> ASTContext::createIndirectGotoStmt(std::vector<ASTContext::var_t> &value)
{
	// TODO
	/*queue.push(
			make_shared<IndirectGotoStmt>(
					get<SourceLocation>(value[0]),
					get<SourceLocation>(value[1]),
					pop_stmt<Expr>()));*/
	return nullptr;
}

std::shared_ptr<Stmt> ASTContext::createContinueStmt(std::vector<ASTContext::var_t> &value)
{
	return make_shared<ContinueStmt>(
					get<SourceLocation>(value[0])
	);
}

std::shared_ptr<Stmt> ASTContext::createBreakStmt(std::vector<ASTContext::var_t> &value)
{
	return make_shared<BreakStmt>(
					get<SourceLocation>(value[0])
	);
}

std::shared_ptr<Stmt> ASTContext::createReturnStmt(std::vector<ASTContext::var_t> &value)
{
	return make_shared<ReturnStmt>(
					get<SourceLocation>(value[0]),
					get<shared_ptr<Expr>>(value[1])
            );
}

std::shared_ptr<Stmt> ASTContext::createCommentStmt(std::vector<ASTContext::var_t> &value)
{
    return make_shared<CommentStmt>(
                get<string>(value[0]),
                    get<SourceLocation>(value[1])
            );
}

std::shared_ptr<Stmt> ASTContext::createDeclRefExpr(std::vector<ASTContext::var_t> &value)
{
	return make_shared<DeclRefExpr>(
			get<shared_ptr<NamedDecl>>(value[0]),
			get<shared_ptr<QualType>>(value[1]),
			get<SourceLocation>(value[2])
	);
}

std::shared_ptr<Stmt> ASTContext::createPredefinedExpr(std::vector<ASTContext::var_t> &value)
{
	// TODO
	/*queue.push(
			make_shared<PredefinedExpr>(
					get<SourceLocation>(value[0]),
					get<QualType>(value[1]),
					get<PredefinedExpr::IdentType>(value[2])));*/
	return nullptr;
}

std::shared_ptr<Stmt> ASTContext::createIntegerLiteral(std::vector<ASTContext::var_t> &value)
{
	auto type = BuiltinType::creator(BuiltinType::Int, QualType::None);
	return make_shared<IntegerLiteral>(
					get<int>(value[0]),
					type,
					get<SourceLocation>(value[1])
	);
}

std::shared_ptr<Stmt> ASTContext::createCharacterLiteral(std::vector<ASTContext::var_t> &value)
{
	auto type = BuiltinType::creator(BuiltinType::SChar, QualType::None);
	return make_shared<CharacterLiteral>(
					get<unsigned>(value[0]),
					get<bool>(value[1]),
					type,
					get<SourceLocation>(value[2])
	);
}

std::shared_ptr<Stmt> ASTContext::createFloatingLiteral(std::vector<ASTContext::var_t> &value)
{
	auto type = BuiltinType::creator(BuiltinType::Double, QualType::None);
	return make_shared<FloatingLiteral>(
					get<float>(value[0]),
					get<bool>(value[1]),
					type,
					get<SourceLocation>(value[2])
	);
}

std::shared_ptr<Stmt> ASTContext::createImaginaryLiteral(std::vector<ASTContext::var_t> &value)
{
	// TODO
	/*queue.push(
			make_shared<ImaginaryLiteral>(
					pop_stmt<Expr>(),
					get<QualType>(value[0])));*/
	return nullptr;
}

std::shared_ptr<Stmt> ASTContext::createStringLiteral(std::vector<ASTContext::var_t> &value)
{
	// TODO: make default string type
	auto type = BuiltinType::creator(BuiltinType::SChar, QualType::None);
	auto pType = createType(Type::TypeClass::Pointer, type, QualType::Const);
	return StringLiteral::Create(
			get<string>(value[0]),
			get<unsigned long>(value[1]),
			get<bool>(value[2]),
			pType,
			get<SourceLocation>(value[3]),
			get<unsigned long>(value[4])
	);
}

std::shared_ptr<Stmt> ASTContext::createParenExpr(std::vector<ASTContext::var_t> &value)
{
	return make_shared<ParenExpr>(
					get<SourceLocation>(value[0]),
					get<SourceLocation>(value[1]),
					get<shared_ptr<Expr>>(value[2])
	);
}

std::shared_ptr<Stmt> ASTContext::createUnaryOperator(std::vector<ASTContext::var_t> &value)
{
	auto expr = get<shared_ptr<Expr>>(value[0]);
	auto type = expr->getType().lock();
	return make_shared<UnaryOperator>(
					expr,
					get<UnaryOperator::Opcode>(value[1]),
					type,
					get<SourceLocation>(value[2])
	);
}

std::shared_ptr<Stmt> ASTContext::createSizeOfAlignOfExpr(std::vector<ASTContext::var_t> &value)
{
	//TODO: set return type
	auto retType = BuiltinType::creator(BuiltinType::UInt, QualType::None);
	if(holds_alternative<shared_ptr<QualType>>(value[2]))
	{
		return make_shared<SizeOfAlignOfExpr>(
						true,
						get<shared_ptr<QualType>>(value[2]),
						retType,
						get<SourceLocation>(value[0]),
						get<SourceLocation>(value[1])
		);
	}
	else
	{
		return make_shared<SizeOfAlignOfExpr>(
						true,
						get<shared_ptr<Expr>>(value[2]),
						retType,
						get<SourceLocation>(value[0]),
						get<SourceLocation>(value[1])
		);
	}
}

std::shared_ptr<Stmt> ASTContext::createArraySubscriptExpr(std::vector<ASTContext::var_t> &value)
{
	auto lhs = get<shared_ptr<Expr>>(value[0]);
	shared_ptr<QualType> arrayType = lhs->getType().lock();
	shared_ptr<Type> arrayElementBaseType = arrayType->getTypePtr();
	shared_ptr<PointerType> arrayElementType = dynamic_pointer_cast<PointerType>(arrayElementBaseType);
	return make_shared<ArraySubscriptExpr>(
					get<shared_ptr<Expr>>(value[0]),
					get<shared_ptr<Expr>>(value[1]),
					arrayType,
					get<SourceLocation>(value[2])
	);
}

std::shared_ptr<Stmt> ASTContext::createCallExpr(std::vector<ASTContext::var_t> &value)
{
	auto fn = get<shared_ptr<Expr>>(value[0]);
	auto retType = fn->getType().lock();
	auto args = get<list<shared_ptr<Expr>>>(value[1]);
	return make_shared<CallExpr>(
					get<shared_ptr<Expr>>(value[0]),
					args,
					args.size(),
					retType,
					get<SourceLocation>(value[2])
	);
}

std::shared_ptr<Stmt> ASTContext::createMemberExpr(std::vector<ASTContext::var_t> &value)
{
	// TODO: Get type from symbol table
	auto expr = get<shared_ptr<Expr>>(value[0]);
	auto memberType = expr->getType().lock();
	return make_shared<MemberExpr>(
					expr,
					get<bool>(value[1]),
					get<shared_ptr<NamedDecl>>(value[2]),
					get<SourceLocation>(value[3]),
					memberType
	);
}

std::shared_ptr<Stmt> ASTContext::createCompoundLiteralExpr(std::vector<ASTContext::var_t> &value)
{
	// TODO
	/*return make_shared<CompoundLiteralExpr>(
					get<SourceLocation>(value[0]),
					get<QualType>(value[1]),
					pop_stmt<Expr>(),
					get<bool>(value[2])
	);*/
	return nullptr;
}

std::shared_ptr<Stmt> ASTContext::createImplicitCastExpr(std::vector<ASTContext::var_t> &value)
{
	// TODO
	/*return make_shared<ImplicitCastExpr>(
					get<QualType>(value[0]),
					get<CastExpr::CastKind>(value[1]),
					pop_stmt<Expr>(),
					get<bool>(value[2])
	);*/
	return nullptr;
}

std::shared_ptr<Stmt> ASTContext::createCStyleCastExpr(std::vector<ASTContext::var_t> &value)
{
	auto expr = get<shared_ptr<Expr>>(value[1]);
	auto exprType = expr->getType().lock();
	return make_shared<CStyleCastExpr>(
					exprType,
					get<CastExpr::CastKind>(value[0]),
					expr,
					get<shared_ptr<QualType>>(value[2]),
					get<SourceLocation>(value[3]),
					get<SourceLocation>(value[4])
	);
}

std::shared_ptr<Stmt> ASTContext::createBinaryOperator(std::vector<ASTContext::var_t> &value)
{
	auto lhs = get<shared_ptr<Expr>>(value[0]);
	auto rhs = get<shared_ptr<Expr>>(value[1]);
	// TODO Compare and get operation type
	auto exprType = lhs->getType().lock();
	return make_shared<BinaryOperator>(
					lhs,
					rhs,
					get<BinaryOperator::Opcode>(value[2]),
					exprType,
					get<SourceLocation>(value[3])
	);
}

std::shared_ptr<Stmt> ASTContext::createCompoundAssignOperator(std::vector<ASTContext::var_t> &value)
{
	// TODO Get types
	auto lhs = get<shared_ptr<Expr>>(value[0]);
	auto rhs = get<shared_ptr<Expr>>(value[1]);

	auto resultType = lhs->getType().lock();
	auto comLhsType = lhs->getType().lock();
	// TODO Compare and get operation type
	auto comResultType = lhs->getType().lock();
	return make_shared<CompoundAssignOperator>(
					lhs,
					rhs,
					get<CompoundAssignOperator::Opcode>(value[2]),
					resultType,
					comLhsType,
					comResultType,
					get<SourceLocation>(value[3])
	);
}

std::shared_ptr<Stmt> ASTContext::createConditionalOperator(std::vector<ASTContext::var_t> &value)
{
	// TODO Compare and get type here
	auto lhs = get<shared_ptr<Expr>>(value[1]);
	auto rhs = get<shared_ptr<Expr>>(value[2]);
	auto type = lhs->getType().lock();
	return make_shared<ConditionalOperator>(
					get<shared_ptr<Expr>>(value[0]),
					lhs,
					rhs,
					type
	);
}

/*std::shared_ptr<Stmt> ASTContext::createAddrLabelExpr(std::vector<ASTContext::var_t> &value)
{
	queue.push(
			make_shared<AddrLabelExpr>(
					get<SourceLocation>(value[0]),
					get<SourceLocation>(value[1]),
					pop_stmt<LabelStmt>(),
					get<QualType>(value[2])));
}*/

/*std::shared_ptr<Stmt> ASTContext::createStmtExpr(std::vector<ASTContext::var_t> &value)
{
	queue.push(
			make_shared<StmtExpr>(
					pop_stmt<CompoundStmt>(),
					get<QualType>(value[0]),
					get<SourceLocation>(value[1]),
					get<SourceLocation>(value[2])));
}*/

/*std::shared_ptr<Stmt> ASTContext::createTypesCompatibleExpr(std::vector<ASTContext::var_t> &value)
{
	queue.push(
			make_shared<TypesCompatibleExpr>(
					get<QualType>(value[0]),
					get<SourceLocation>(value[1]),
					get<QualType>(value[2]),
					get<QualType>(value[3]),
					get<SourceLocation>(value[4])));
}*/

/*std::shared_ptr<Stmt> ASTContext::createShuffleVectorExpr(std::vector<ASTContext::var_t> &value)
{
	vector<shared_ptr<Expr>> exprs;
	for(auto i=get<unsigned>(value[0]);i!=0;i++){
		exprs.push_back(pop_stmt<Expr>());
	}
	queue.push(
			make_shared<ShuffleVectorExpr>(
					exprs,
					get<unsigned>(value[1]),
					get<QualType>(value[2]),
					get<SourceLocation>(value[3]),
					get<SourceLocation>(value[4])));
}*/

/*std::shared_ptr<Stmt> ASTContext::createChooseExpr(std::vector<ASTContext::var_t> &value)
{
	queue.push(
			make_shared<ChooseExpr>(
					get<SourceLocation>(value[0]),
					pop_stmt<Expr>(),
					pop_stmt<Expr>(),
					pop_stmt<Expr>(),
					get<QualType>(value[1]),
					get<SourceLocation>(value[2])));
}*/

/*std::shared_ptr<Stmt> ASTContext::createGNUNullExpr(std::vector<ASTContext::var_t> &value)
{
	queue.push(
			make_shared<GNUNullExpr>(
					get<QualType>(value[0]),
					get<SourceLocation>(value[1])));
}*/

std::shared_ptr<Stmt> ASTContext::createVAArgExpr(std::vector<ASTContext::var_t> &value)
{
	// TODO
	/*queue.push(
			make_shared<VAArgExpr>(
					get<SourceLocation>(value[0]),
					pop_stmt<Expr>(),
					get<QualType>(value[1]),
					get<SourceLocation>(value[2])));*/
	return nullptr;
}

std::shared_ptr<Stmt> ASTContext::createInitListExpr(std::vector<ASTContext::var_t> &value)
{
	/*vector<shared_ptr<Expr>> exprs;
	for(auto i=get<unsigned>(value[0]);i!=0;i++){
		exprs.push_back(pop_stmt<Expr>());
	}
	queue.push(
			make_shared<InitListExpr>(
					get<SourceLocation>(value[0]),
					exprs,
					get<unsigned>(value[1]),
					get<SourceLocation>(value[2])));*/
	return nullptr;
}

std::shared_ptr<Stmt> ASTContext::createParenListExpr(std::vector<ASTContext::var_t> &value)
{
	/*vector<shared_ptr<Expr>> exprs;
	for(auto i=get<unsigned>(value[0]);i!=0;i++){
		exprs.push_back(pop_stmt<Expr>());
	}
	queue.push(
			make_shared<ParenListExpr>(
					get<SourceLocation>(value[0]),
					exprs,
					get<unsigned>(value[1]),
					get<SourceLocation>(value[2])));*/
	return nullptr;
}

/*void ASTContext::createExtQualType(std::vector<ASTContext::var_t> &value)
{
	queue_type.push(
			make_shared<ExtQualType>(
					pop_type(),
					get<QualType>(value[0]),
					get<unsigned>(value[1]),
					static_cast<QualType::GCAttrTypes>(get<int>(value[2]))));
}*/

std::shared_ptr<QualType> ASTContext::createBuiltinType(std::vector<ASTContext::var_t> &value)
{
	return BuiltinType::creator(
			get<BuiltinType::Kind>(value[0]),
			get<QualType::TQ>(value[1])
	);
}

/*std::shared_ptr<Type> ASTContext::createFixedWidthIntType(std::vector<ASTContext::var_t> &value)
{
	return FixedWidthIntType(
					get<unsigned>(value[0]),
					get<bool>(value[1])
	);
}*/

/*std::shared_ptr<Type> ASTContext::createComplexType(std::vector<ASTContext::var_t> &value)
{
	return ComplexType(
					get<QualType>(value[0]),
					get<QualType>(value[1])
	);
}*/

std::shared_ptr<QualType> ASTContext::createPointerType(std::vector<ASTContext::var_t> &value)
{
	shared_ptr<QualType> qual = make_shared<QualType>();
	std::shared_ptr<Type> pointer = PointerType::creator(
					get<shared_ptr<QualType>>(value[0]),
					qual
	);
	qual->Value.first = pointer;
	qual->Value.second = get<QualType::TQ>(value[1]);
	return qual;
}

/*std::shared_ptr<Type> ASTContext::createBlockPointerType(std::vector<ASTContext::var_t> &value)
{
	return BlockPointerType::creator(
					get<QualType>(value[0]),
					get<QualType>(value[1])
	);
}*/

/*void ASTContext::createReferenceType(std::vector<ASTContext::var_t> &value)
{
	queue_type.push(
			make_shared<ReferenceType>(
					get<Type::TypeClass>(value[0]),
					get<QualType>(value[1]),
					get<QualType>(value[2])));
}*/

/*void ASTContext::createLValueReferenceType(std::vector<ASTContext::var_t> &value)
{
	queue_type.push(
			make_shared<LValueReferenceType>(
					get<QualType>(value[0]),
					get<QualType>(value[1])));
}*/

/*void ASTContext::createRValueReferenceType(std::vector<ASTContext::var_t> &value)
{
	queue_type.push(
			make_shared<RValueReferenceType>(
					get<QualType>(value[0]),
					get<QualType>(value[1])));
}*/

/*void ASTContext::createMemberPointerType(std::vector<ASTContext::var_t> &value)
{
	queue_type.push(
			make_shared<MemberPointerType>(
					get<QualType>(value[0]),
					pop_type(),
					get<QualType>(value[1])));
}*/

/*std::shared_ptr<QualType> ASTContext::createConstantArrayType(std::vector<ASTContext::var_t> &value)
{
	return ConstantArrayType::creator(
					get<shared_ptr<QualType>>(value[0]),
					get<shared_ptr<QualType>>(value[1]),
					get<int>(value[2]),
					get<ArrayType::ArraySizeModifier>(value[3]),
					get<unsigned>(value[4])
	);
}*/

std::shared_ptr<QualType> ASTContext::createConstantArrayWithExprType(std::vector<ASTContext::var_t> &value)
{
	auto qualifier = make_shared<QualType>();
	auto type = ConstantArrayWithExprType::creator(
			get<shared_ptr<QualType>>(value[0]),
			qualifier,
			get<shared_ptr<Expr>>(value[1]),
			get<ArrayType::ArraySizeModifier>(value[2]),
			get<int>(value[3])
	);
	qualifier->Value.first = type;
	return qualifier;
}

/*std::shared_ptr<Type> ASTContext::createConstantArrayWithoutExprType(std::vector<ASTContext::var_t> &value)
{
	return ConstantArrayWithoutExprType(
					get<QualType>(value[0]),
					get<QualType>(value[1]),
					get<int>(value[2]),
					get<ArrayType::ArraySizeModifier>(value[3]),
					get<unsigned>(value[4])
	);
}*/

std::shared_ptr<QualType> ASTContext::createIncompleteArrayType(std::vector<ASTContext::var_t> &value)
{
	auto qualifier = make_shared<QualType>();
	auto type = IncompleteArrayType::creator(
					get<shared_ptr<QualType>>(value[0]),
					qualifier,
					get<ArrayType::ArraySizeModifier>(value[1]),
					get<int>(value[2])
	);
	qualifier->Value.first = type;
	return qualifier;
}

/*void ASTContext::createVariableArrayType(std::vector<ASTContext::var_t> &value)
{
	queue_type.push(
			make_shared<VariableArrayType>(
					get<QualType>(value[0]),
					get<QualType>(value[1]),
					pop_stmt<Expr>(),
					get<ArrayType::ArraySizeModifier>(value[2]),
					get<unsigned>(value[3])));
}*/

/*void ASTContext::createDependentSizedArrayType(std::vector<ASTContext::var_t> &value)
{
	queue_type.push(
			make_shared<DependentSizedArrayType>(
					get<QualType>(value[0]),
					get<QualType>(value[1]),
					pop_stmt<Expr>(),
					get<ArrayType::ArraySizeModifier>(value[2]),
					get<unsigned>(value[3])));
}*/

/*void ASTContext::createVectorType(std::vector<ASTContext::var_t> &value)
{
	queue_type.push(
			make_shared<VectorType>(
					get<QualType>(value[0]),
					get<unsigned>(value[1]),
					get<QualType>(value[2])));
}*/

/*void ASTContext::createExtVectorType(std::vector<ASTContext::var_t> &value)
{
	queue_type.push(
			make_shared<ExtVectorType>(
					get<QualType>(value[0]),
					get<unsigned>(value[1]),
					get<QualType>(value[2])));
}*/

std::shared_ptr<QualType> ASTContext::createFunctionNoProtoType(std::vector<ASTContext::var_t> &value)
{
	auto qualifier = make_shared<QualType>();
	auto func = FunctionNoProtoType::creator(
					get<shared_ptr<QualType>>(value[0]),
					qualifier,
					get<bool>(value[1])
	);
	qualifier->Value.first = func;
	return qualifier;
}

std::shared_ptr<QualType> ASTContext::createTypedefType(std::vector<var_t> &value)
{
	shared_ptr<QualType> qualifier = make_shared<QualType>();
	auto type = TypedefType::creator(
			qualifier,
			get<shared_ptr<TypedefDecl>>(value[0]),
			get<shared_ptr<QualType>>(value[1])
	);
	qualifier->Value.first = type;
	return qualifier;
}

std::shared_ptr<QualType> ASTContext::createRecordType(std::vector<var_t> &value)
{
	shared_ptr<QualType> qualifier = make_shared<QualType>();
	auto type = RecordType::creator(get<shared_ptr<RecordDecl>>(value[0]), qualifier);
	qualifier->Value.first = type;
	return qualifier;
}

std::shared_ptr<QualType> ASTContext::createEnumType(std::vector<var_t> &value)
{
	shared_ptr<QualType> qualifier = make_shared<QualType>();
	auto type = make_shared<EnumType>(
			get<shared_ptr<EnumDecl>>(value[0]), qualifier
	);
	qualifier->Value.first = type;
	return qualifier;
}

std::shared_ptr<QualType> ASTContext::createFunctionProtoType(std::vector<ASTContext::var_t> &value)
{
	shared_ptr<QualType> qualifier = make_shared<QualType>();
	auto args = get<vector<shared_ptr<QualType>>>(value[1]);
	auto type = FunctionProtoType::creator(
			get<shared_ptr<QualType>>(value[0]),
			args,
			args.size(),
			false,
			0, false,
			false,
			vector<QualType>(),
			0,
			qualifier,
			false
	);
	qualifier->Value.first = type;
	return qualifier;
}

/*void ASTContext::createTypeOfExprType(std::vector<ASTContext::var_t> &value)
{
	queue_type.push(
			make_shared<TypeOfExprType>(
					pop_stmt<Expr>(),
					get<QualType>(value[0])));
}*/

/*void ASTContext::createDependentTypeOfExprType(std::vector<ASTContext::var_t> &value)
{
	queue_type.push(
			make_shared<DependentTypeOfExprType>(
					pop_stmt<Expr>()));
}*/

/*void ASTContext::createTypeOfType(std::vector<ASTContext::var_t> &value)
{
	queue_type.push(
			make_shared<TypeOfType>(
					get<QualType>(value[0]),
					get<QualType>(value[1])));
}*/

/*void ASTContext::createDecltypeType(std::vector<ASTContext::var_t> &value)
{
	queue_type.push(
			make_shared<DecltypeType>(
					pop_stmt<Expr>(),
					get<QualType>(value[0]),
					get<QualType>(value[1])));
}*/

/*void ASTContext::createDependentDecltypeType(std::vector<ASTContext::var_t> &value)
{
	queue_type.push(
			make_shared<DependentDecltypeType>(
					pop_stmt<Expr>()));
}*/
