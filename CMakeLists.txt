cmake_minimum_required(VERSION 3.30)
project(-test_lsp)


set(_DEBUG)
set(CMAKE_CXX_STANDARD 20)

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
        Messages/TextDocument/Rapams/DidChangeTextParams.h
        Documents.h
        Messages/TextDocument/Rapams/CompletionParams.h
        Messages/TextDocument/Rapams/Params.h
        Messages/ResponceResult.h
        Messages/TextDocument/Result/CompletionResult.h
        Messages/TextDocument/CompletionList.h
        Messages/TextDocument/Result/FullDocumentDiagnosticReport.h
        Messages/Range.h
        Messages/TextDocument/DiagnosticStruct.h
        Messages/TextDocument/Rapams/DocumentDiagnosticParams.h
        Messages/DictionaryWords.cpp
        Messages/DictionaryWords.h
        Messages/TextDocument/TextDocument.cpp
        Messages/TextDocument/TextDocument.h
)

add_executable(my-lsp-server ${SOURCE_FILES})
target_include_directories(my-lsp-server PRIVATE external/)


