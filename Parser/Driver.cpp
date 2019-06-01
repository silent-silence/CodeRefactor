//
// Created by gaojian on 19-4-6.
//

#include "OpenHelper/OpenHelper.h"
#include "Driver.h"
#include "YaccAdapter.h"
#include "Basic/SourceLocation.h"
#include "AST/ASTContext.h"

Driver::Driver(OpenHelper &opener, YaccAdapter &adapter)
	: m_opener{opener}, trace_scanning{false}, trace_parsing{false}, m_adapter{adapter}
{}

int Driver::parse()
{
	scan_begin();
	yy::Parser parser(*this);
	parser.set_debug_level(trace_parsing);
	int res = parser.parse();
	m_adapter.parseDone();
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

OpenHelper& Driver::getOpenHelper()
{
	return m_opener;
}

YaccAdapter& Driver::getAdapter()
{
	return m_adapter;
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