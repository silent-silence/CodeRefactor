#ifndef IDENTIFIERTABLE_H
#define IDENTIFIERTABLE_H

#include <map>
#include <memory>

class IdentifierInfo {

    unsigned TokenID            : 8;
    bool HasMacro               : 1; // True if there is a #define for this.
    bool IsExtension            : 1; // True if identifier is a lang extension.
    bool IsPoisoned             : 1; // True if identifier is poisoned.
    bool IsCPPOperatorKeyword   : 1; // True if ident is a C++ operator keyword.
    bool NeedsHandleIdentifier  : 1; // See "RecomputeNeedsHandleIdentifier".
    // 9 bits left in 32-bit word.
    std::shared_ptr<void> FETokenInfo;
    std::map<std::shared_ptr<IdentifierInfo>, std::string> Entry;

    IdentifierInfo(const IdentifierInfo&);  // NONCOPYABLE.
    void operator=(const IdentifierInfo&);  // NONASSIGNABLE.
    friend class IdentifierTable;

public:
    IdentifierInfo();
    template <std::size_t StrLen>
    bool isStr(const char (&Str)[StrLen]) const {
        return getLength() == StrLen-1 && !memcmp(getName(), Str, StrLen-1);
    }
    const char *getName() const;
    unsigned getLength() const;

    bool hasMacroDefinition() const;
    void setHasMacroDefinition(bool Val);

    //  tok::TokenKind getTokenID() const { return (tok::TokenKind)TokenID; }
    //  void setTokenID(tok::TokenKind ID) { TokenID = ID; }

    //  tok::PPKeywordKind getPPKeywordID() const;

    bool isExtensionToken() const;
    void setIsExtensionToken(bool Val);

    void setIsPoisoned(bool Value = true);
    bool isPoisoned() const;

    void setIsCPlusPlusOperatorKeyword(bool Val = true);
    bool isCPlusPlusOperatorKeyword() const;

    template<typename T>
    std::weak_ptr<T> getFETokenInfo() const { return std::reinterpret_pointer_cast<T>(FETokenInfo); }
    void setFETokenInfo(std::shared_ptr<void> T) { FETokenInfo = T; }

    bool isHandleIdentifierCase() const { return NeedsHandleIdentifier; }

private:
    void RecomputeNeedsHandleIdentifier();
};

#endif // IDENTIFIERTABLE_H
