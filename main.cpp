#include <iostream>
#include <memory>
#include "parser.hpp"
#include "Parser/Driver.h"

using std::make_shared;				using std::shared_ptr;
using std::string;

#ifdef ENV_TEST
#include <gtest/gtest.h>
int main(int argc, char *argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
#else
int main()
{
	Driver driver;
	driver.parse("a");

	return 0;
}
#endif
