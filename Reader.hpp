/**
 * @file   Reader.hpp
 * @author  <hungptit@gmail.com>
 * @date   Thu Jul 12 00:09:08 2012
 *
 */
#ifndef H5READER_HPP_
#define H5READER_HPP_

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "H5Cpp.h"
#include "H5Utils.hpp"

namespace H5Interface {
    class Reader {
      public:
        typedef H5Traits::dimension_type dimension_type;

        Reader(std::string fileName);
        ~Reader();

        // Read a dataset to a C array
        template <typename T>
        void read(const std::string &dataSetName, T **data,
                  H5Traits::dimension_type &dims);

        // Read a dataset and its dimensions to a std::vector
        template <typename T>
        void read(const std::string &dataSetName, std::vector<T> &dat,
                  H5Traits::dimension_type &dataDimentions);

        // Read a dataset to a std::vector
        template <typename T>
        void read(const std::string &dataSetName, std::vector<T> &dat);

        // Read a string dataset to a std::string
        template <typename T> T read(const std::string &dataSetName);

      protected:
        std::string FileName;
        std::string DataSetName;

        template <typename T>
        H5::DataSet openDataset(const std::string &dataSetName,
                                H5Traits::dimension_type &dims);

      private:
        H5::H5File *FileID;
    };
#include "private/Reader.cpp"
}
#endif
