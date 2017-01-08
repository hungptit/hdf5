EigenWriter::EigenWriter (const std::string & fileName) : Writer(fileName) {}

template<class _Matrix>
void EigenWriter::write(const std::string & dataSetName,
			const _Matrix & mat,
			const H5Traits::dimension_type & cdims,
                        const int compressionLevel)
{
  H5Traits::dimension_type dims;
  dims.resize(2);
  dims[0] = mat.cols();
  dims[1] = mat.rows();
  write(dataSetName, mat.data(), dims, cdims, compressionLevel);
}


template<class _Matrix>
void EigenWriter::write(const std::string & dataSetName,
			const _Matrix & mat,
			const int compressionLevel)
{
  H5Traits::dimension_type dims;
  dims.resize(2);
  dims[0] = mat.cols();
  dims[1] = mat.rows();

  // Compute the optimum chunk dimensions
  H5Traits::dimension_type cdims;
  calculate(dims, cdims);    
  Writer::write(dataSetName, mat.data(), dims, cdims, compressionLevel);
}
