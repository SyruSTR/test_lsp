//
// Created by grusted on 7/1/25.
//

#ifndef SCOPE_H
#define SCOPE_H
#include "json.hpp"
#include "../Messages/TextDocument/Params/CompletionParams.h"

namespace lsp_test {

    using json = nlohmann::json;

    enum var_type {
        IT_DOUBLE,
        IT_INT,
        IT_STRING,
        IT_ANY,
        IT_NIL,
        IT_UNDEF,
        IT_BOOL
    };

    struct variable_struct {
        std::string name;
        var_type type;
        bool is_nil_possibility;
    };

    void inline from_json(const json& j, variable_struct& var) {
        j.at("name").get_to(var.name);
        j.at("type").get_to(var.type);
        j.at("is_nil_possibility").get_to(var.is_nil_possibility);
    }

    struct function_struct {

    };

    struct scope {
        Position start;
        Position end;
        std::vector<variable_struct> variables;
        std::vector<scope> children;
    };

    void inline from_json(const json& j, scope& s) {
        j.at("start").get_to(s.start);
        j.at("end").get_to(s.end);
        j.at("variables").get_to(s.variables);
        j.at("children").get_to(s.children);
    }

    struct main_scope : scope {
        std::vector<function_struct> functions;
    };

    void inline from_json(const json& j, main_scope& s) {
        from_json(j, static_cast<scope&>(s));
        j.at("function").get_to(s);
    }
}
#endif //SCOPE_H
