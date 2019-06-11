#ifdef ENV_TEST

#include <gtest/gtest.h>

int main(int argc, char *argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

#else

#include <string>
#include <iostream>
#include <memory>
#include <unistd.h>
#include "CodeRefactor.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::weak_ptr;
using std::shared_ptr;
using std::make_shared;

int main(int argc, char **argv)
{
    string str(argv[1]);
    auto ptr = make_shared<CodeRefactor>(str);
    ptr->run();
    ptr->show();
	return 0;
}
#endif
