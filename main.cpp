#ifdef ENV_TEST

#include <gtest/gtest.h>

int main(int argc, char *argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

#else

#include "OpenHelper/StringStreamOpenHelper.h"
#include "Parser/Driver.h"
#include "AST/ASTContext.h"

using std::string;

int main()
{
	int a[] = {1, 2};

	a[4] = 2;
	std::cout << a[4] << typeid(a).name() << std::endl;

	StringStreamOpenHelper openHelper;
	openHelper << string("a=4;\n");
	//openHelper << string("a[1];\n");
	ASTContext context;
	Driver driver(openHelper, context);
	driver.parse();

	std::string output;
	openHelper >> output;
	std::cout << output;

	return 0;
}
#endif
