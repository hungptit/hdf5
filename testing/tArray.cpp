#include <typeinfo>
#include <algorithm>
#include <vector>
#include "hdf5/H5Interface.hpp"
#include "Utilities.hpp"
#include "boost/filesystem.hpp"
#include "utils/Utils.hpp"
#include "gtest/gtest.h"

using boost::filesystem::path;

template <typename T>
bool testReadWrite(const path &dataFile, const int rows,
                   const int cols, const int compressionLevel) {
    bool isOK = true;
    H5Interface::H5Traits::dimension_type dims;
    dims.push_back(rows);
    dims.push_back(cols);
    H5Interface::H5Traits::dimension_type cdims(dims.begin(), dims.end());

    std::string dataSetName =
        "/data/real/" + Utilities::getTypeString<T>() + "_dataset";

    size_t buflen = rows * cols;
    std::vector<T> data;
    data.resize(buflen);
    {
        auto idx = 0;
        for (auto &val : data) {
            val = idx++;
        }
    }

    {
        H5Interface::Writer h5Writer(dataFile.string());
        h5Writer.write(dataSetName, &data[0], dims, cdims, compressionLevel);
    }

    T *rdata = NULL;

    {
        H5Interface::H5Traits::dimension_type rdims;
        H5Interface::Reader h5Reader(dataFile.string());
        h5Reader.read<T>(dataSetName, &rdata, rdims);
        isOK &= std::equal(data.begin(), data.end(), rdata);
        isOK &= std::equal(dims.begin(), dims.end(), rdims.begin());
    }

    delete[] rdata;
    return isOK;
}

template <typename T>
void testRead(const path &dataFile, const std::string &dataSetName) {
    H5Interface::H5Traits::dimension_type rdims;
    H5Interface::Reader reader(dataFile.string());
    std::vector<T> rdata;
    reader.read<T>(dataSetName, rdata, rdims);
}

template <typename T>
bool testWrite(const path &dataFile,
               const std::string &dataSetName) {
    bool isOK = true;
    size_t rows = 3;
    size_t cols = 3;
    size_t buflen = rows * cols;
    std::vector<T> data;
    data.resize(buflen);
    {
        auto idx = 0;
        for (auto &val : data) {
            val = ++idx;
        }
    }

    H5Interface::H5Traits::dimension_type dims;
    dims.push_back(rows);
    dims.push_back(cols);
    H5Interface::H5Traits::dimension_type cdims(dims.begin(), dims.end());
    H5Interface::Writer h5Writer(dataFile.string());

    h5Writer.write(dataSetName, &data[0], dims, cdims);
    return isOK;
}

TEST(ReadWriteArray, Positive) {
    Tools::TemporaryDirectory tmpDir;

    path dataFile = tmpDir.getPath() / path("tArray.h5");
    EXPECT_TRUE(testReadWrite<double>(dataFile, 7, 9, 9));
    EXPECT_TRUE(testReadWrite<float>(dataFile, 7, 9, 9));

    EXPECT_TRUE(testReadWrite<long>(dataFile, 7, 29, 9));
    EXPECT_TRUE(testReadWrite<size_t>(dataFile, 7, 9, 9));

    EXPECT_TRUE(testReadWrite<int>(dataFile, 7, 19, 9));
    EXPECT_TRUE(testReadWrite<unsigned int>(dataFile, 7, 9, 9));

    EXPECT_TRUE(testReadWrite<short>(dataFile, 27, 9, 9));
    EXPECT_TRUE(testReadWrite<unsigned short>(dataFile, 7, 9, 9));

    EXPECT_TRUE(testReadWrite<char>(dataFile, 17, 9, 9));
    EXPECT_TRUE(testReadWrite<unsigned char>(dataFile, 7, 9, 9));
}

TEST(ReadWriteArray, Negative) {
    Tools::TemporaryDirectory tmpDir;
    // Cannot write to an existing dataset
    {
        path dataFile = tmpDir.getPath() / path("tArray.h5");
        EXPECT_TRUE(testReadWrite<double>(dataFile, 7, 9, 9));
        ASSERT_THROW((testReadWrite<double>(dataFile, 1253, 857, 9)),
                     H5::FileIException);
    }

    // Write to a read only folder
    {
        path dataFile = path("/tArray.h5"); // TODO: This works for Linux/Mac only. How to make it works in Windows?
        ASSERT_THROW((testWrite<double>(dataFile, "/test/group")),
                     H5::FileIException);
    }

    // Open a non-exist file to read
    {
        path dataFile = tmpDir.getPath() / path("tArray1.h5");
        ASSERT_THROW((testRead<double>(dataFile, "///test/group")),
                     H5::FileIException);
    }
}
