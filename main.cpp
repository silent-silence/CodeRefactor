#include <Opener/StringOpener.h>
#include "Parser/Driver.h"

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
	StringOpener opener;
	opener << string("int a;\n");
	opener << string("1 + 2;\n");
	Driver driver(opener);
	driver.parse();

	std::string output;
	opener >> output;
	std::cout << output;

	return 0;
}
#endif
