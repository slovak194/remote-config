cmake_minimum_required(VERSION 3.13)
project(remote-config)

set(CMAKE_CXX_STANDARD 20)

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wno-psabi")

find_package(Eigen3 REQUIRED NO_MODULE)
find_package(Boost REQUIRED COMPONENTS system chrono regex)
find_package(nlohmann_json REQUIRED)
find_package(yaml-cpp REQUIRED)

add_subdirectory(test)
