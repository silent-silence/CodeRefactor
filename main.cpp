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
	int a[] = {1, 2};

	a[4] = 2;
	std::cout << a[4] << typeid(a).name() << std::endl;

	StringOpener opener;
	opener << string("a=4;\n");
	//opener << string("a[1];\n");
	Driver driver(opener);
	driver.parse();

	std::string output;
	opener >> output;
	std::cout << output;

	return 0;
}
#endif
