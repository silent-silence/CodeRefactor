#ifndef DECLARATION_NAME_H
#define DECLARATION_NAME_H

#include <string>

/// @brief The identity of a declaration
class DeclarationName {
public:
	DeclarationName() = default;
	DeclarationName(const char *s)
		: m_identity{s}
	{}
	std::string getIdentifier() const { return m_identity; }
	std::string getStoredName() const { return m_storedName; }

	bool operator==(const DeclarationName &name2) const {
		return m_storedName == name2.m_storedName;
	}

private:
	std::string m_identity;
	std::string m_storedName;
};

class DeclarationNameHash {
public:
	size_t operator()(const DeclarationName &name) const {
		return std::hash<std::string>()(name.getIdentifier());
	};
};


#endif
