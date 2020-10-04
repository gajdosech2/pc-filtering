cmake_minimum_required(VERSION 3.16)
project(COGSConverter)

set(CMAKE_CXX_COMPILER "/usr/bin/clang++-8")
set(CMAKE_CXX_FLAGS "-std=c++1z -DNO_FREETYPE")
set(CMAKE_CXX_STANDARD 17)

include_directories(libs/include)
include_directories(include)
link_directories("libs/lib/Release")

add_executable(COGSConverter
        include/FormattingUtilities.h
        include/lodepng.h
        include/PointCleanNetFormatter.h
        include/ScanFormatter.h
        include/ScanImager.h
        include/ScanTabler.h
        src/COGSConverter.cpp
        src/FormattingUtilities.cpp
        src/lodepng.cpp
        src/PointCleanNetFormatter.cpp
        src/ScanFormatter.cpp
        src/ScanImager.cpp
        )

target_link_libraries(COGSConverter Utils_Clang8.0.1_v0.1.0 COGS_Clang8.0.1_v0.1.0 GEOM_Clang8.0.1_v0.1.0 assimp)









