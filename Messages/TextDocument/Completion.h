//
// Created by grusted on 6/16/25.
//

#ifndef COMPLETION_H
#define COMPLETION_H
#include <string>
#include <vector>

#include "Result/CompletionResult.h"
#include "../DictionaryWords.h"
#include "../RequestMessage.h"
#include "../ResponseMessage.h"
#include "../../external/json.hpp"

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
    json completion(const RequestMessage<T>& msg, const DictionaryWords& dictionary, std::string starts_with) {
        CompletionResult completionResult;

        dictionary.WrapToCompletionList(completionResult.completion_list,starts_with);


        completionResult.completion_list.isIncomplete = true;
        // completionResult.completion_list.items.emplace_back("TypeScript");
        // completionResult.completion_list.items.emplace_back("LSP");
        // completionResult.completion_list.items.emplace_back("Lua");
        //dictionary.WrapToCompletionList(completionResult.completion_list);


        ResponseMessage<CompletionResult> response = ResponseMessage(msg.id,completionResult);
        json j = response;
        return response;
    }
}


#endif //COMPLETION_H
