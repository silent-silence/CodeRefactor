#include "DeclGroup.h"

using std::shared_ptr;
using std::make_shared;

std::shared_ptr<DeclGroup> DeclGroup::Create(std::vector<std::shared_ptr<Decl> > Decls, unsigned NumDecls)
{
    unsigned Size = sizeof(DeclGroup) + sizeof(std::shared_ptr<Decl>) * NumDecls;
    return std::move(make_shared<DeclGroup>(NumDecls, Decls));
}

unsigned DeclGroup::size() const
{
    return NumDecls;
}

const shared_ptr<Decl> &DeclGroup::operator[](unsigned i) const
{
    return Decls[i];
}

DeclGroup::DeclGroup()
    : NumDecls(0)
{}

std::shared_ptr<Decl> &DeclGroup::operator[](unsigned i) {
    return Decls[i];
}

DeclGroup::DeclGroup(unsigned numdecls, std::vector<std::shared_ptr<Decl> > decls)
    : NumDecls(numdecls)
{
    Decls=decls;
}

DeclGroupRef::DeclGroupRef()
    : D(nullptr)
{}

DeclGroupRef::DeclGroupRef(std::shared_ptr<Decl> d)
    : D(d)
{}

DeclGroupRef DeclGroupRef::Create(std::vector<std::shared_ptr<Decl> > Decls, unsigned NumDecls)
{
    if (NumDecls == 0)
        return DeclGroupRef();
    if (NumDecls == 1)
        return DeclGroupRef(Decls[0]);
    //        return DeclGroupRef(DeclGroup::Create(Decls, NumDecls));
}

std::weak_ptr<Decl> DeclGroupRef::getSingleDecl()
{
    return D;
}

const std::weak_ptr<Decl> DeclGroupRef::getSingleDecl() const
{
    return this->getSingleDecl();
}

DeclGroup &DeclGroupRef::getDeclGroup()
{
    //        return *((DeclGroup*)(reinterpret_cast<uintptr_t>(D) & ~Mask));
}

const DeclGroup &DeclGroupRef::getDeclGroup() const
{
    return this->getDeclGroup();
}

DeclGroupRef::iterator DeclGroupRef::begin()
{
    if (isSingleDecl())
        return D ? &D : nullptr;
    return &getDeclGroup()[0];
}

DeclGroupRef::iterator DeclGroupRef::end()
{
    if (isSingleDecl())
        return D ? &D+1 : nullptr;
    DeclGroup &G = getDeclGroup();
    return &G[0] + G.size();
}

DeclGroupRef::const_iterator DeclGroupRef::begin() const
{
    if (isSingleDecl())
        return D ? &D : nullptr;
    return &getDeclGroup()[0];
}

DeclGroupRef::const_iterator DeclGroupRef::end() const
{
    if (isSingleDecl())
        return D ? &D+1 : nullptr;
    const DeclGroup &G = getDeclGroup();
    return &G[0] + G.size();
}

DeclGroupRef DeclGroupRef::getFromOpaquePtr(std::shared_ptr<void> Ptr)
{
    DeclGroupRef X;
    X.D = std::static_pointer_cast<Decl>(Ptr);
    return X;
}
