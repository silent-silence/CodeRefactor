//
// Created by 17271 on 2019/6/11.
//

#ifndef CODEREFACTOR_DECLITERATOR_H
#define CODEREFACTOR_DECLITERATOR_H

#include <memory>
#include <list>

class Decl;

class DeclIterator {
public:
	DeclIterator(std::list<std::shared_ptr<Decl>>::iterator it);

	DeclIterator &operator++();
	DeclIterator &operator--();

	std::shared_ptr<Decl> operator *();

	bool operator ==(DeclIterator it) const;
	bool operator !=(DeclIterator it) const;

private:
	std::list<std::shared_ptr<Decl>>::iterator m_it;
};

#endif //CODEREFACTOR_DECLITERATOR_H
