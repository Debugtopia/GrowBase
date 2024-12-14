cmake_minimum_required(VERSION 3.10)
project(GrowBase VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/../bin/out)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/../bin/out)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_CURRENT_SOURCE_DIR}../bin/out)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_CURRENT_SOURCE_DIR}/../bin/out)
set(PCH_HEADER "BaseApp.h")

# source files
file(GLOB_RECURSE SOURCES "*.cpp" "*.h" "*.hpp" "*.hh" "*.cc")
add_executable(GrowBase ${SOURCES})

# target directories
target_include_directories(GrowBase PRIVATE 
    .
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/lib/enet/include
)

# library files
if (WIN32)
    target_link_libraries(GrowBase PRIVATE 
        ws2_32.lib
        winmm.lib
    )
elseif (UNIX)
    target_link_libraries(GrowBase PRIVATE 
        pthread
    )
endif()

target_link_libraries(GrowBase PRIVATE 
    optimized ${CMAKE_SOURCE_DIR}/lib/enet/libs/Release/enet.lib
    debug ${CMAKE_SOURCE_DIR}/lib/enet/libs/Debug/enet.lib
)

# target precompiled headers
target_precompile_headers(GrowBase PRIVATE ${PCH_HEADER})

# target definitions
target_compile_definitions(GrowBase PRIVATE 
    _CRT_SECURE_NO_WARNINGS
)