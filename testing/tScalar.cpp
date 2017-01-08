#include "H5Cpp.h"
#include "hdf5.h"
#include "hdf5/Writer.hpp"
#include "hdf5/H5Reader.hpp"
#include "Utilities.hpp"
#include "gtest/gtest.h"

using boost::filesystem::path

template <typename T>
bool testReadWrite(const path & dataFile.string(), const T val)
{  
    bool flag = true;
    path dataSetName = "/data/real/" + Utilities::getTypeString<T>();
  
    {
        H5Interface::Writer h5Writer(dataFile.string());
        h5Writer.write<T>(dataSetName, val);
    }
  
    {
        T rval;
        H5Interface::H5Reader h5Reader(dataFile.string());
        rval = h5Reader.read<T>(dataSetName);
        flag &= (val == rval);
    }    
    return flag;
}

TEST(ReadWriteScalar, Positive) 
{
    Tools::TemporaryDirectory tmpDir;
    path dataFile = tmpDir.getPath() / path("tScalar.h5");     

    EXPECT_TRUE((testReadWrite<double>(dataFile.string(), 1.5)));
    EXPECT_TRUE((testReadWrite<float>(dataFile.string(), 2.5)));

    EXPECT_TRUE((testReadWrite<long>(dataFile.string(), -3)));
    EXPECT_TRUE((testReadWrite<size_t>(dataFile.string(), 4)));
  
    EXPECT_TRUE((testReadWrite<int>(dataFile.string(), -1)));
    EXPECT_TRUE((testReadWrite<unsigned int>(dataFile.string(), 6)));

    EXPECT_TRUE((testReadWrite<short>(dataFile.string(), 7)));
    EXPECT_TRUE((testReadWrite<unsigned short>(dataFile.string(), 8)));

    EXPECT_TRUE((testReadWrite<char>(dataFile.string(), -9)));
    EXPECT_TRUE((testReadWrite<unsigned char>(dataFile.string(), 10)));
}
