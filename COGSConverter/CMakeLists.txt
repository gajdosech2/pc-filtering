cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(COGSConverter)

include_directories(
    ${CMAKE_CURRENT_LIST_DIR}/libs/include
    ${CMAKE_CURRENT_LIST_DIR}/include
)

link_directories(
    ${CMAKE_CURRENT_LIST_DIR}/libs/lib/Release
)

add_executable(${PROJECT_NAME} 
    include/lodepng.h
    src/lodepng.cpp

    include/FormattingUtilities.h   
    include/PCDFormatter.h
    include/XYZFormatter.h
    include/ScanBoxes.h
    include/ScanFormatter.h
    include/ScanImager.h
    include/ScanSegmentation.h
    
    src/COGSConverter.cpp
    src/FormattingUtilities.cpp
    src/XYZFormatter.cpp
    src/ScanBoxes.cpp
    src/ScanFormatter.cpp
    src/ScanImager.cpp
    src/ScanSegmentation.cpp
    src/PCDFormatter.cpp
 )

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

if (MSVC)
    target_link_libraries(${PROJECT_NAME} 
        COGS_msvc141_v0.1.0
        GEOM_msvc141_v0.1.0 
        Utils_msvc141_v0.1.0
    )

    file(GLOB COGS_DLLS ${CMAKE_CURRENT_LIST_DIR}/libs/bin/Release/*.dll)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${COGS_DLLS} $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
else()
    target_link_libraries(${PROJECT_NAME} 
        libGEOM_Clang8.0.1_v0.1.0
    )
endif (MSVC)









