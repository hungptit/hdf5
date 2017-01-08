PROJECT (HDF5)
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
# set (CMAKE_BUILD_TYPE Debug)
set (CMAKE_BUILD_TYPE Release)

set(PROJECTS_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/../../")
ENABLE_TESTING()

# Set compiler
# set(CMAKE_CXX_COMPILER clang++)
# set(CMAKE_CXX_COMPILER icpc)
set(CMAKE_CXX_COMPILER "${PROJECTS_FOLDER}/3p/llvm/bin/clang++")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pedantic -O3 -Wno-deprecated-register -Wno-keyword-macro -Wno-unused-local-typedef")
#  -fdefine-sized-deallocation

# Include related folders
INCLUDE_DIRECTORIES ("${PROJECTS_FOLDER}")
INCLUDE_DIRECTORIES ("${PROJECTS_FOLDER}/3p/eigen/include/eigen3/")
INCLUDE_DIRECTORIES ("${PROJECTS_FOLDER}/3p/hdf5/include")
INCLUDE_DIRECTORIES ("${PROJECTS_FOLDER}/3p/gtest/include")

message ("${PROJECTS_FOLDER}/3p/hdf5/include")

set(LIB_GTEST_MAIN "${PROJECTS_FOLDER}/3p/gtest/lib/libgtest_main.a")
set(LIB_GTEST "${PROJECTS_FOLDER}/3p/gtest/lib/libgtest.a")
set(LIB_HDF5_CPP "${PROJECTS_FOLDER}/3p/hdf5/lib/libhdf5_cpp.a")
set(LIB_HDF5 "${PROJECTS_FOLDER}/3p/hdf5/lib/libhdf5.a")
set(LIB_HDF5_HL "${PROJECTS_FOLDER}/3p/hdf5/lib/libhdf5_hl.a")
set(LIBZ "${PROJECTS_FOLDER}/3p/zlib/lib/libz.a")

# Find Boost libraries
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

# This option make sure that we use the local boost version.
set(Boost_NO_SYSTEM_PATHS TRUE) 
if (Boost_NO_SYSTEM_PATHS)
  set(BOOST_ROOT "${PROJECTS_FOLDER}/3p/boost")
  message(STATUS ${PROJECTS_FOLDER})
  message(STATUS ${BOOST_ROOT})
  set(BOOST_INCLUDE_DIRS "${BOOST_ROOT}/include")
  set(BOOST_LIBRARY_DIRS "${BOOST_ROOT}/lib")
endif (Boost_NO_SYSTEM_PATHS)

find_package(Boost REQUIRED regex date_time system filesystem thread graph program_options)
include_directories(${BOOST_INCLUDE_DIRS})


# Compile all binaries
set(SRC_FILES tArray tEigen tVector tString tUtils)
foreach (source_file ${SRC_FILES})
  ADD_EXECUTABLE(${source_file} ${source_file}.cpp)
  TARGET_LINK_LIBRARIES(${source_file} ${Boost_LIBRARIES} ${LIB_HDF5_CPP} ${LIB_HDF5} ${LIB_HDF5_HL} ${LIBZ}
    ${LIB_GTEST} ${LIB_GTEST_MAIN} -lpthread -ldl)
  ADD_TEST(${source_file} ./${source_file})
endforeach (source_file ${SRC_FILES})

# Memory test setup
find_program(CTEST_MEMORYCHECK_COMMAND NAMES "valgrind")
INCLUDE(Dart)
