cmake_minimum_required (VERSION 2.8.8)
project (segmentation CXX)

set (CXX_FLAGS "-std=c++14")
add_definitions(${CXX_FLAGS})

find_package(OpenMP REQUIRED)
find_package(Boost REQUIRED COMPONENTS system iostreams)
find_package(ZLIB REQUIRED)

set(COMMON_SRC src/voxel_set.cpp src/fast_clustering.cpp src/graph.cpp)

add_executable(fast-split src/split.cpp) 
add_executable(bc src/main.cpp src/graph.cpp)

add_library(common ${COMMON_SRC})

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src)

target_link_libraries(fast-split common ${Boost_LIBRARIES})
target_link_libraries(bc common ${Boost_LIBRARIES})

add_subdirectory(src/test)
