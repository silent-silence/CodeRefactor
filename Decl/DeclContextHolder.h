//
// Created by 17271 on 2019/5/22.
//

#ifndef CODEREFACTOR_DECLCONTEXTHOLDER_H
#define CODEREFACTOR_DECLCONTEXTHOLDER_H

#include <string>
#include <memory>

class SourceLocation;
class DeclContext;
class QualType;
class Decl;
class Stmt;
class IdentifierInfo;
class TypedefDecl;
class Expr;

/// @brief This class hold the root of DeclContext, and provide create decls' methods
class DeclContextHolder {
public:
	DeclContextHolder();

	std::shared_ptr<DeclContext> getContextRoot() const;

#ifdef ENV_TEST
	void clean();
#endif

	/// @brief Do create decls
	std::shared_ptr<Decl> createVariable(std::shared_ptr<DeclContext> &context, std::string &name, SourceLocation &location, std::shared_ptr<QualType> type);
	std::shared_ptr<Decl> createTypedefDecl(std::shared_ptr<DeclContext> &context, std::shared_ptr<QualType> type, std::string &name, SourceLocation &location);
	std::shared_ptr<DeclContext> createBlock(std::shared_ptr<DeclContext> &context, SourceLocation &&location);
	std::shared_ptr<DeclContext> createStruct(std::shared_ptr<DeclContext> &context, SourceLocation &&location, std::string &name);
	std::shared_ptr<DeclContext> createStruct(std::shared_ptr<DeclContext> &context, SourceLocation &&location);
	std::shared_ptr<Decl> createEnum(std::shared_ptr<DeclContext> &context, SourceLocation &&location, std::string &name);
	std::shared_ptr<Decl> createEnum(std::shared_ptr<DeclContext> &context, SourceLocation &&location);
	std::shared_ptr<Decl> createEnumConstant(std::shared_ptr<DeclContext> &context, SourceLocation &&location, std::string name, std::shared_ptr<Expr> init);
	std::shared_ptr<Decl> createFunction(std::shared_ptr<DeclContext> &context, std::string &name, SourceLocation &location, std::shared_ptr<QualType> type);
	std::shared_ptr<Decl> createParmVar(std::shared_ptr<DeclContext> &context, std::string &name, SourceLocation &location, std::shared_ptr<QualType> type);

private:
	std::shared_ptr<DeclContext> m_contextRoot;
};


#endif //CODEREFACTOR_DECLCONTEXTHOLDER_H
