#ifndef SOURCELOCATION_H
#define SOURCELOCATION_H

#include <string>

/// @brief Represent the location（file name, lines, rows) of a token in code
class SourceLocation
{
public:
    SourceLocation();
    SourceLocation(std::string fileName, unsigned line, unsigned byte);

    bool operator>(const SourceLocation &sourceLocation) const;
    bool operator<(const SourceLocation &sourceLocation) const;
    bool operator==(const SourceLocation &sourceLocation) const;

    std::string file() const;
    unsigned int line() const;
    unsigned int bytes() const;

private:
    std::string m_file;
    unsigned int m_line;
    unsigned int m_bytes;
};

#endif // SOURCELOCATION_H
