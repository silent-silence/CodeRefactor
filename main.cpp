#ifdef ENV_TEST

#include <gtest/gtest.h>

int main(int argc, char *argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

#else

#include "OpenHelper/StringStreamOpenHelper.h"
#include "OpenHelper/FileOpenHelper.h"
#include "Parser/YaccAdapter.h"
#include "Decl/DeclContextHolder.h"
#include "Parser/Driver.h"
#include "AST/ASTContext.h"
#include "ASTOperation/Printer.h"

using std::string;

int main()
{
    FileOpenHelper openHelper("a.txt", "b.txt");
	ASTContext context;
	DeclContextHolder declContextHolder;
	YaccAdapter adapter(context, declContextHolder, openHelper);

	Driver driver(openHelper, adapter);
    Printer printer(openHelper);

	driver.parse();
    printer.print(context.getRoot().lock());

	return 0;
}
#endif
