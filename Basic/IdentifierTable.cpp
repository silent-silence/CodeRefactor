#include "IdentifierTable.h"

const char *IdentifierInfo::getName() const {
    //    if (Entry) return Entry->getKeyData();

    //    return ((std::pair<IdentifierInfo, const char*>*) this)->second;
}

unsigned IdentifierInfo::getLength() const {
    //    if (Entry) return Entry->getKeyLength();
    //    const char* p = ((std::pair<IdentifierInfo, const char*>*) this)->second-2;
    //    return (((unsigned) p[0])
    //            | (((unsigned) p[1]) << 8)) - 1;
}

bool IdentifierInfo::hasMacroDefinition() const
{
    return HasMacro;
}

void IdentifierInfo::setHasMacroDefinition(bool Val)
{
    if (HasMacro == Val) return;

    HasMacro = Val;
    if (Val)
        NeedsHandleIdentifier = 1;
    else
        RecomputeNeedsHandleIdentifier();
}

bool IdentifierInfo::isExtensionToken() const
{
    return IsExtension;
}

void IdentifierInfo::setIsExtensionToken(bool Val)
{
    IsExtension = Val;
    if (Val)
        NeedsHandleIdentifier = 1;
    else
        RecomputeNeedsHandleIdentifier();
}

void IdentifierInfo::setIsPoisoned(bool Value)
{
    IsPoisoned = Value;
    if (Value)
        NeedsHandleIdentifier = 1;
    else
        RecomputeNeedsHandleIdentifier();
}

bool IdentifierInfo::isPoisoned() const
{
    return IsPoisoned;
}

void IdentifierInfo::setIsCPlusPlusOperatorKeyword(bool Val)
{
    IsCPPOperatorKeyword = Val;
    if (Val)
        NeedsHandleIdentifier = 1;
    else
        RecomputeNeedsHandleIdentifier();
}

bool IdentifierInfo::isCPlusPlusOperatorKeyword() const
{
    return IsCPPOperatorKeyword;
}

void IdentifierInfo::RecomputeNeedsHandleIdentifier()
{
    NeedsHandleIdentifier =
            (isPoisoned() | hasMacroDefinition() | isCPlusPlusOperatorKeyword() |
             isExtensionToken());
}
