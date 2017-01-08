void calculate(const H5Traits::dimension_type & dims, H5Traits::dimension_type & cdims, const size_t MAX_CHUNK_LEN)
{
    cdims.resize(dims.size());
    const int val = static_cast<int>(log(MAX_CHUNK_LEN) / (dims.size() * log(2)));
    const hsize_t N = static_cast<hsize_t>(pow(2, val));
    for (size_t idx = 0; idx < dims.size(); idx ++)
        cdims[idx] = std::min(N, dims[idx]);
}


bool checkDataType(const H5::DataSet & dataset, const H5::DataType & dataType)
{
    return (dataset.getDataType() == dataType);
}


template <typename Container>
typename Container::value_type product(Container & data)
{
    typename Container::value_type prod = 1;
    for (auto val: data)
    {
        prod = prod * val;
    }
    return prod;
}

  
/**
 * Trait classes which return the HDF5 data type for built-in data types including double
 * float, long, unsigned long, int, unsigned int, short, unsigned short, char, unsigned char.
 */ 
template<> H5::DataType const & H5DataTypeTrait<long double>::dataType(H5::PredType::NATIVE_LDOUBLE);
template<> H5::DataType const & H5DataTypeTrait<double>::dataType(H5::PredType::NATIVE_DOUBLE);
template<> H5::DataType const & H5DataTypeTrait<float>::dataType(H5::PredType::NATIVE_FLOAT);
template<> H5::DataType const & H5DataTypeTrait<long>::dataType(H5::PredType::NATIVE_LONG);
template<> H5::DataType const & H5DataTypeTrait<unsigned long>::dataType(H5::PredType::NATIVE_ULONG);
template<> H5::DataType const & H5DataTypeTrait<int>::dataType(H5::PredType::NATIVE_INT);
template<> H5::DataType const & H5DataTypeTrait<unsigned int>::dataType(H5::PredType::NATIVE_UINT);
template<> H5::DataType const & H5DataTypeTrait<short>::dataType(H5::PredType::NATIVE_SHORT);
template<> H5::DataType const & H5DataTypeTrait<unsigned short>::dataType(H5::PredType::NATIVE_USHORT);
template<> H5::DataType const & H5DataTypeTrait<char>::dataType(H5::PredType::NATIVE_CHAR);
template<> H5::DataType const & H5DataTypeTrait<unsigned char>::dataType(H5::PredType::NATIVE_UCHAR);

  
void parseDataSetNames(const std::string & str, std::vector < std::string > & gNames) {
    size_t start = 0;
    size_t pos;
    while (true) 
    {
        pos = str.find("/", start);
        if (pos != std::string::npos) 
        {
            if (start != pos) 
            {
                gNames.push_back(str.substr(start, pos - start));
                
            } 
            start = pos + 1;
        }
        else 
        {
            gNames.push_back(str.substr(start));
            break;
        }                        
    }
}

const size_t H5Traits::MAX_CHUNK_LEN = 1048576;
