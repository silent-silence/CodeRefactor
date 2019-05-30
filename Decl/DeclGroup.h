#ifndef DECLGROUP_H
#define DECLGROUP_H

#include <memory>
#include <vector>

class Decl;

class DeclGroup {
public:
    static std::shared_ptr<DeclGroup> Create(std::vector<std::shared_ptr<Decl>> Decls, unsigned NumDecls);
    DeclGroup(unsigned numdecls, std::vector<std::shared_ptr<Decl>> decls);
    unsigned size() const;
    std::shared_ptr<Decl>& operator[](unsigned i);
    std::shared_ptr<Decl> const& operator[](unsigned i) const;
private:
    DeclGroup();

    unsigned NumDecls;
    std::vector<std::shared_ptr<Decl>> Decls;
};

class DeclGroupRef {
    enum Kind { SingleDeclKind=0x0, DeclGroupKind=0x1, Mask=0x1 };
    std::shared_ptr<Decl> D;

    Kind getKind() const {
//        return (Kind) (std::reinterpret_pointer_cast<uintptr_t>(D) & Mask);
    }

public:
    DeclGroupRef();

    explicit DeclGroupRef(std::shared_ptr<Decl> d);
//    explicit DeclGroupRef(std::shared_ptr<DeclGroup> dg)
//        : D((Decl*) (reinterpret_cast<uintptr_t>(dg) | DeclGroupKind)) {}

    static DeclGroupRef Create(std::vector<std::shared_ptr<Decl>> Decls, unsigned NumDecls);

    typedef std::shared_ptr<Decl>* iterator;
    typedef std::shared_ptr<Decl> const * const_iterator;

    bool isNull() const { return D == 0; }
    bool isSingleDecl() const { return getKind() == SingleDeclKind; }
    bool isDeclGroup() const { return getKind() == DeclGroupKind; }

    std::weak_ptr<Decl> getSingleDecl();
    const std::weak_ptr<Decl> getSingleDecl() const;

    DeclGroup &getDeclGroup();
    const DeclGroup &getDeclGroup() const;

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

//    void *getAsOpaquePtr() const { return D; }
    static DeclGroupRef getFromOpaquePtr(std::shared_ptr<void> Ptr);
};

#endif // DECLGROUP_H
