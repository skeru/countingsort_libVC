cmake_minimum_required( VERSION 3.0.2 )

# target
set ( EXE_NAME "run_countsort" )

# look for dependencies
list ( APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" )

find_package ( LibVersioningCompiler REQUIRED )
include_directories ( ${LIBVC_INCLUDES} )
link_directories( ${LIVC_LIB_DIR} )

# finalize dependencies
set ( DEP_LIBS ${DEP_LIBS} ${LIBVC_LIBRARIES} )

# sources
set ( SRC_CNTSRT ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/kernel.cpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/TimeMonitor.cpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/utils.cpp
                )

# compiler options
set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )
set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3" )
set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto" )
set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_PATH_TO_KERNEL=\"\\\"${CMAKE_CURRENT_SOURCE_DIR}/\\\"\"" )

# finalize target
add_executable( ${EXE_NAME} ${SRC_CNTSRT} )
target_link_libraries( ${EXE_NAME} ${DEP_LIBS} )

# install path
if ( CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT )
  set (CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR})
endif ()

install ( TARGETS ${EXE_NAME} DESTINATION bin )
