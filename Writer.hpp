/**
 * @file   H5Writer.hpp
 * @author  <hungptit@gmail.com>
 * @date   Thu Jul 12 00:08:21 2012
 *
 * @todo Support complex data type
 */

#ifndef WRITER_HPP_
#define WRITER_HPP_

#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm>

#include "H5Utils.hpp"

namespace H5Interface
{

    class Writer
    {
      public:

        Writer(const std::string & fileName);
        ~Writer();

        // Public APIs
        void write(const std::string & dataSetName,
                   const std::string & strBuf);

        template <class T>
        void write(const std::string & dataSetName,
                   const std::vector<T> & data);

        template <typename T>
        void write(const std::string & dataSetName,
                   const T *data,
                   const H5Traits::dimension_type & dims,
                   const H5Traits::dimension_type & cdims,
                   const int compressionLevel = BEST_COMPRESSION);

        template <class T>
        void write(const std::string & dataSetName,
                   const std::vector<T> & data,
                   const H5Traits::dimension_type & dims,
                   const H5Traits::dimension_type & cdims,
                   const int compressionLevel = BEST_COMPRESSION);

      private:
        // Private typedef
        typedef std::vector<H5::Group*> GroupContainer;
        typedef std::vector<std::string> GroupNameContainer;

        // Private data
        std::string FileName;
        std::vector<std::string> GroupNames;

        H5::H5File * FileID;
        std::vector<H5::Group*> Groups;

        // Private methods
        void openFile();
        void closeFile();
        void openGroups(std::vector< std::string > & groupNames);
        void openAGroup(const std::string & groupName);
        void closeGroups();
        void cleanUp();
    };
#include "private/Writer.cpp"
}
#endif
