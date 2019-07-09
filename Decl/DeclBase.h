#ifndef DECL_BASE_H
#define DECL_BASE_H

#include <memory>
#include <string>
#include <unordered_map>
#include <list>
#include "Basic/SourceLocation.h"
#include "Iterator/DeclIterator.h"

class DeclContext;
class NamedDecl;

/// @brief Represent a declaration
class Decl {
public:
	/// @brief The Kind of declaration
	enum class Kind : unsigned {
#define DECL(Derived, Base) Derived,
#define DECL_RANGE(CommonBase, Start, End) \
    CommonBase##First = Start, CommonBase##Last = End,
#define LAST_DECL_RANGE(CommonBase, Start, End) \
    CommonBase##First = Start, CommonBase##Last = End
#include "Decl/DeclNodes.def"
	};

	Decl(Kind declKind, SourceLocation location);
	Decl(Kind declKind, std::weak_ptr<DeclContext> context, SourceLocation location);
	virtual ~Decl() = 0;

	/// @brief Get locations in code
	/*virtual SourceRange getSourceRange() const;
	SourceLocation getLocStart() const;*/
	SourceLocation getSourceLocation() const;

	/// @brief Get kind of the declaration
	Decl::Kind getKind() const;
	//std::string getDeclKindName();

	/// @brief Get context the declaration belong
	std::weak_ptr<Decl> getNextDeclInContext();
	const std::weak_ptr<Decl> getNextDeclInContext() const;

	std::weak_ptr<DeclContext> getDeclContext();
	const std::weak_ptr<DeclContext> getDeclContext() const;

private:
	/// @brief Which context does the declaration belong
	std::weak_ptr<DeclContext> m_belongToContext;
	std::weak_ptr<Decl> m_nextDeclInContext;

	/// @brief Declaration location in code
	SourceLocation m_loc;

	/// @brief Kind of the declaration
	Kind m_kind;

	unsigned int InvalidDecl :  1;
	unsigned int HasAttrs : 1;
	bool Implicit : 1;
	/// @brief Haven used
	bool m_isUsed;
};

/// @brief StoreDecl acts like a union, stored an pointer to named decl or id to unnamed decl
class StoredDecl {
public:
	enum class StoredKind {
		DeclPtr,
		DeclId
	};
	StoredDecl();
	explicit StoredDecl(StoredKind kind, std::shared_ptr<NamedDecl> ptr = nullptr);
	virtual ~StoredDecl() = default;

	/// @brief Get decl name or decl id as string
	virtual std::string getDeclAsString() const;

private:
	StoredKind declKind;
	std::shared_ptr<NamedDecl> declPtr;
/// @brief System allocate an unique id for unnamed decl
	static unsigned declId;
};

/// @brief This should only be used in look up for a name
class LookupStoredDecl : public StoredDecl {
public:
	LookupStoredDecl(std::string name);
	~LookupStoredDecl() override = default;

	std::string getDeclAsString() const override;

private:
	std::string lookupName;
};

/// @brief A hash algorithm
class StoredDeclHash {
public:
	std::size_t operator()(const StoredDecl &name) const;
};

/// @brief Compare, used by unordered_map
bool operator ==(const StoredDecl &s1, const StoredDecl &s2);

/// @brief A context is where a decl stored
class DeclContext {
public:
	DeclContext(Decl::Kind K);
	virtual ~DeclContext() = 0;

	Decl::Kind getDeclKind() const;
	std::string getDeclKindName() const;

	/// @brief Get parent DeclContext
	virtual std::weak_ptr<DeclContext> getParent() = 0;
	virtual const std::weak_ptr<DeclContext> getParent() const = 0;

	//std::weak_ptr<DeclContext> getPrimaryContext();
	//std::weak_ptr<DeclContext> getLookupContext();
	//const std::weak_ptr<DeclContext> getLookupContext() const;

	/// @brief Add a decl
	void addDecl(std::shared_ptr<Decl> d);
	//void addHiddenDecl(std::shared_ptr<Decl> d);

	/// @brief Look up for a name in current and parent contexts
	std::weak_ptr<Decl> lookup(StoredDecl &name);
	std::weak_ptr<Decl> lookup(StoredDecl &&name);
	std::weak_ptr<Decl> lookup(std::string &name);
	std::weak_ptr<Decl> lookup(std::string &&name);

	/// @brief Look up for a name in current context
	std::weak_ptr<Decl> lookupCurrentContext(StoredDecl &name);
	std::weak_ptr<Decl> lookupCurrentContext(StoredDecl &&name);
	std::weak_ptr<Decl> lookupCurrentContext(std::string &name);
	std::weak_ptr<Decl> lookupCurrentContext(std::string &&name);

	/// @brief Look up for name in current and parent contexts, throw differently on existence
	/// @param name: name to be looked up
	/// @param lookIntoParent: need to look up into parent context(if any)
	/// @throw SymbolNotFound: if not find the symbol
	/// @throw SymbolAlreadyExist: if find the symbol
	void lookupForExistence(StoredDecl &name, bool lookIntoParent);

	void renameDecl(std::shared_ptr<Decl> decl, std::string newName);

	DeclIterator decl_begin();
	DeclIterator decl_end();

	//void makeDeclVisibleInContext(std::shared_ptr<NamedDecl> d);

	void moveToTail(std::shared_ptr<Decl> decl);

private:
	Decl::Kind m_declKind;
	std::unordered_map<StoredDecl, std::shared_ptr<Decl>, StoredDeclHash> m_decls;
	std::list<std::shared_ptr<Decl>> m_declsList;
};

#endif
