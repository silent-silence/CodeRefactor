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
    shared_ptr<CodeRefactor> ptr=nullptr;
    switch (argc) {
    case 1:
        ptr = make_shared<CodeRefactor>();
        ptr->run();
        ptr->show();
        break;
    case 2:
        ptr = make_shared<CodeRefactor>(argv[1]);
        ptr->run();
        ptr->show();
        break;
    case 3:
        ptr = make_shared<CodeRefactor>();
        ptr->run();
        ptr->show(argv[2]);
        break;
    case 4:
        if(!string(argv[1]).compare("-o")){
            ptr = make_shared<CodeRefactor>(argv[3]);
            ptr->run();
            ptr->show(argv[2]);
        }
        else if(!string(argv[2]).compare("-o")){
            ptr = make_shared<CodeRefactor>(argv[1]);
            ptr->run();
            ptr->show(argv[3]);
        }
        else{

        }
        break;
    default:break;
    }
	return 0;
}
#endif
