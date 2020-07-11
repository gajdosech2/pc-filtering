cmake_minimum_required(VERSION 3.16)
project(UbuntuCC)

set(CMAKE_CXX_COMPILER  "/usr/bin/g++-7")
set(CMAKE_CXX_FLAGS "-std=c++1z")
set(CMAKE_CXX_STANDARD 17)

include_directories(SkeletexLibs/include)
link_directories("SkeletexLibs/lib/Release")

add_executable(UbuntuCC COGSConverter.cpp DataFormatter.h ImageGenerator.h PointFeatures.h DataFormatter.cpp ImageGenerator.cpp)

target_link_libraries(UbuntuCC COGS GEOM assimp)



