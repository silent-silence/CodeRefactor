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

/// @brief This class hold the root of DeclContext, and provide create decls' methods
class DeclContextHolder {
public:
	DeclContextHolder();

	std::shared_ptr<DeclContext> getContext() const;

#ifdef ENV_TEST
	void clean();
#endif

	std::shared_ptr<Decl> createVariable(std::shared_ptr<DeclContext> &context, std::string name, SourceLocation &location, std::shared_ptr<QualType> type);
	std::shared_ptr<DeclContext> createBlock(std::shared_ptr<DeclContext> &context, SourceLocation &&location);
	void createFunction();

private:
	std::shared_ptr<DeclContext> m_contextRoot;
};


#endif //CODEREFACTOR_DECLCONTEXTHOLDER_H
