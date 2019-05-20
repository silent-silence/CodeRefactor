#ifndef SOURCELOCATION_H
#define SOURCELOCATION_H

#include <string>

class SourceLocation
{
public:
    SourceLocation();
private:
    std::string m_file;
    unsigned int m_line;
    unsigned int m_bytes;
};

#endif // SOURCELOCATION_H
