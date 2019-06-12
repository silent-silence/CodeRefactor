#ifndef DECLGROUP_H
#define DECLGROUP_H

#include <memory>
#include <vector>

class Decl;
class DeclGroup;

/// @brief Represent reference to decls, can be a single Decl or multiply decls in DeclGroup
class DeclGroupRef {
public:
	enum class Kind { SingleDeclKind, DeclGroupKind };
	DeclGroupRef();
	explicit DeclGroupRef(std::shared_ptr<Decl> d);
	explicit DeclGroupRef(std::shared_ptr<DeclGroup> dg);
	explicit DeclGroupRef(std::vector<std::shared_ptr<Decl>> Decls);

	bool isNull() const { return bool(D); }
	bool isSingleDecl() const { return m_groupKind == Kind::SingleDeclKind; }
	bool isDeclGroup() const { return m_groupKind == Kind::DeclGroupKind; }

	std::weak_ptr<Decl> getSingleDecl();
	const std::weak_ptr<Decl> getSingleDecl() const;

	std::weak_ptr<DeclGroup> getDeclGroup();
	const std::weak_ptr<DeclGroup> getDeclGroup() const;

private:
	std::shared_ptr<Decl> D;
	std::shared_ptr<DeclGroup> groupRef;
	Kind m_groupKind;
};

/// @brief Holds Several number of Decls
class DeclGroup {
public:
	DeclGroup(std::vector<std::shared_ptr<Decl>> decls);

	unsigned size() const;
	std::shared_ptr<Decl> &operator[](unsigned i);
	std::shared_ptr<Decl> const &operator[](unsigned i) const;
private:
	DeclGroup() = default;

	std::vector<std::shared_ptr<Decl>> Decls;
};

#endif // DECLGROUP_H
