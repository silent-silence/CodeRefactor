#ifndef DECL_BASE_H
#define DECL_BASE_H

#include <memory>
#include <string>
#include <unordered_map>
#include "Basic/SourceLocation.h"
#include "DeclName.h"

class DeclContext;
class SourceRange;
class NamedDecl;

/// @brief Represent a declaration
class Decl {
public:
	Decl(std::weak_ptr<DeclContext> context, SourceLocation location)
		: m_belongToContext{context}, m_loc{location}, m_isUsed{false}
	{}
	virtual ~Decl() = 0;

	/// @brief The Kind of declaration
	enum class Kind {
	};

	/// @brief Get locations in code
	/*virtual SourceRange getSourceRange() const;
	SourceLocation getLocStart() const;
	SourceLocation getLocEnd() const;*/

	/// @brief Get kind of the declaration
	Kind getKind() const { return m_kind; }
	//std::string getDeclKindName();

	/// @brief Get context the declaration belong
	std::weak_ptr<DeclContext> getDeclContext() { return m_belongToContext; }
	const std::weak_ptr<DeclContext> getDeclContext() const { return m_belongToContext; }

private:
	/// @brief Which context does the declaration belong
	std::weak_ptr<DeclContext> m_belongToContext;

	/// @brief Declaration location in code
	SourceLocation m_loc;

	/// @brief Kind of the declaration
	Kind m_kind;

	/// @brief Haven used
	bool m_isUsed;
};

class DeclContext {
public:
	DeclContext() = default;
	virtual ~DeclContext() = default;

	Decl::Kind getDeclKind() const;
	std::string getDeclKindName() const;

	std::weak_ptr<DeclContext> getParent() { return m_parentDecl; };
	const std::weak_ptr<DeclContext> getParent() const { return m_parentDecl; };

	bool isFunction() const;
	bool isFileContext() const;
	bool isTranslationUnit() const;
	bool isRecord() const;
	bool isTransparentContext() const;
	bool encloses(DeclContext *dc) const;

	//std::weak_ptr<DeclContext> getPrimaryContext();
	//std::weak_ptr<DeclContext> getLookupContext();
	//const std::weak_ptr<DeclContext> getLookupContext() const;

	void addDecl(std::shared_ptr<Decl> d);
	//void addHiddenDecl(std::shared_ptr<Decl> d);

	std::weak_ptr<Decl> lookup(DeclName name) {
		return m_decls.find(name) != m_decls.cend() ? m_decls.find(name)->second : nullptr;
	}

	//void makeDeclVisibleInContext(std::shared_ptr<NamedDecl> d);

private:
	std::weak_ptr<DeclContext> m_parentDecl;
	std::unordered_map<DeclName, std::shared_ptr<Decl>, DeclarationNameHash> m_decls;
};

#endif