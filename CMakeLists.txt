set(CMAKE_CXX_FLAGS "-std=c++17 -O3 -Wall -pedantic")
add_executable(water_crisis main.cpp params.cpp solution.cpp step.cpp)
