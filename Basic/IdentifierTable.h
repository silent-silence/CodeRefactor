#ifndef IDENTIFIERTABLE_H
#define IDENTIFIERTABLE_H

#include <map>
#include <memory>

/// @brief This class store an identifier name an is's attributes
class IdentifierInfo {
public:
	/// @brief Construct an unique but illegal name(@example '%%1') by default
	IdentifierInfo();
	/// @brief Explicit give a name
	IdentifierInfo(std::string name);

	IdentifierInfo(const IdentifierInfo&) = delete;  // NONCOPYABLE.
	void operator=(const IdentifierInfo&) = delete;  // NONASSIGNABLE.

	bool isStr(const std::string name) const;
	const std::string getName() const;

	bool hasMacroDefinition() const;
	void setHasMacroDefinition(bool Val);

	bool isAnonymous() const;

private:
	static unsigned anonymousNameId;
	/// @brief Name for the identifier
	std::string m_entry;
    bool HasMacro;// True if there is a #define for this.
};

#endif // IDENTIFIERTABLE_H
