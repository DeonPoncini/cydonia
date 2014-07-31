cmake_minimum_required(VERSION 2.8)
project(libnetwork)

################################
# Compiler setup
################################

add_definitions(-Wall)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -DBOOST_LOG_DYN_LINK")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

################################
# Third party libraries
################################
find_package(Threads)
find_package(Boost 1.55.0 COMPONENTS
    log log_setup thread system chrono date_time REQUIRED)

include_directories(${BOOST_INCLUDE_DIRS})

################################
# Locate source
################################
file(GLOB_RECURSE INCLUDES include/*.h)
file(GLOB_RECURSE SRC src/*.cpp)

include_directories(include)
include_directories(src)

################################
# Libraries
################################
set(NETWORK_LIBS
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    )

################################
# Build targets
################################
add_library(network SHARED
    ${SRC}
    ${INCLUDES})

target_link_libraries(network
    ${NETWORK_LIBS})

