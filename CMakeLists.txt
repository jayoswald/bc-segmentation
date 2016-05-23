cmake_minimum_required (VERSION 2.8.8)
project (segmentation)

find_package(OpenMP REQUIRED)
find_package(Boost REQUIRED COMPONENTS system iostreams)
find_package(ZLIB REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -std=c++11 -Wall")
set(COMMON_SRC src/voxel_set.cpp src/fast_clustering.cpp src/graph.cpp)

add_executable(fast-split src/fast_split.cpp)
add_executable(segment src/segment.cpp)
add_library(common ${COMMON_SRC})

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src)

target_link_libraries(fast-split common ${Boost_LIBRARIES})
target_link_libraries(segment common ${Boost_LIBRARIES})

add_subdirectory(src/test)
set(CMAKE_BUILD_TYPE RELEASE)

install(TARGETS fast-split segment RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin/)
