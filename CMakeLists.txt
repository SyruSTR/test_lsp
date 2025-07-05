cmake_minimum_required(VERSION 3.30)
project(-test_lsp)


set(_DEBUG)
set(CMAKE_CXX_STANDARD 20)

set(TEST_COMPILER
        CompilerHandler/test_launch.cpp
        CompilerHandler/Compiler.h
        CompilerHandler/Compiler.cpp
)
add_executable(compiler-test ${TEST_COMPILER})

set(SOURCE_FILES
        main.cpp

        Logger/Logger.cpp
        Logger/Logger.h

        Messages/methods/ServerInfo.h
        Messages/Message.h
        Messages/ResponseMessage.h
        Messages/RequestMessage.h
        Messages/TextDocument/TextDocumentSync.h
        Messages/NotificationMessage.h
        Messages/TextDocument/Params/DidChangeTextParams.h
        Messages/TextDocument/Params/CompletionParams.h
        Messages/TextDocument/Params/Params.h
        Messages/TextDocument/Result/ResponseResult.h
        Messages/TextDocument/Result/CompletionResult.h
        Messages/TextDocument/CompletionList.h
        Messages/TextDocument/Result/FullDocumentDiagnosticReport.h
        Messages/Range.h
        Messages/TextDocument/DiagnosticStruct.h
        Messages/TextDocument/Params/DocumentDiagnosticParams.h
        Messages/DictionaryWords.cpp
        Messages/DictionaryWords.h
        Messages/TextDocument/TextDocument.cpp
        Messages/TextDocument/TextDocument.h
        Messages/TextDocument/Params/DidOpenTextDocumentParams.h
        Messages/TextDocument/TextDocumentItem.h
        CompilerHandler/Compiler.h
        CompilerHandler/Compiler.cpp
        CompilerHandler/scope.h
)
option(DEBUGING "Enable debugging" OFF)

add_executable(my-lsp-server ${SOURCE_FILES})
if(DEBUGING)
    message(STATUS "Debugging is enabled.")
    target_compile_definitions(my-lsp-server PRIVATE DEBUG)
endif()
target_include_directories(my-lsp-server PRIVATE external/)


