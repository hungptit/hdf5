EigenReader::EigenReader(const std::string & fileName) : Reader(fileName) 
{
  // Do nothing
}       

template<typename T, int N, int K>
void EigenReader::read(const std::string & dataSetName, Eigen::Matrix<T, N, K> & mat)
{
  H5Traits::dimension_type dataDimentions;
  H5::DataSet dataset = openDataset<T>(dataSetName, dataDimentions);
  resize< typename Eigen::Matrix<T, N, K> >(mat, dataDimentions);
  H5::DataSpace memSpace(dataDimentions.size(), &dataDimentions[0]);
  dataset.read(mat.data(), dataset.getDataType(), memSpace, dataset.getSpace());
}


template<typename Matrix>
void EigenReader::resize(Matrix & mat, const H5Traits::dimension_type & dims)
{
  switch(dims.size())
    {
    case 1:
      if (mat.rows() == Eigen::Dynamic)
	mat.resize(dims[0], 1);
      else
	mat.resize(1, dims[0]);
      break;
    case 2:
      mat.resize(dims[1], dims[0]);
      break;
    default:
      const std::string errorMessage = "Cannot read a multidimensional matrix from dataset \"" + 
          DataSetName + " \" in file \"" + 
          FileName + "\" into an Eigen matrix.";
      throw std::runtime_error(errorMessage);
      break;
    }
}
