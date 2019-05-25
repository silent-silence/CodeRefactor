#include "SourceLocation.h"

SourceLocation::SourceLocation()
    :m_file{""}, m_line{0}, m_bytes{0}
{
}

bool SourceLocation::operator>(const SourceLocation &sourceLocation) const
{
    if(file().compare(sourceLocation.file())){
        return false;
    }
    if(line()<sourceLocation.line()){
        return false;
    }
    if(bytes()<sourceLocation.bytes()){
        return false;
    }
    return true;
}

bool SourceLocation::operator<(const SourceLocation &sourceLocation) const
{
    if(m_file.compare(sourceLocation.file())){
        return false;
    }
    if(line()>sourceLocation.line()){
        return false;
    }
    if(bytes()>sourceLocation.bytes()){
        return false;
    }
    return true;
}

std::string SourceLocation::file() const
{
    return m_file;
}

unsigned SourceLocation::line() const
{
    return m_line;
}

unsigned SourceLocation::bytes() const
{
    return m_bytes;
}
