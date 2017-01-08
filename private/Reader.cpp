Reader::Reader(std::string fileName)
    : FileName(fileName), DataSetName(), FileID(NULL) {
    try {
        FileID = new H5::H5File(FileName, H5F_ACC_RDONLY);
    }

    catch (H5::FileIException error) {
        std::cerr << "Cannot open file \"" << FileName << "\" to read."
                  << std::endl;
        throw(error);
    }
}

Reader::~Reader() { delete FileID; }

template <typename T>
void Reader::read(const std::string &datasetName, T **dat,
                  H5Traits::dimension_type &dims) {
    H5::DataSet dataset = openDataset<T>(datasetName, dims);
    H5::DataSpace memSpace(dims.size(), &dims[0]);
    T *data_out = new T[product(dims)];
    dataset.read(data_out, dataset.getDataType(), memSpace, dataset.getSpace());
    *dat = data_out;
}

template <typename T>
void Reader::read(const std::string &datasetName, std::vector<T> &dat,
                  H5Traits::dimension_type &dims) {
    H5::DataSet dataset = openDataset<T>(datasetName, dims);
    H5::DataSpace memSpace(dims.size(), &dims[0]);
    dat.resize(product(dims));
    dataset.read(&dat[0], dataset.getDataType(), memSpace, dataset.getSpace());
}

template <typename T>
void Reader::read(const std::string &datasetName, std::vector<T> &dat) {
    H5Traits::dimension_type dataDimentions;
    read(datasetName, dat, dataDimentions);
}

template <typename T> T Reader::read(const std::string &datasetName) {
    T val;
    H5Traits::dimension_type dataDimentions;
    H5::DataSet dataset = openDataset<T>(datasetName, dataDimentions);
    H5::DataSpace memSpace(dataDimentions.size(), &dataDimentions[0]);
    if (product(dataDimentions) == 1)
        dataset.read(&val, dataset.getDataType(), memSpace, dataset.getSpace());
    else
        throw std::runtime_error(
            "The expected and the actual sizes of the dataset \"" +
            DataSetName + " \" in file \"" + FileName + "\" are different.");
    return val;
}

template <>
std::string Reader::read<std::string>(const std::string &datasetName) {
    std::string strBuf;
    H5Traits::dimension_type dataDimentions;
    H5::DataSet dataset = openDataset<char>(datasetName, dataDimentions);
    H5::DataSpace memSpace(dataDimentions.size(), &dataDimentions[0]);
    strBuf.resize(product(dataDimentions));
    dataset.read(&strBuf[0], dataset.getDataType(), memSpace,
                 dataset.getSpace());
    return strBuf;
}

template <typename T>
H5::DataSet Reader::openDataset(const std::string &datasetName,
                                H5Traits::dimension_type &dims) {
    try {
        DataSetName = datasetName;
        H5::DataSet dataset = FileID->openDataSet(datasetName);

        if (checkDataType(dataset, H5DataTypeTrait<T>::dataType) == false) {
            throw std::runtime_error("Type of dataset \"" + DataSetName +
                                     "\" in \"" + FileName +
                                     "\" and the expected data type do not "
                                     "match. You cannot read this dataset.");
        }

        H5::DataSpace dataSpace = dataset.getSpace();
        dims.resize(dataSpace.getSimpleExtentNdims());
        dataSpace.getSimpleExtentDims(&dims[0]);
        return dataset;
    }

    catch (H5::FileIException error) {
        // TODO: Can use variadic template to construct the error message
        std::cerr << "Cannot open \"" << DataSetName << "\" from \"" << FileName
                  << "\" to read." << std::endl;
        throw(error);
    }

    catch (H5::GroupIException error) {
        throw(error);
    }
}
