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

class Opener;

class Driver
{
public:
    explicit Driver(Opener &opener);
    ~Driver() = default;

    int parse ();

    void error (const yy::location& l, const std::string& m);
    void error (const std::string& m);

    Scanner &getSacnner();
    Opener &getOpener();
    std::string &getOpenedFrom() const;

private:
	void scan_begin ();

	bool trace_parsing;
	bool trace_scanning;
	Opener &m_opener;
	Scanner m_scanner;
};

#endif //CODEREFACTOR_DRIVER_H
