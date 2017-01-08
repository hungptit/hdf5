/**
 * @file   EigenReader.hpp
 * @author  <hungptit@gmail.com>
 * @date   Tue Oct  2 23:04:30 2012
 *
 */
#ifndef EIGENREADER_HPP_
#define EIGENREADER_HPP_

#include "Reader.hpp"
#include "Eigen/Dense"

namespace H5Interface 
{
  class EigenReader : public Reader 
  {
  public:
    EigenReader(const std::string & fileName);
    
    template<typename T, int N, int K>
    void read(const std::string & dataSetName, Eigen::Matrix<T, N, K> & mat);    
      
  private:      
    template<typename Matrix> 
    void resize(Matrix & mat, const H5Traits::dimension_type & dataDimentions);
  };
#include "private/EigenReader.cpp"
}
#endif
