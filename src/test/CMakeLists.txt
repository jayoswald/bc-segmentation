add_executable(tests main.cpp catch.hpp test_graph.cpp)
target_link_libraries(tests common ${Boost_LIBRARIES})
