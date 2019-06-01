#ifndef DECLGROUP_H
#define DECLGROUP_H

#include <memory>
#include <vector>

class Decl;
class DeclGroup;

/// @brief A reference to DeclGroup, can be one of a Decl or a DeclGroup
class DeclGroupRef {
public:
	enum class Kind { SingleDeclKind, DeclGroupKind };
	DeclGroupRef();
	explicit DeclGroupRef(std::shared_ptr<Decl> d);
	explicit DeclGroupRef(std::shared_ptr<DeclGroup> dg);
	DeclGroupRef(std::vector<std::shared_ptr<Decl>> Decls, unsigned NumDecls);

	bool isNull() const { return bool(D); }
	bool isSingleDecl() const { return m_groupKind == Kind::SingleDeclKind; }
	bool isDeclGroup() const { return m_groupKind == Kind::DeclGroupKind; }

	std::weak_ptr<Decl> getSingleDecl();
	const std::weak_ptr<Decl> getSingleDecl() const;

	std::weak_ptr<DeclGroup> getDeclGroup();
	const std::weak_ptr<DeclGroup> getDeclGroup() const;

	/*void *getAsOpaquePtr() const { return D; }
	static DeclGroupRef getFromOpaquePtr(std::shared_ptr<void> Ptr);*/
private:
	std::shared_ptr<Decl> D;
	std::shared_ptr<DeclGroup> groupRef;
	Kind m_groupKind;
};

/// @brief Holds Several number of Decls
class DeclGroup {
public:
	DeclGroup(unsigned numdecls, std::vector<std::shared_ptr<Decl>> decls);

	unsigned size() const;
	std::shared_ptr<Decl> &operator[](unsigned i);
	std::shared_ptr<Decl> const &operator[](unsigned i) const;
private:
	DeclGroup();

	unsigned NumDecls;
	std::vector<std::shared_ptr<Decl>> Decls;
};

#endif // DECLGROUP_H
