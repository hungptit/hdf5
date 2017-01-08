namespace
{
  template <typename T>
  bool isContaint(std::vector<T> & data, std::vector<T> & searchPattern)
  {
    if (data.size() < searchPattern.size())
      {
	return false;
      }
    else
      {
            
      }
  }
}

Writer::Writer(const std::string & fileName) :
  FileName(fileName),
  GroupNames()
{
  openFile();
}


Writer::~Writer()
{
  cleanUp();
}


void Writer::openFile()
{
  try
    {
      H5::Exception::dontPrint(); // Do not print the error if the below line throw.
      FileID = new H5::H5File(FileName, H5F_ACC_EXCL);
    }

  catch (H5::FileIException error)
    {
      FileID = new H5::H5File(FileName, H5F_ACC_RDWR);
    }
}

void Writer::closeFile()
{
  if (FileID != NULL)
    {
      delete FileID;
      FileID = NULL;
    }
}


// We can make this better by opening neccessary groups.
void Writer::openGroups(std::vector< std::string > & groupNames)
{
  if (!Groups.empty() && (GroupNames == groupNames))
    {
      // Do not need to open the group. It has already opened.
      return;
    }
  else
    {
      // Need to close the opened group before open a new group. We can make
      // this code segment smarter.
      closeGroups();  
      GroupNames = groupNames;
    }

  std::string groupName;      // This string will grow inside the for loop
  for (GroupNameContainer::const_iterator it = GroupNames.begin(); it != GroupNames.end(); ++it)
    {
      groupName += "/" + *it;
      openAGroup(groupName);
    }

  // Now cache the path of an opened group.
  GroupNames = groupNames;

}


void Writer::openAGroup(const std::string & groupName)
{
  std::cout << groupName << std::endl;
  try
    {
      Groups.push_back(new H5::Group(FileID->createGroup(groupName)));
    }
  catch (H5::FileIException error)
    {
      Groups.push_back(new H5::Group(FileID->openGroup(groupName)));
    }
}


void Writer::closeGroups()
{
  for (GroupContainer::reverse_iterator it = Groups.rbegin(); it != Groups.rend(); ++it) 
    {
      delete *it;
    }
  Groups.clear();
}


void Writer::cleanUp()
{
  closeGroups();
  closeFile();
}


/**
 * @brief Below are implementations of write method for several datatype.
 * 
 * @note User need to specify the compression dimension if there is not any
 * realiable solution.
 */

template <class T>
void Writer::write(const std::string & dataSetName,
                   const std::vector<T> & data)
{
  H5Traits::dimension_type dims(1, data.size());
  H5Traits::dimension_type cdims(1, std::min(data.size(), H5Traits::MAX_CHUNK_LEN));
  write(dataSetName, data, dims, cdims, BEST_COMPRESSION);
}


template <typename T>
void Writer::write(const std::string & dataSetName,
                   const std::vector<T> & data,
                   const H5Traits::dimension_type & dims,
                   const H5Traits::dimension_type & cdims,
                   const int compressionLevel)
{
  write(dataSetName, &data[0], dims, cdims, compressionLevel);
}


template <typename T>
void Writer::write(const std::string & dataSetName,
                   const T *data,
                   const H5Traits::dimension_type & dims,
                   const H5Traits::dimension_type & cdims,
                   const int compressionLevel)
{
  H5::Exception::dontPrint();

  std::cout << "Dataset: " << dataSetName << std::endl;

  std::vector< std::string > groupNames;
  parseDataSetNames(dataSetName, groupNames);
  groupNames.pop_back(); // The last element is the dataset name
  openGroups(groupNames);

  H5::DataType dataType = H5DataTypeTrait<T>::dataType;
  H5::DataSpace dataspace(dims.size(), &dims[0]);

  H5::DSetCreatPropList datasetPropList;
  datasetPropList.setChunk(dims.size(), &cdims[0]);
  datasetPropList.setDeflate(compressionLevel);

  try
    {
      H5::DataSet dataset = FileID->createDataSet(dataSetName, dataType, dataspace, datasetPropList );
      dataset.write(data, dataType);
    }

  catch (H5::FileIException error)
    {
      std::cerr << "Cannot write to an existing dataset \"" << dataSetName << "\" in \""<< FileName << "\"" << std::endl;
      throw(error);
    }
}


void Writer::write(const std::string & dataSetName, const std::string & strBuf)
{
  H5Traits::dimension_type dims(1, strBuf.size());
  H5Traits::dimension_type cdims(1, strBuf.size());
  write(dataSetName, strBuf.c_str(), dims, cdims, BEST_COMPRESSION);
}
