//
// Created by gaojian on 19-4-6.
//

#include <iostream>
#include "StdioOpenHelper.h"

StdioOpenHelper::StdioOpenHelper()
	: OpenHelper(std::cin, std::cout, "stdio")
{}
