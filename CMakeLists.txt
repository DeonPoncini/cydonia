cmake_minimum_required(VERSION 2.8)
project(cydonia)

################################
# Compiler setup
################################

set(NETWORK_VERSION "0.0.1")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_LOG_DYN_LINK")

################################
# Third party libraries
################################
find_package(Threads)
find_package(Boost 1.55.0 COMPONENTS
    log log_setup thread system REQUIRED)
find_package(zephyr)

include_directories(${BOOST_INCLUDE_DIRS})
include_directories(${ZEPHYR_INCLUDE_DIRS})

################################
# Locate source
################################
file(GLOB_RECURSE INCLUDES include/*.h)
file(GLOB_RECURSE SRC src/*.cpp)
file(GLOB_RECURSE TEST test/*.cpp)

include_directories(include)
include_directories(src)

################################
# Libraries
################################
set(NETWORK_LIBS
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    ${ZEPHYR_LIBRARIES}
    )

################################
# Build targets
################################
add_library(cydonia SHARED
    ${SRC}
    ${INCLUDES})

target_link_libraries(cydonia
    ${NETWORK_LIBS})

add_executable(cydonia-test
    ${TEST})

target_link_libraries(cydonia-test
    cydonia)

################################
# Export library
################################

export_project(
    NAME cydonia
    INCLUDES ${CMAKE_CURRENT_LIST_DIR}/include
    BINS cydonia-test
    LIBS cydonia
    VERSION ${NETWORK_VERSION}
)

