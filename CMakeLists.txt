cmake_minimum_required(VERSION 3.30)
project(-test_lsp)


set(_DEBUG)
set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
        main.cpp
        external/json.hpp

        Logger/Logger.cpp
        Logger/Logger.h

        Messages/TextDocument/Completion.h
        Messages/methods/initialize.h
        Messages/methods/InitializeResult.h
        Messages/methods/ServerInfo.h
        Messages/Message.h
        Messages/ResponseMessage.h
        Messages/RequestMessage.h
)

add_executable(my-lsp-server ${SOURCE_FILES})

