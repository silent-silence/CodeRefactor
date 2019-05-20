#include "SourceLocation.h"

SourceLocation::SourceLocation()
{}


SourceLocation::SourceLocation(std::string fileName, unsigned line, unsigned bytes)
	: m_file{fileName}, m_line{line}, m_bytes{bytes}
{}

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

unsigned int SourceLocation::line() const
{
    return m_line;
}

unsigned int SourceLocation::bytes() const
{
    return m_bytes;
}
