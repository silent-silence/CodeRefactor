#include "ASTContext.h"
#include "AST/Expr.h"

using std::get;							using std::string;
using std::queue;						using std::holds_alternative;
using std::vector;						using std::shared_ptr;
using std::make_shared;					using std::dynamic_pointer_cast;

void ASTContext::updateASTRoot(std::shared_ptr<Stmt> root)
{
	m_ASTRoot = root;
}

#ifdef ENV_TEST
std::weak_ptr<Stmt> ASTContext::getRoot() const
{
	return m_ASTRoot;
}

void ASTContext::cleanAST()
{
	m_ASTRoot = std::shared_ptr<Stmt>();
}
#endif

std::shared_ptr<Stmt> ASTContext::createDeclStmt(std::vector<var_t> &value)
{
	return make_shared<DeclStmt>(
					get<SourceLocation>(value[0]),
					get<SourceLocation>(value[1])
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
					get<vector<shared_ptr<Stmt>>>(value[0]),
					get<SourceLocation>(value[1]),
					get<SourceLocation>(value[2])
	);
}

std::shared_ptr<Stmt> ASTContext::createCaseStmt(std::vector<ASTContext::var_t> &value)
{
	return make_shared<CaseStmt>(
					get<shared_ptr<Expr>>(value[0]),
					get<shared_ptr<Expr>>(value[1]),
					get<SourceLocation>(value[2]),
					get<SourceLocation>(value[3]),
					get<SourceLocation>(value[4])
	);
}

std::shared_ptr<Stmt> ASTContext::createDefaultStmt(std::vector<ASTContext::var_t> &value)
{
	return make_shared<DefaultStmt>(
					get<SourceLocation>(value[0]),
					get<SourceLocation>(value[1]),
					get<shared_ptr<Stmt>>(value[2])
	);
}

std::shared_ptr<Stmt> ASTContext::createLabelStmt(std::vector<ASTContext::var_t> &value)
{
	// TODO
	/*return make_shared<LabelStmt>(
					get<SourceLocation>(value[0]),
					pop_stmt()
	);*/
}

std::shared_ptr<Stmt> ASTContext::createIfStmt(std::vector<ASTContext::var_t> &value)
{
	// TODO
	/*return make_shared<IfStmt>(
					get<SourceLocation>(value[0]),
					pop_stmt<Expr>(),
					pop_stmt(),
					value[1].emplace<SourceLocation>(SourceLocation())\
	);*/
}

std::shared_ptr<Stmt> ASTContext::createSwitchStmt(std::vector<ASTContext::var_t> &value)
{
	return make_shared<SwitchStmt>(
			get<SourceLocation>(value[0]),
			get<shared_ptr<Expr>>(value[1])
	);
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
	// TODO
	/*return make_shared<ForStmt>(
					pop_stmt(),
					pop_stmt<Expr>(),
					pop_stmt<Expr>(),
					pop_stmt(),
					get<SourceLocation>(value[0]),
					get<SourceLocation>(value[1]),
					get<SourceLocation>(value[2])
	);*/
}

std::shared_ptr<Stmt> ASTContext::createGotoStmt(std::vector<ASTContext::var_t> &value)
{
	// TODO
	/*queue.push(
			make_shared<GotoStmt>(
					pop_stmt<LabelStmt>(),
					get<SourceLocation>(value[0]),
					get<SourceLocation>(value[1])));*/
}

std::shared_ptr<Stmt> ASTContext::createIndirectGotoStmt(std::vector<ASTContext::var_t> &value)
{
	// TODO
	/*queue.push(
			make_shared<IndirectGotoStmt>(
					get<SourceLocation>(value[0]),
					get<SourceLocation>(value[1]),
					pop_stmt<Expr>()));*/
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

std::shared_ptr<Stmt> ASTContext::createDeclRefExpr(std::vector<ASTContext::var_t> &value)
{
	// TODO
	/*queue.push(
			make_shared<DeclRefExpr>(
					pop_stmt<NamedDecl>(),//error
					get<QualType>(value[0]),
					get<SourceLocation>(value[1]),
					get<bool>(value[2]),
					get<bool>(value[3])));*/
}

std::shared_ptr<Stmt> ASTContext::createPredefinedExpr(std::vector<ASTContext::var_t> &value)
{
	// TODO
	/*queue.push(
			make_shared<PredefinedExpr>(
					get<SourceLocation>(value[0]),
					get<QualType>(value[1]),
					get<PredefinedExpr::IdentType>(value[2])));*/
}

std::shared_ptr<Stmt> ASTContext::createIntegerLiteral(std::vector<ASTContext::var_t> &value)
{
	// TODO: make default integer type
	QualType type(make_shared<Type>(), 0);
	return make_shared<IntegerLiteral>(
					get<int>(value[0]),
					type,
					get<SourceLocation>(value[1])
	);
}

std::shared_ptr<Stmt> ASTContext::createCharacterLiteral(std::vector<ASTContext::var_t> &value)
{
	// TODO: make default character type
	QualType type(make_shared<Type>(), 0);
	return make_shared<CharacterLiteral>(
					get<unsigned>(value[0]),
					get<bool>(value[1]),
					type,
					get<SourceLocation>(value[2])
	);
}

std::shared_ptr<Stmt> ASTContext::createFloatingLiteral(std::vector<ASTContext::var_t> &value)
{
	// TODO: make default floating type
	QualType type(make_shared<Type>(), 0);
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
}

std::shared_ptr<Stmt> ASTContext::createStringLiteral(std::vector<ASTContext::var_t> &value)
{
	// TODO: make default string type
	QualType type(make_shared<Type>(), 0);
	return StringLiteral::Create(
			get<char *>(value[0]),
			get<unsigned long>(value[1]),
			get<bool>(value[2]),
			type,
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
	// TODO: get operator type
	QualType type(make_shared<Type>(), 0);
	return make_shared<UnaryOperator>(
					get<shared_ptr<Expr>>(value[0]),
					get<UnaryOperator::Opcode>(value[1]),
					type,
					get<SourceLocation>(value[2])
	);
}

std::shared_ptr<Stmt> ASTContext::createSizeOfAlignOfExpr(std::vector<ASTContext::var_t> &value)
{
	//TODO: set return type
	QualType retType(make_shared<Type>(), 0);
	if(holds_alternative<shared_ptr<Type>>(value[3]))
	{
		return make_shared<SizeOfAlignOfExpr>(
						true,
						get<QualType>(value[3]),
						retType,
						get<SourceLocation>(value[0]),
						get<SourceLocation>(value[1])
		);
	}
	else
	{
		return make_shared<SizeOfAlignOfExpr>(
						true,
						get<shared_ptr<Expr>>(value[3]),
						retType,
						get<SourceLocation>(value[0]),
						get<SourceLocation>(value[1])
		);
	}
}

std::shared_ptr<Stmt> ASTContext::createArraySubscriptExpr(std::vector<ASTContext::var_t> &value)
{
	// TODO: get array elements' type
	QualType arrayType(make_shared<Type>(), 0);
	return make_shared<ArraySubscriptExpr>(
					get<shared_ptr<Expr>>(value[0]),
					get<shared_ptr<Expr>>(value[1]),
					arrayType,
					get<SourceLocation>(value[2])
	);
}

std::shared_ptr<Stmt> ASTContext::createCallExpr(std::vector<ASTContext::var_t> &value)
{
	// TODO: get return type
	// TODO: init CallExpr::SubExpr
	QualType retType(make_shared<Type>(), 0);
	auto args = get<vector<shared_ptr<Expr>>>(value[1]);
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
	// TODO: Get type here
	QualType memberType(make_shared<Type>(), 0);
	return make_shared<MemberExpr>(
					get<shared_ptr<Expr>>(value[0]),
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
}

std::shared_ptr<Stmt> ASTContext::createCStyleCastExpr(std::vector<ASTContext::var_t> &value)
{
	// TODO Get types
	QualType exprType(make_shared<Type>(), 0);
	return make_shared<CStyleCastExpr>(
					exprType,
					get<CastExpr::CastKind>(value[0]),
					get<shared_ptr<Expr>>(value[1]),
					get<QualType>(value[2]),
					get<SourceLocation>(value[3]),
					get<SourceLocation>(value[4])
	);
}

std::shared_ptr<Stmt> ASTContext::createBinaryOperator(std::vector<ASTContext::var_t> &value)
{
	// TODO Get operation type
	QualType exprType(make_shared<Type>(), 0);
	return make_shared<BinaryOperator>(
					get<shared_ptr<Expr>>(value[0]),
					get<shared_ptr<Expr>>(value[1]),
					get<BinaryOperator::Opcode>(value[2]),
					exprType,
					get<SourceLocation>(value[3])
	);
}

std::shared_ptr<Stmt> ASTContext::createCompoundAssignOperator(std::vector<ASTContext::var_t> &value)
{
	// TODO Get types
	QualType ResType(make_shared<Type>(), 0);
	QualType CompLHSType(make_shared<Type>(), 0);
	QualType CompResultType(make_shared<Type>(), 0);
	return make_shared<CompoundAssignOperator>(
					get<shared_ptr<Expr>>(value[0]),
					get<shared_ptr<Expr>>(value[1]),
					get<CompoundAssignOperator::Opcode>(value[2]),
					ResType,
					CompLHSType,
					CompResultType,
					get<SourceLocation>(value[3])
	);
}

std::shared_ptr<Stmt> ASTContext::createConditionalOperator(std::vector<ASTContext::var_t> &value)
{
	// TODO Get type here
	QualType type(make_shared<Type>(), 0);
	return make_shared<ConditionalOperator>(
					get<shared_ptr<Expr>>(value[0]),
					get<shared_ptr<Expr>>(value[1]),
					get<shared_ptr<Expr>>(value[2]),
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

std::shared_ptr<Type> ASTContext::createBuiltinType(std::vector<ASTContext::var_t> &value)
{
	return make_shared<BuiltinType>(get<BuiltinType::Kind>(value[0]));
}

std::shared_ptr<Type> ASTContext::createFixedWidthIntType(std::vector<ASTContext::var_t> &value)
{
	// TODO
	/*return FixedWidthIntType(
					get<unsigned>(value[0]),
					get<bool>(value[1])
	);*/
}

std::shared_ptr<Type> ASTContext::createComplexType(std::vector<ASTContext::var_t> &value)
{
	// TODO
	/*return ComplexType(
					get<QualType>(value[0]),
					get<QualType>(value[1])
	);*/
}

std::shared_ptr<Type> ASTContext::createPointerType(std::vector<ASTContext::var_t> &value)
{
	// TODO
	/*return PointerType(
					get<QualType>(value[0]),
					get<QualType>(value[1])
	);*/
}

std::shared_ptr<Type> ASTContext::createBlockPointerType(std::vector<ASTContext::var_t> &value)
{
	// TODO
	/*return BlockPointerType(
					get<QualType>(value[0]),
					get<QualType>(value[1])
	);*/
}

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

std::shared_ptr<Type> ASTContext::createConstantArrayType(std::vector<ASTContext::var_t> &value)
{
	// TODO
	/*return ConstantArrayType(
					get<QualType>(value[0]),
					get<QualType>(value[1]),
					get<int>(value[2]),
					get<ArrayType::ArraySizeModifier>(value[3]),
					get<unsigned>(value[4])
	);*/
}

std::shared_ptr<Type> ASTContext::createConstantArrayWithExprType(std::vector<ASTContext::var_t> &value)
{
	// TODO
	/*return ConstantArrayWithExprType(
					get<QualType>(value[0]),
					get<QualType>(value[1]),
					get<int>(value[2]),
					get<shared_ptr<Expr>>(value[5]),
					get<ArrayType::ArraySizeModifier>(value[3]),
					get<unsigned>(value[4])
	);*/
}

std::shared_ptr<Type> ASTContext::createConstantArrayWithoutExprType(std::vector<ASTContext::var_t> &value)
{
	// TODO
	/*return ConstantArrayWithoutExprType(
					get<QualType>(value[0]),
					get<QualType>(value[1]),
					get<int>(value[2]),
					get<ArrayType::ArraySizeModifier>(value[3]),
					get<unsigned>(value[4])
	);*/
}

std::shared_ptr<Type> ASTContext::createIncompleteArrayType(std::vector<ASTContext::var_t> &value)
{
	// TODO
	/*return IncompleteArrayType(
					get<QualType>(value[0]),
					get<QualType>(value[1]),
					get<ArrayType::ArraySizeModifier>(value[2]),
					get<unsigned>(value[3])
	);*/
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

std::shared_ptr<Type> ASTContext::createFunctionNoProtoType(std::vector<ASTContext::var_t> &value)
{
	// TODO
	/*return FunctionNoProtoType(
					get<QualType>(value[0]),
					get<QualType>(value[1]),
					get<bool>(value[2])
	);*/
}

std::shared_ptr<Type> ASTContext::createFunctionProtoType(std::vector<ASTContext::var_t> &value)
{
//    queue_type.push(
//                make_shared<FunctionProtoType>(
//                    get<QualType>(value[0]),
//                get<QualType>(value[1]),
	//            get<bool>(value[2])));
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
