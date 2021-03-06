cmake_minimum_required(VERSION 3.9)
set(GTEST_SOURCE_DIR "${PROJECT_SOURCE_DIR}/framework/googletest")
include(ExternalProject)
ExternalProject_Add(googletest
    GIT_REPOSITORY    https://github.com/google/googletest.git
    GIT_TAG           "release-1.8.1"
    GIT_SHALLOW       1
    CMAKE_ARGS        -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY=${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
                      -DCMAKE_LIBRARY_OUTPUT_DIRECTORY=${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    SOURCE_DIR        "${GTEST_SOURCE_DIR}"
    INSTALL_COMMAND   ""
)

# This is needed for linking gtest and gtest_main because they
# are in an external project, so these libs are not available 
# at configure time for CMake to find.
link_directories(${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

add_executable(brtools-tests
    mocks/mock_sequence_visitor.cpp

    tests/stream_parser_test.cpp
    tests/section_parser_test.cpp
    tests/file_parser_test.cpp
    tests/string_bit_index_type_test.cpp
    tests/stringtrie_test.cpp
    tests/varint_test.cpp
    tests/uint_test.cpp
    tests/random_test.cpp
    tests/sized_ref_test.cpp
    tests/variable_test.cpp
    tests/sequence_test.cpp
)

add_dependencies(brtools-tests googletest)

target_compile_features(brtools-tests PRIVATE
    cxx_std_14
)

target_include_directories(brtools-tests
    SYSTEM PUBLIC "${GTEST_SOURCE_DIR}/googletest/include"
    SYSTEM PUBLIC "${GTEST_SOURCE_DIR}/googlemock/include"
    PRIVATE "${PROJECT_SOURCE_DIR}/source"
    PRIVATE "${PROJECT_SOURCE_DIR}/include"
    PRIVATE "."
)

target_link_libraries(brtools-tests
    brtools
    gtest
    gmock
    gtest_main
)

include(GoogleTest)
gtest_add_tests(TARGET brtools-tests)
