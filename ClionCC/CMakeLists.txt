cmake_minimum_required(VERSION 3.16)
project(UbuntuCC)

set(CMAKE_CXX_COMPILER "/usr/bin/clang++-8")
set(CMAKE_CXX_FLAGS "-std=c++1z -DNO_FREETYPE")
set(CMAKE_CXX_STANDARD 17)

include_directories(SkeletexLibs/include)
include_directories(PNGWriter)
link_directories("SkeletexLibs/lib/Release")

add_executable(UbuntuCC PointCleanNetFormatter.h PointCleanNetFormatter.cpp COGSConverter.cpp DataFormatter.h ImageGenerator.h PointFeatures.h DataFormatter.cpp ImageGenerator.cpp PNGWriter/pngwriter.cc TrimValues.h)

target_link_libraries(UbuntuCC Utils_Clang8.0.1_v0.1.0 COGS_Clang8.0.1_v0.1.0 GEOM_Clang8.0.1_v0.1.0 assimp png)









