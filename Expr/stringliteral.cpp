#include "stringliteral.h"

std::shared_ptr<StringLiteral> StringLiteral::Create(const char *StrData,
                                                     unsigned ByteLength,
                                                     bool Wide,
                                                     QualType Ty,
                                                     unsigned NumStrs)
{

}

std::shared_ptr<StringLiteral> StringLiteral::Create(const char *StrData,
                                                     unsigned ByteLength,
                                                     bool Wide,
                                                     QualType Ty)
{
    return Create(StrData, ByteLength, Wide, Ty, 1);
}

StringLiteral::StringLiteral(QualType Ty)
    : Expr(StringLiteralClass, Ty)
{}
