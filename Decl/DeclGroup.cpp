#include "DeclGroup.h"

using std::shared_ptr;
using std::make_shared;

DeclGroup::DeclGroup(unsigned numdecls, std::vector<std::shared_ptr<Decl> > decls)
		: NumDecls(numdecls)
{
	Decls=decls;
}

unsigned DeclGroup::size() const
{
	return NumDecls;
}

const shared_ptr<Decl> &DeclGroup::operator[](unsigned i) const
{
	return Decls[i];
}

std::shared_ptr<Decl> &DeclGroup::operator[](unsigned i) {
	return Decls[i];
}

DeclGroup::DeclGroup()
		: NumDecls(0)
{}

DeclGroupRef::DeclGroupRef()
	: D(nullptr), groupRef(nullptr)
{}

DeclGroupRef::DeclGroupRef(std::shared_ptr<Decl> d)
	: D(d), m_groupKind{Kind::SingleDeclKind}, groupRef{nullptr}
{}

DeclGroupRef::DeclGroupRef(std::shared_ptr<DeclGroup> dg)
	: groupRef{dg}, m_groupKind{Kind::DeclGroupKind}, D{nullptr}
{}

DeclGroupRef::DeclGroupRef(std::vector<std::shared_ptr<Decl>> Decls, unsigned NumDecls)
	: groupRef(make_shared<DeclGroup>(NumDecls, Decls)), m_groupKind{Kind::DeclGroupKind}, D{nullptr}
{}

std::weak_ptr<Decl> DeclGroupRef::getSingleDecl()
{
	return D;
}

const std::weak_ptr<Decl> DeclGroupRef::getSingleDecl() const
{
	return D;
}

std::weak_ptr<DeclGroup> DeclGroupRef::getDeclGroup()
{
	return groupRef;
}

const std::weak_ptr<DeclGroup> DeclGroupRef::getDeclGroup() const
{
	return groupRef;
}

/*DeclGroupRef DeclGroupRef::getFromOpaquePtr(std::shared_ptr<void> Ptr)
{
	DeclGroupRef X;
	X.D = std::static_pointer_cast<Decl>(Ptr);
	return X;
}*/
