
cmake_minimum_required(VERSION 3.16.3 FATAL_ERROR)

# SETUP CPM -----
include("cmake/cpm.cmake")
CPMUsePackageLock(package-lock.cmake) # Use a package-lock for versioning
# ---------------

# Project
project("My Game" DESCRIPTION "My game" LANGUAGES C CXX)
# ---

# Fix cmake paths
list(PREPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(PREPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
list(PREPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})
# ---

set(CPM_SOURCE_CACHE "${PROJECT_SOURCE_DIR}/.cache/cpm")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSPDLOG_FMT_EXTERNAL -DNOMINMAX")
set(CMAKE_CXX_STANDARD 23) # 23 recommended, but support 20 and lower
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(BUILD_SHARED_LIBS OFF)

# Apple needs this flag to compile mixed objective/c++
if(APPLE AND NOT XCODE)
    set(CMAKE_CXX_FLAGS "-ObjC++")
endif()

# Linux and Wayland support
if(NOT WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-psabi")
else()
    # Ignore warnings about missing pdb
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ignore:4099")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /ignore:4099")
    set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} /ignore:4099")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /permissive-")
endif()

set(CMAKE_SKIP_RPATH TRUE)
set(CMAKE_SKIP_BUILD_RPATH TRUE)

# SETUP LIBS ---
CPMAddPackage(
    NAME
        rawrbox
    GITHUB_REPOSITORY
        edunad/rawrbox
    GIT_TAG
        master
    OPTIONS
        "RAWRBOX_BUILD_SAMPLES OFF"
        "RAWRBOX_BUILD_TESTING OFF"

        "RAWRBOX_BUILD_RAWRBOX_RESOURCES ON"
)
# --------------

add_subdirectory("./code")
