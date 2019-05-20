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
    unsigned int line() const;
    unsigned int bytes() const;

private:
    std::string m_file;
    unsigned int m_line;
    unsigned int m_bytes;
};

#endif // SOURCELOCATION_H
