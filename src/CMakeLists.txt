cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_STANDARD 11)

set(ENVSDK $ENV{PLAYDATE_SDK_PATH})

add_compile_options(-Wno-strict-prototypes)

if (NOT ${ENVSDK} STREQUAL "")
    # Convert path from Windows
    file(TO_CMAKE_PATH ${ENVSDK} SDK)
else()
    execute_process(
            COMMAND bash -c "egrep '^\\s*SDKRoot' $HOME/.Playdate/config"
            COMMAND head -n 1
            COMMAND cut -c9-
            OUTPUT_VARIABLE SDK
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif()

if (NOT EXISTS ${SDK})
    message(FATAL_ERROR "SDK Path not found; set ENV value PLAYDATE_SDK_PATH")
    return()
endif()

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
set(CMAKE_XCODE_GENERATE_SCHEME TRUE)

# Game Name Customization
set(PLAYDATE_GAME_NAME phoenix)
set(PLAYDATE_GAME_DEVICE phoenix_DEVICE)

project(${PLAYDATE_GAME_NAME} C ASM)

# Set property for IDE folder structure (specifically for Xcode)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

file(GLOB PD_SOURCES "*.c" "*.h")
file(GLOB SHARED_SOURCES "../shared/*.c" "../shared/*.h")

# Combine all source files
set(ALL_SOURCES
    ${PD_SOURCES}
    ${SHARED_SOURCES}
)

# Fix include directories - include project root and src directory
include_directories(
    ${CMAKE_SOURCE_DIR}  # Include project root
    ${CMAKE_SOURCE_DIR}/../shared  # Include src directory
)

if (TOOLCHAIN STREQUAL "armgcc")
    add_executable(${PLAYDATE_GAME_DEVICE} main.c ${ALL_SOURCES} )
else()
    add_library(${PLAYDATE_GAME_NAME} SHARED main.c ${ALL_SOURCES} )
endif()

target_compile_definitions(${PLAYDATE_GAME_NAME} PRIVATE TARGET_PLAYDATE_FAMILY)

# Enable Debug Info and disable optimizations for macOS builds (simulator)
if(NOT TOOLCHAIN STREQUAL "armgcc")
    set(CMAKE_BUILD_TYPE Debug)

    # Force dSYM generation and disable optimizations
    set(CMAKE_C_FLAGS_DEBUG "-O0 -g -fno-omit-frame-pointer")
    set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -fno-omit-frame-pointer")

    # Force Xcode to use DWARF with dSYM format
    if(CMAKE_GENERATOR MATCHES "Xcode")
        set(CMAKE_XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "dwarf-with-dsym")
    endif()
endif()

# Set header search paths for Xcode specifically
if(CMAKE_GENERATOR MATCHES "Xcode")
    set(CMAKE_XCODE_ATTRIBUTE_HEADER_SEARCH_PATHS
        "${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src")
endif()

include(${SDK}/C_API/buildsupport/playdate_game.cmake)

add_custom_command(TARGET phoenix POST_BUILD
    COMMAND codesign --force --sign - $<TARGET_FILE:phoenix>
    COMMENT "Ad-hoc signing pdex.dylib for Playdate Simulator"
)
