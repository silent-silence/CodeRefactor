#include "IdentifierTable.h"

using std::to_string;

unsigned IdentifierInfo::anonymousNameId = 0;

IdentifierInfo::IdentifierInfo()
	: IdentifierInfo("%" + to_string(++anonymousNameId))
{}

IdentifierInfo::IdentifierInfo(std::string name)
	: m_entry{name}, HasMacro{false}
{}

bool IdentifierInfo::isStr(const std::string name) const
{
	return m_entry == name;
}

const std::string IdentifierInfo::getName() const
{
	return m_entry;
}

/*unsigned IdentifierInfo::getLength() const {
    //    if (Entry) return Entry->getKeyLength();
    //    const char* p = ((std::pair<IdentifierInfo, const char*>*) this)->second-2;
    //    return (((unsigned) p[0])
    //            | (((unsigned) p[1]) << 8)) - 1;
}*/

bool IdentifierInfo::hasMacroDefinition() const
{
    return HasMacro;
}

void IdentifierInfo::setHasMacroDefinition(bool Val)
{
    HasMacro = Val;
}

bool IdentifierInfo::isAnonymous() const
{
	return m_entry[0] == '%';
}

/*bool IdentifierInfo::isExtensionToken() const
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
}*/
