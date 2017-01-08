/**
 * @file   EigenWriter.hpp
 * @author  <hungptit@gmail.com>
 * @date   Tue Oct  2 23:05:04 2012
 * 
 * @brief  
 * 
 * 
 */
#ifndef EIGENWRITER_HPP_
#define EIGENWRITER_HPP_

#include <algorithm>
#include <Eigen/Dense>
#include "Writer.hpp"

namespace H5Interface {
    class EigenWriter : public Writer {
      public:
        EigenWriter (const std::string & fileName);
      
        template<class _Matrix>
        void write(const std::string & dataSetName,
                   const _Matrix & mat,
                   const H5Traits::dimension_type & chunkDimensions,
                   const int compressionLevel = BEST_COMPRESSION);
        
        
        template<class _Matrix>
        void write(const std::string & dataSetName,
                   const _Matrix & mat,
                   const int compressionLevel = BEST_COMPRESSION);
    };

#include "private/EigenWriter.cpp"
}

#endif
