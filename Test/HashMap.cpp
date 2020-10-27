#include "../src/HashMap.h"

#include <iostream>
#include <string>
#include "gtest/gtest.h"

TEST(HashMap, ConstructorTest){
    HashMap< std::string, int> map1;
    HashMap< std::string, int> map2(10);
}

