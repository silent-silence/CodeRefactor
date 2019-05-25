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

class DeclContextHolder {
public:
	DeclContextHolder();

	std::shared_ptr<DeclContext> getContext() const;

	void createVariable(std::string name, SourceLocation &location, QualType type);

#ifdef ENV_TEST
	void clean();
#endif

private:
	std::shared_ptr<DeclContext> m_contextRoot;
};


#endif //CODEREFACTOR_DECLCONTEXTHOLDER_H
