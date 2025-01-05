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

    target_link_libraries(GrowBase PRIVATE 
        # RELEASE
        
        # enet
        optimized ${CMAKE_SOURCE_DIR}/lib/enet/libs/win/Release/enet.lib

        # SFML
        optimized ${CMAKE_SOURCE_DIR}/lib/SFML/libs/win/Release/sfml-graphics.lib
        optimized ${CMAKE_SOURCE_DIR}/lib/SFML/libs/win/Release/sfml-main.lib
        optimized ${CMAKE_SOURCE_DIR}/lib/SFML/libs/win/Release/sfml-system.lib
        optimized ${CMAKE_SOURCE_DIR}/lib/SFML/libs/win/Release/sfml-window.lib

        # zlib
        optimized ${CMAKE_SOURCE_DIR}/lib/zlib/libs/win/Release/zlib.lib
        optimized ${CMAKE_SOURCE_DIR}/lib/zlib/libs/win/Release/zlibstatic.lib

        # DEBUG

        # enet
        debug ${CMAKE_SOURCE_DIR}/lib/enet/libs/win/Debug/enet.lib

        # SFML
        debug ${CMAKE_SOURCE_DIR}/lib/SFML/libs/win/Debug/sfml-graphics-d.lib
        debug ${CMAKE_SOURCE_DIR}/lib/SFML/libs/win/Debug/sfml-main-d.lib
        debug ${CMAKE_SOURCE_DIR}/lib/SFML/libs/win/Debug/sfml-system-d.lib
        debug ${CMAKE_SOURCE_DIR}/lib/SFML/libs/win/Debug/sfml-window-d.lib

        # zlib
        debug ${CMAKE_SOURCE_DIR}/lib/zlib/libs/win/Debug/zlibd.lib
    )
elseif (UNIX)
    target_link_libraries(GrowBase PRIVATE 
        pthread
    )

    target_link_libraries(GrowBase PRIVATE 
        # RELEASE
        
        # enet
        optimized ${CMAKE_SOURCE_DIR}/lib/enet/libs/unix/Release/enet.so

        # SFML
        optimized ${CMAKE_SOURCE_DIR}/lib/SFML/libs/unix/Release/sfml-graphics.so
        optimized ${CMAKE_SOURCE_DIR}/lib/SFML/libs/unix/Release/sfml-main.so
        optimized ${CMAKE_SOURCE_DIR}/lib/SFML/libs/unix/Release/sfml-system.so
        optimized ${CMAKE_SOURCE_DIR}/lib/SFML/libs/unix/Release/sfml-window.so

        # zlib
        optimized ${CMAKE_SOURCE_DIR}/lib/zlib/libs/unix/Release/zlib.so

        # DEBUG

        # enet
        debug ${CMAKE_SOURCE_DIR}/lib/enet/libs/unix/Debug/enet.so

        # SFML
        debug ${CMAKE_SOURCE_DIR}/lib/SFML/libs/unix/Debug/sfml-graphics-d.so
        debug ${CMAKE_SOURCE_DIR}/lib/SFML/libs/unix/Debug/sfml-main-d.so
        debug ${CMAKE_SOURCE_DIR}/lib/SFML/libs/unix/Debug/sfml-system-d.so
        debug ${CMAKE_SOURCE_DIR}/lib/SFML/libs/unix/Debug/sfml-window-d.so

        # zlib
        debug ${CMAKE_SOURCE_DIR}/lib/zlib/libs/unix/Debug/zlibd.so
    )
endif()

# target precompiled headers
target_precompile_headers(GrowBase PRIVATE ${PCH_HEADER})

# target compiling definitions
target_compile_definitions(GrowBase PRIVATE 
    _CRT_SECURE_NO_WARNINGS
    _WINSOCK_DEPRECATED_NO_WARNINGS
)