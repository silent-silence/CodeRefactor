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

	//  tok::TokenKind getTokenID() const { return (tok::TokenKind)TokenID; }
	//  void setTokenID(tok::TokenKind ID) { TokenID = ID; }

	//  tok::PPKeywordKind getPPKeywordID() const;

	/*bool isExtensionToken() const;
	void setIsExtensionToken(bool Val);

	void setIsPoisoned(bool Value = true);
	bool isPoisoned() const;

	void setIsCPlusPlusOperatorKeyword(bool Val = true);
	bool isCPlusPlusOperatorKeyword() const;*/

	/*template<typename T>
	std::weak_ptr<T> getFETokenInfo() const { return std::reinterpret_pointer_cast<T>(FETokenInfo); }
	void setFETokenInfo(std::shared_ptr<void> T) { FETokenInfo = T; }*/

	/*bool isHandleIdentifierCase() const { return NeedsHandleIdentifier; }*/

private:
	static unsigned anonymousNameId;

	std::string m_entry;
	/*std::map<std::shared_ptr<IdentifierInfo>, std::string> Entry;*/
	/*unsigned TokenID            : 8;*/
    bool HasMacro;// True if there is a #define for this.
	/*bool IsExtension            : 1; // True if identifier is a lang extension.
	bool IsPoisoned             : 1; // True if identifier is poisoned.
	bool IsCPPOperatorKeyword   : 1; // True if ident is a C++ operator keyword.
	bool NeedsHandleIdentifier  : 1; // See "RecomputeNeedsHandleIdentifier".*/
	// 9 bits left in 32-bit word.
	//std::shared_ptr<void> FETokenInfo;

/*private:
    void RecomputeNeedsHandleIdentifier();*/
};

#endif // IDENTIFIERTABLE_H
