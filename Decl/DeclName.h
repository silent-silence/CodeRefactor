#ifndef DECLARATION_NAME_H
#define DECLARATION_NAME_H

#include <string>

class IdentifierInfo;

/// @brief The identity of a declaration
// TODO: this class can be removed
class DeclName {
public:
	DeclName(const std::shared_ptr<IdentifierInfo> id)
		: m_storedIdentifier{id}
	{}
	virtual ~DeclName() = default;

	virtual std::string getAsString() const;
	virtual std::weak_ptr<IdentifierInfo> getAsIdentifierInfo() const;

protected:
	DeclName() = default;

private:
	std::shared_ptr<IdentifierInfo> m_storedIdentifier;
};

#endif
