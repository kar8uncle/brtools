cmake_minimum_required(VERSION 3.9)

add_executable(rseqprint
    main.cpp
)

target_link_libraries(rseqprint
    brtools
)

target_compile_features(rseqprint PRIVATE
    cxx_std_14
)
