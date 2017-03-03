cmake_minimum_required( VERSION 3.0.2 )

set ( EXE_NAME "run_countsort" )

set ( SRC_CNTSRT ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/kernel.cpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/utils.cpp
                )

set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )
set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3" )
set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto" )


add_executable( ${EXE_NAME} ${SRC_CNTSRT} )

if ( CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT )
  set (CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR})
endif ()

install ( TARGETS ${EXE_NAME} DESTINATION bin )
