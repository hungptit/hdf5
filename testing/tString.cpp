#include <iostream>
#include <algorithm>
#include <vector>
#include "hdf5/H5Interface.hpp"
#include "Utilities.hpp"
#include "gtest/gtest.h"
#include "utils/Utils.hpp"
using boost::filesystem::path;

inline bool testReadWrite(const path & dataFile){
    bool flag = true;
    std::string dataSetName("/testdata/data/string");
    std::string writeBuf("This is a test string. dfsdgfsdhgfdhg2112121541311313");

    {
        H5Interface::Writer h5Writer(dataFile.string());
        h5Writer.write(dataSetName, writeBuf);
        std::cout << "writeBuf: " << writeBuf << std::endl;
    }

    {
        H5Interface::Reader h5Reader(dataFile.string());
        std::string readBuf = h5Reader.read<std::string>(dataSetName);
        std::cout << "readBuf: " << readBuf << std::endl;
        flag &= (readBuf == writeBuf);
    }

    return flag;
}

TEST(ReadWriteString, Positive) 
{
    Tools::TemporaryDirectory tmpDir;
    path dataFile = tmpDir.getPath() / path("/tString.h5");
    EXPECT_TRUE(testReadWrite(dataFile));    
}

