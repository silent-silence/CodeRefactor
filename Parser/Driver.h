//
// Created by gaojian on 19-4-6.
//

#ifndef CODEREFACTOR_DRIVER_H
#define CODEREFACTOR_DRIVER_H

#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include "parser.hpp"
#include "Scanner.h"

class OpenHelper;
class YaccAdapter;

/// @brief To drive bison and flex
class Driver
{
public:
    Driver(OpenHelper &opener, YaccAdapter &adapter);
    ~Driver() = default;

    /// @brief Start parse
    int parse ();

    /// @brief Called on an error occurred while parsing
    void error (const yy::location& l, const std::string& m);
    void error (const std::string& m);

    Scanner &getSacnner();
    OpenHelper &getOpenHelper();
    YaccAdapter &getAdapter();
    std::string &getOpenedFrom() const;

    /// @brief Look for type name existence
	yy::Parser::symbol_type identifier_or_type_name(std::string id, yy::Parser::location_type loc);

private:
	/// @brief Start scan
	void scan_begin();

	YaccAdapter &m_adapter;

	bool trace_parsing;
	bool trace_scanning;
	OpenHelper &m_opener;
	Scanner m_scanner;
};

#endif //CODEREFACTOR_DRIVER_H
