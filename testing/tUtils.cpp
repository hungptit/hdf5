#include <iostream>
#include <string>
#include <vector>
#include "H5Cpp.h"
#include "hdf5/H5Interface.hpp"
#include "Utilities.hpp"
#include "gtest/gtest.h"

bool testParseGroupNames() 
{
    bool flag = true;

    {
        std::string dataSetName = "/aa//bbb/cccc/////ddddd/ef";
        std::vector< std::string > names;
        std::vector< std::string > expectedNames;
        expectedNames.push_back("aa");
        expectedNames.push_back("bbb");
        expectedNames.push_back("cccc");
        expectedNames.push_back("ddddd");
        expectedNames.push_back("ef");
        H5Interface::parseDataSetNames(dataSetName, names);
        flag &= (names == expectedNames);
        Utilities::disp(names, "Names");
        Utilities::disp(expectedNames, "Expected names");
    }

    return flag;
}


template <typename T>
T testProduct()
{
    std::vector<T> data = {1,2,3,4,5,6};
    return H5Interface::product(data);
}

TEST(ReadWriteString, Positive) 
{    
    EXPECT_TRUE(testParseGroupNames());    
}


TEST(Product, Positive) 
{    
    EXPECT_TRUE(testProduct<int>() == 720);    
    EXPECT_TRUE(testProduct<float>() == 720);    
    EXPECT_TRUE(testProduct<double>() == 720);    
}
