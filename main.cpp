#include <iostream>
#include <memory>
#include <vector>
#include <array>
#include <tuple>
#include <stdio.h>

#include "AST/AST.hpp"
#include "AST/ASTContext.h"

using std::cout;
using std::endl;
using std::array;
using std::vector;
using std::string;
using std::weak_ptr;
using std::shared_ptr;
using std::make_shared;

int main()
{
    string str;
    std::snprintf(str.data(), 3, "%s", "abcdefg");
    cout<<str<<endl;
//    shared_ptr<int> a=make_shared<int>(8);
//    auto ptr=(&a)+1;
    return 0;
}
