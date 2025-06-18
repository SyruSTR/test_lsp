//
// Created by grusted on 6/16/25.
//

#ifndef COMPLETION_H
#define COMPLETION_H
#include <string>
#include <vector>

#include "CompletionList.h"
#include "CompletionResult.h"
#include "../RequestMessage.h"
#include "../ResponseMessage.h"
#include "../../external/json.hpp"
#include "../../Documents.h"

namespace lsp_test {

    using json = nlohmann::json;

    // json completion(const RequestMessage& msg, Documents &documents) {
    //     CompletionList completion;
    //     completion.isIncomplete = false;
    //
    //     // auto content = documents.textDocuments[msg.params.value().textDocument];
    //
    //     json request = msg;
    //     completion.items.emplace_back("TypeScript");
    //     completion.items.emplace_back("LSP");
    //     completion.items.emplace_back("Lua");
    //     // request["result"] = {{"items", completion.items}};
    //     request["result"].push_back({"items",completion.items});
    //     return request;
    // }
    template<typename T, std::enable_if_t<std::is_base_of_v<Params, T>, bool> = true>
    json completion(const RequestMessage<T>& msg) {
        CompletionResult completionResult;


        completionResult.completion_list.isIncomplete = false;
        completionResult.completion_list.items.emplace_back("TypeScript");
        completionResult.completion_list.items.emplace_back("LSP");
        completionResult.completion_list.items.emplace_back("Lua");


        ResponseMessage<CompletionResult> response = ResponseMessage(msg.id,completionResult);
        json j = response;
        // j["result"].push_back({"items",completionList.items});
        // for (const auto &item : ComList.items) {
        //     response["result"]["items"].push_back(item.label);
        // }
        return response;
    }
}


#endif //COMPLETION_H
