#ifndef DECL_BASE_H
#define DECL_BASE_H

#include <memory>
#include <string>
#include <unordered_map>
#include "Basic/SourceLocation.h"
#include "DeclName.h"

class DeclContext;
class NamedDecl;

/// @brief Represent a declaration
class Decl {
public:
	/// @brief The Kind of declaration
	enum Kind {
#define DECL(Derived, Base) Derived,
#define DECL_RANGE(CommonBase, Start, End) \
    CommonBase##First = Start, CommonBase##Last = End,
#define LAST_DECL_RANGE(CommonBase, Start, End) \
    CommonBase##First = Start, CommonBase##Last = End
#include "Decl/DeclNodes.def"
	};

	Decl(Kind declKind, SourceLocation location)
		: m_kind{declKind}, m_loc{location}, m_isUsed{false}
	{}
	Decl(Kind declKind, std::weak_ptr<DeclContext> context, SourceLocation location)
		: m_kind{declKind}, m_belongToContext{context}, m_loc{location}, m_isUsed{false}
	{}
	virtual ~Decl() = 0;

	/*/// @brief According to C99 6.2.3, there are four namespaces, labels, tags, members and ordinary
	enum IdentifierNamespace {
		IDNS_Label = 0x1,
		IDNS_Tag = 0x2,
		IDNS_Member = 0x4,
		IDNS_Ordinary = 0x8,
	};*/

	/// @brief Get locations in code
	/*virtual SourceRange getSourceRange() const;
	SourceLocation getLocStart() const;
	SourceLocation getLocEnd() const;*/

	/// @brief Get kind of the declaration
	Kind getKind() const { return m_kind; }
	//std::string getDeclKindName();

	/// @brief Get context the declaration belong
	std::weak_ptr<Decl> getNextDeclInContext() { return m_nextDeclInContext; }
	const std::weak_ptr<Decl> getNextDeclInContext() const { return m_nextDeclInContext; }

	std::weak_ptr<DeclContext> getDeclContext() { return m_belongToContext; }
	const std::weak_ptr<DeclContext> getDeclContext() const { return m_belongToContext;	}

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

/// @brief StoreDecl acts like a union
class StoredDecl {
public:
	enum class StoredKind {
		DeclPtr,
		DeclId
	};
	StoredDecl() = default;
	StoredDecl(StoredKind kind, std::shared_ptr<NamedDecl> ptr = nullptr);
	virtual ~StoredDecl() = default;

	/// @brief Get decl name or decl id as string
	virtual std::string getDeclAsString() const;

	/// @brief An hash algorithm
	size_t operator()(const StoredDecl &name) const;

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

bool operator ==(const StoredDecl &s1, const StoredDecl &s2);

/// @brief A context is where a decl stored
class DeclContext {
public:
	DeclContext(Decl::Kind K)
		: m_declKind{K}
	{}

	virtual ~DeclContext() = default;

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

	std::weak_ptr<Decl> lookupCurrentContext(StoredDecl &name);
	std::weak_ptr<Decl> lookupCurrentContext(StoredDecl &&name);
	std::weak_ptr<Decl> lookupCurrentContext(std::string &name);
	std::weak_ptr<Decl> lookupCurrentContext(std::string &&name);

	//void makeDeclVisibleInContext(std::shared_ptr<NamedDecl> d);

private:
	Decl::Kind m_declKind;
	std::unordered_map<StoredDecl, std::shared_ptr<Decl>, StoredDecl> m_decls;
};

#endif