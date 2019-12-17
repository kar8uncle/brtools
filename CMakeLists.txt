cmake_minimum_required(VERSION 3.9)
project(brtools 
    VERSION 1.0.0 
    DESCRIPTION "Tools for parsing and processing binary_revolution(br) files."
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

enable_testing()

add_subdirectory("${PROJECT_SOURCE_DIR}/test")
add_subdirectory("${PROJECT_SOURCE_DIR}/apps")

add_library(brtools STATIC
	source/brtools/data/sequence/parser.h
    source/brtools/data/sequence/parser.cpp
    source/brtools/data/sequence/sequence.cpp
	source/brtools/data/sequence/events_impl.h
	source/brtools/data/sequence/resolvable.h

	source/brtools/data/types/random.h
    source/brtools/data/types/random.cpp
	source/brtools/data/types/variable.h
    source/brtools/data/types/variable.cpp
	source/brtools/data/types/sized_ref.h
    source/brtools/data/types/sized_ref.cpp
	source/brtools/data/types/uint.h
	source/brtools/data/types/varint.h

    source/brtools/error/errors.cpp
    source/brtools/error/integrity_error.cpp

	source/brtools/io/stream_parser.h
    source/brtools/io/stream_parser.cpp 
	source/brtools/io/section_parser.h
    source/brtools/io/section_parser.cpp 
	source/brtools/io/file_parser.h
    source/brtools/io/file_parser.cpp

	source/brtools/trie/detail/string_bit_index_type.h
    source/brtools/trie/stringtrie.cpp

	source/brtools/util/integrity_expect.h
	source/brtools/util/stream_data_extractor.h


	header/brtools/data/audio_data.h
	header/brtools/data/playable.h

	header/brtools/data/sequence/eventfwd.h
	header/brtools/data/sequence/events.h
	header/brtools/data/sequence/sequence.h
	header/brtools/data/sequence/visitable.h
	header/brtools/data/sequence/visitor.h
	header/brtools/data/sequence/visitor_acceptable.h

	header/brtools/error/errors.h
	header/brtools/error/integrity_error.h

	header/brtools/trie/stringtrie.h
	header/brtools/util/id_dispenser.h
	header/brtools/util/unit_testable.h 
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

if (MSVC)
	# Suppresses all MSVC warnings :-)
	target_compile_options(brtools PUBLIC "/W0")
endif (MSVC)