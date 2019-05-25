#ifndef SOURCELOCATION_H
#define SOURCELOCATION_H

#include <string>

class SourceLocation
{
public:
    SourceLocation();
    bool operator>(const SourceLocation &sourceLocation) const;
    bool operator<(const SourceLocation &sourceLocation) const;
protected:
    std::string file() const;
    unsigned line() const;
    unsigned bytes() const;

private:
    std::string m_file;
    unsigned m_line;
    unsigned m_bytes;
};

#endif // SOURCELOCATION_H
