#include <iostream>
#include <algorithm>
#include <vector>
#include "hdf5/H5Interface.hpp"
#include "Utilities.hpp"
#include "gtest/gtest.h"
#include "utils/Utils.hpp"

using boost::filesystem::path;

template <typename T>
inline bool testReadWrite(const path &dataFile, const int rows,
                          const int cols) {
    std::string dataSetName("/testdata/matrix/data_");
    dataSetName += Utilities::getTypeString<T>();

    std::vector<T> wdata;
    H5Interface::H5Traits::dimension_type wdims(2);
    H5Interface::H5Traits::dimension_type wcdims(2);

    wdims[0] = cols;
    wdims[1] = rows;

    wcdims = wdims;

    size_t buflen = rows * cols;

    wdata.resize(buflen);

    {
        size_t idx = 0;
        for (auto &val : wdata) {
            val = ++idx;
        }
    }

    {
        H5Interface::Writer h5Writer(dataFile.string());
        h5Writer.write(dataSetName, wdata, wdims, wcdims);
    }

    std::vector<T> rdata1;
    std::vector<T> rdata2;
    H5Interface::H5Traits::dimension_type rdims(2);

    {
        H5Interface::Reader h5Reader(dataFile.string());
        h5Reader.read(dataSetName, rdata1, rdims);
        h5Reader.read(dataSetName, rdata2);
    }

    Utilities::disp(wdims, "Write data dimensions");
    Utilities::disp(rdims, "Read data dimensions");

    return (wdata == rdata1) && (wdata == rdata2) && (wdims == rdims);
}

template <typename T>
inline bool testReadWriteVector(const path &dataFile, const int buflen) {
    path dataSetName("/testdata/vector/data_");
    dataSetName += Utilities::getTypeString<T>();

    std::vector<T> wdata;

    wdata.resize(buflen);

    {
        size_t idx = 0;
        for (auto &val : wdata) {
            val = ++idx;
        }
    }

    {
        H5Interface::Writer h5Writer(dataFile.string());
        h5Writer.write(dataSetName, wdata);
    }

    std::vector<T> rdata;
    {
        H5Interface::Reader h5Reader(dataFile.string());
        h5Reader.read(dataSetName, rdata);
    }

    return (wdata == rdata);
}

TEST(ReadWriteVector, Positive) {
    Tools::TemporaryDirectory tmpDir;
    path dataFile = tmpDir.getPath() / path("/tString.h5");

    EXPECT_TRUE((testReadWrite<double>(dataFile, 12, 857)));
    EXPECT_TRUE((testReadWrite<float>(dataFile, 9, 7)));
    EXPECT_TRUE((testReadWrite<size_t>(dataFile, 3, 8)));
    EXPECT_TRUE((testReadWrite<long>(dataFile, 8, 5)));
    EXPECT_TRUE((testReadWrite<int>(dataFile, 5, 6)));
    EXPECT_TRUE((testReadWrite<unsigned int>(dataFile, 12, 15)));
    EXPECT_TRUE((testReadWrite<short>(dataFile, 7, 13)));
    EXPECT_TRUE((testReadWrite<unsigned short>(dataFile, 21, 57)));
    EXPECT_TRUE((testReadWrite<char>(dataFile, 131, 157)));
    EXPECT_TRUE((testReadWrite<unsigned char>(dataFile, 1234, 567)));
}

TEST(ReadWriteVector, Negative) {
    Tools::TemporaryDirectory tmpDir;
    path dataFile = tmpDir.getPath() / path("/tString.h5");

    EXPECT_TRUE((testReadWrite<double>(dataFile, 12, 857)));
    ASSERT_THROW((testReadWrite<double>(dataFile, 1253, 857)),
                 H5::FileIException);
}
