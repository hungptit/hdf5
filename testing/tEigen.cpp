#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cstdlib>

#include "hdf5/Writer.hpp"
#include "hdf5/Reader.hpp"
#include "Utilities.hpp"
#include "gtest/gtest.h"

#include "Eigen/Dense"
#include "Eigen/Core"
#include "hdf5/EigenReader.hpp"
#include "hdf5/EigenWriter.hpp"
#include "utils/Utils.hpp"

using boost::filesystem::path;

template <typename Matrix>
void createTestMatrix(Matrix & mat)
{
  for (int row = 0 ; row < mat.rows(); row ++){
    for (int col = 0 ; col < mat.cols() ; col ++){
      mat(row, col) = (row + 1) * (col + 1) / 2;
    }
  }
}


template<typename T, int rows, int cols>
bool unitTest(const path & dataFile)
{
  bool flag = true;

  std::string dataSetName("/data/unit_test/Eigen/data_case4_");
  dataSetName += Utilities::getTypeString<T>();

  Eigen::Matrix<T, rows, cols> mat(rows,cols);
  createTestMatrix(mat);
  std::cout << mat << std::endl;

  // Write the test data
  {
    H5Interface::EigenWriter writer(dataFile.string());
    writer.write(dataSetName, mat);
  }

  // Read back the written data
  {
    typedef Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> Matrix;
    Matrix x;
    H5Interface::EigenReader reader(dataFile.string());
    reader.read<T, Eigen::Dynamic, Eigen::Dynamic>(dataSetName, x);
    std::cout << x << std::endl;
    flag &= (mat == x);
  }

  // Read back the written data
  {
    typedef Eigen::Matrix<T, rows, Eigen::Dynamic> Matrix;
    Matrix x;
    H5Interface::EigenReader reader(dataFile.string());
    reader.read<T, rows, Eigen::Dynamic>(dataSetName, x);
    std::cout << x << std::endl;
    flag &= (mat == x);
  }

  // Read back the written data
  {
    typedef Eigen::Matrix<T, Eigen::Dynamic, cols> Matrix;
    Matrix x;
    H5Interface::EigenReader reader(dataFile.string());
    reader.read<T, Eigen::Dynamic, cols>(dataSetName, x);
    std::cout << x << std::endl;
    flag &= (mat == x);
  }

  // Read back the written data
  {
    typedef Eigen::Matrix<T, rows, cols> Matrix;
    Matrix x;
    H5Interface::EigenReader reader(dataFile.string());
    reader.read<T, rows, cols>(dataSetName, x);
    std::cout << x << std::endl;
     flag &= (mat == x);
  }

  return flag;
}


TEST(ReadWriteEigen, Positive) 
{
    Tools::TemporaryDirectory tmpDir;
    path dataFile = tmpDir.getPath() / path("tEigen.h5");     

    EXPECT_TRUE((unitTest<double, 15, 7>(dataFile)));
    EXPECT_TRUE((unitTest<float, 5, 7>(dataFile)));
    EXPECT_TRUE((unitTest<size_t, 51, 7>(dataFile)));
    EXPECT_TRUE((unitTest<long, 3, 72>(dataFile)));
    EXPECT_TRUE((unitTest<int, 3, 43>(dataFile)));
    EXPECT_TRUE((unitTest<unsigned int, 5, 3>(dataFile)));
    EXPECT_TRUE((unitTest<short, 6, 18> (dataFile)));
    EXPECT_TRUE((unitTest<unsigned short, 4, 6>(dataFile)));
    EXPECT_TRUE((unitTest<char, 9, 41> (dataFile)));
    EXPECT_TRUE((unitTest<unsigned char, 17, 3> (dataFile))); 
}


TEST(ReadWriteEigen, Negative) 
{
    path dataFile("/tEigen.h5");     
    ASSERT_ANY_THROW((unitTest<double, 15, 7>(dataFile)));
}
