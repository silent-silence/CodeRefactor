#ifndef BUILTINTYPE_H
#define BUILTINTYPE_H

#include "type.h"

class BuiltinType : public Type
{
public:
    enum Kind {
        Void,

        Bool,     // This is bool and/or _Bool.
        Char_U,   // This is 'char' for targets where char is unsigned.
        UChar,    // This is explicitly qualified unsigned char.
        Char16,   // This is 'char16_t' for C++.
        Char32,   // This is 'char32_t' for C++.
        UShort,
        UInt,
        ULong,
        ULongLong,
        UInt128,  // __uint128_t

        Char_S,   // This is 'char' for targets where char is signed.
        SChar,    // This is explicitly qualified signed char.
        WChar,    // This is 'wchar_t' for C++.
        Short,
        Int,
        Long,
        LongLong,
        Int128,   // __int128_t

        Float, Double, LongDouble,

        NullPtr,  // This is the type of C++0x 'nullptr'.

        Overload,  // This represents the type of an overloaded function declaration.
        Dependent, // This represents the type of a type-dependent expression.

        UndeducedAuto, // In C++0x, this represents the type of an auto variable
        // that has not been deduced yet.
        ObjCId,    // This represents the ObjC 'id' type.
        ObjCClass  // This represents the ObjC 'Class' type.
    };
    BuiltinType();
private:
    Kind TypeKind;
};

#endif // BUILTINTYPE_H
