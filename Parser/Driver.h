//
// Created by gaojian on 19-4-6.
//

#ifndef CODEREFACTOR_DRIVER_H
#define CODEREFACTOR_DRIVER_H

#include <fstream>
#include <string>
#include <map>
#include "parser.hpp"
#include "Scanner.h"

class Driver
{
public:
    Driver();
    ~Driver() = default;
    
    //std::map<std::string, int> variables;
    
    int result;
    
    // CHANGE: add lexer object as a member
    Scanner scanner;

    // CHANGE: add ifstream object as a member
    std::ifstream instream;

    // Handling the scanner.
    void scan_begin ();
    void scan_end ();
    bool trace_scanning;
    
    // Run the parser on file F.
    // Return 0 on success.
    int parse (const std::string& f);
    // The name of the file being parsed.
    // Used later to pass the file name to the location tracker.
    std::string file;
    // Whether parser traces should be generated.
    bool trace_parsing;

    // Error handling.
    void error (const yy::location& l, const std::string& m);
    void error (const std::string& m);
};

#endif //CODEREFACTOR_DRIVER_H
