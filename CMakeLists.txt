cmake_minimum_required(VERSION 3.30)
project(-test_lsp)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
        main.cpp
        external/json.hpp
)

add_executable(my-lsp-server ${SOURCE_FILES})

