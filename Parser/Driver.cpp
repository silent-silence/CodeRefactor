//
// Created by gaojian on 19-4-6.
//

#include <Opener/Opener.h>
#include "Driver.h"

Driver::Driver(Opener &opener)
	: m_opener{opener}, trace_scanning{false}, trace_parsing{false}
{}

int Driver::parse()
{
	scan_begin();
	yy::Parser parser(*this);
	parser.set_debug_level(trace_parsing);
	int res = parser.parse();
	return res;
}

void Driver::error(const yy::location& l, const std::string& m)
{
	std::cerr << l << ": " << m << std::endl;
}

void Driver::error(const std::string& m)
{
	std::cerr << m << std::endl;
}

Scanner& Driver::getSacnner()
{
	return m_scanner;
}

Opener& Driver::getOpener()
{
	return m_opener;
}

std::string &Driver::getOpenedFrom() const
{
	return m_opener.getOpenedName();
}

void Driver::scan_begin()
{
	m_scanner.set_debug(trace_scanning);
	m_scanner.switch_streams(&m_opener.getInputStream(), &m_opener.getOutputStream());
}
