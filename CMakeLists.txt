cmake_minimum_required(VERSION 3.1)
project(db_benchmarks VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost 1.50 REQUIRED filesystem)
if(!Boost_FOUND)
	message(FATAL_ERROR "Boost is required for this project.")  
endif()
include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(leveldb)

add_executable(db_benchmarks main.cpp)
target_link_libraries(db_benchmarks ${Boost_LIBRARIES} leveldb)
add_dependencies(db_benchmarks leveldb)
target_include_directories(db_benchmarks PRIVATE leveldb/include)
