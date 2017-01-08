#ifndef H5UTILS_HPP_
#define H5UTILS_HPP_

#include "H5Cpp.h"
#include <cmath>

namespace H5Interface
{
    enum COMPRESSIONLEVEL {
        NO_COMPRESSION = 0,
        BEST_SPEED = 1,
        BEST_COMPRESSION = 9,
        DEFAULT_COMPRESSION = -1
    };


    template <typename T>
    struct H5DataTypeTrait
    {
        static H5::DataType const & dataType;
    };


    struct H5Traits
    {
        typedef std::vector<hsize_t> dimension_type;
        static const size_t MAX_CHUNK_LEN;
    };

    void calculate(const H5Traits::dimension_type & dims,
                   H5Traits::dimension_type & cdims,
                   const size_t MAX_CHUNK_LEN = H5Traits::MAX_CHUNK_LEN);

    void parseDataSetNames(const std::string & str, std::vector < std::string > & gNames);
    
    bool checkDataType(const H5::DataSet & dataset, const H5::DataType & dataType);
    
    template <typename InputIterator, typename T>
    T product (InputIterator first, InputIterator last, const T init);

#include "private/H5Utils.cpp"
}
#endif
