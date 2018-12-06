cmake_minimum_required(VERSION 3.9)
project(brtools 
    VERSION 1.0.0 
    DESCRIPTION "Tools for parsing and processing binary_revolution(br) files."
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

enable_testing()

add_subdirectory(
    "${PROJECT_SOURCE_DIR}/test"
)

add_library(brtools STATIC
    source/error/errors.cpp
    source/error/integrity_error.cpp
    source/io/stream_parser.cpp 
    source/io/section_parser.cpp 
    source/io/file_parser.cpp
    source/trie/stringtrie.cpp
)

set_target_properties(brtools PROPERTIES
    VERSION ${PROJECT_VERSION}
)

target_compile_features(brtools PRIVATE
    cxx_std_14
)

target_include_directories(brtools
    PRIVATE ${PROJECT_SOURCE_DIR}/source
    PUBLIC  ${PROJECT_SOURCE_DIR}/header
)
