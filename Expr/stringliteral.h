#ifndef STRINGLITERAL_H
#define STRINGLITERAL_H

#include "expr.h"

class StringLiteral : public Expr
{
public:
    static std::shared_ptr<StringLiteral> Create(const char *StrData,
                                                 unsigned ByteLength,
                                                 bool Wide,
                                                 QualType Ty,
                                                 unsigned NumStrs);
    static std::shared_ptr<StringLiteral> Create(const char *StrData,
                                                 unsigned ByteLength,
                                                 bool Wide,
                                                 QualType Ty);
private:
    StringLiteral(QualType Ty);

    const char *StrData;
    unsigned ByteLength;
    bool IsWide;
    unsigned NumConcatenated;
};

#endif // STRINGLITERAL_H
