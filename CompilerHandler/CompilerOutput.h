//
// Created by grusted on 6/30/25.
//

#ifndef COMPILEROUTPUT_H
#define COMPILEROUTPUT_H
#include "../external/json.hpp"

namespace lsp_test {

    using json = nlohmann::json;

    enum ErrorCode{
        ER_NONE          = 0,
        ER_LEX           = 1,
        ER_SYNTAX        = 2,
        ER_UNDEF_FUNC_OR_REDEF_VAR         = 3,
        ER_PARAMS        = 4,
        ER_UNDEF_VAR_OR_NOTINIT_VAR     = 5,
        ER_FUNC_RETURN   = 6,
        ER_TYPE_COMP     = 7,
        ER_INFERENCE     = 8,
        ER_OTHER_SEM       = 9,
        ER_PARAMS_ARGS_MISMATCH = 40, // for LSP
        ER_PARAMS_TYPE_MISMATCH = 41, // for LSP
        ER_INTERNAL      = 99
    };

    NLOHMANN_JSON_SERIALIZE_ENUM(ErrorCode, {
        {ER_NONE, 0},
        {ER_LEX, 1},
        {ER_SYNTAX, 2},
        {ER_UNDEF_FUNC_OR_REDEF_VAR, 3},
        {ER_PARAMS, 4},
        {ER_UNDEF_VAR_OR_NOTINIT_VAR, 5},
        {ER_FUNC_RETURN, 6},
        {ER_TYPE_COMP, 7},
        {ER_INFERENCE, 8},
        {ER_OTHER_SEM, 9},
        {ER_PARAMS_ARGS_MISMATCH, 40},
        {ER_PARAMS_TYPE_MISMATCH, 41},
        {ER_INTERNAL, 99}
    })

    enum ItemType {
        IT_DOUBLE,
        IT_INT,
        IT_STRING,
        IT_ANY,
        IT_NIL,
        IT_UNDEF,
        IT_BOOL,
        IT_NOT_NIL // for LSP
    };

    enum TokenTypeEnum {
        T_ITS_NOT_A_TOKEN,
        T_EXPONENT,
        T_DECIMAL,
        T_INT,
        T_EQUALS,
        T_ASSIGMENT,
        T_UNDERLINE,
        T_KEYWORD,
        T_ID,
        T_KEYWORD_NIL_POSSIBILITY,
        T_EXCLAMATION_MARK,
        T_STRING,
        T_MORE,
        T_MORE_EQUAL,
        T_LESS,
        T_LESS_EQUAL,
        T_MINUS,
        T_ARROW,
        T_TERN,
        T_COMMENT_STRING,
        T_COMMENT_BLOCK,
        T_NOT_EQUAL,
        T_DIVISION,
        //token without state
        T_COLON,
        T_PLUS,
        T_COMMA,
        T_CURVED_BRACKET_OPEN,
        T_CURVED_BRACKET_CLOSE,
        T_SQUARE_BRACKET_OPEN,
        T_SQUARE_BRACKET_CLOSE,
        T_BRACKET_OPEN,
        T_BRACKET_CLOSE,
        T_NEW_LINE,
        T_EOF,
        T_MULTIPLICATION
    };

    struct TokenType {
        TokenTypeEnum type;

        /**
         *
         * @return length of token, if can resolve from token_type, otherwise it will return 0
         */
        int get_token_length() const {
            switch (this->type) {
                case T_UNDERLINE:
                case T_ASSIGMENT:
                case T_MORE:
                case T_LESS:
                case T_MINUS:
                case T_DIVISION:
                case T_COLON:
                case T_PLUS:
                case T_EXCLAMATION_MARK:
                case T_COMMA:
                case T_CURVED_BRACKET_OPEN:
                case T_CURVED_BRACKET_CLOSE:
                case T_SQUARE_BRACKET_OPEN:
                case T_SQUARE_BRACKET_CLOSE:
                case T_BRACKET_OPEN:
                case T_BRACKET_CLOSE:
                case T_NEW_LINE:
                case T_EOF:
                case T_MULTIPLICATION:
                    return 1;
                case T_EQUALS:
                case T_MORE_EQUAL:
                case T_LESS_EQUAL:
                case T_NOT_EQUAL:
                case T_ARROW:
                case T_TERN:
                    return 2;
                default:
                    return 0;
            }
        }

        std::string get_token_string_representation() const {
            switch (this->type) {
                case T_ITS_NOT_A_TOKEN: return "T_ITS_NOT_A_TOKEN";
                case T_EXPONENT: return "T_EXPONENT";
                case T_DECIMAL: return "T_DECIMAL";
                case T_INT: return "T_INT";
                case T_EQUALS: return "T_EQUALS";
                case T_ASSIGMENT: return "T_ASSIGMENT";
                case T_UNDERLINE: return "T_UNDERLINE";
                case T_KEYWORD: return "T_KEYWORD";
                case T_ID: return "T_ID";
                case T_KEYWORD_NIL_POSSIBILITY: return "T_KEYWORD_NIL_POSSIBILITY";
                case T_EXCLAMATION_MARK: return "T_EXCLAMATION_MARK";
                case T_STRING: return "T_STRING";
                case T_MORE: return "T_MORE";
                case T_MORE_EQUAL: return "T_MORE_EQUAL";
                case T_LESS: return "T_LESS";
                case T_LESS_EQUAL: return "T_LESS_EQUAL";
                case T_MINUS: return "T_MINUS";
                case T_ARROW: return "T_ARROW";
                case T_TERN: return "T_TERN";
                case T_COMMENT_STRING: return "T_COMMENT_STRING";
                case T_COMMENT_BLOCK: return "T_COMMENT_BLOCK";
                case T_NOT_EQUAL: return "T_NOT_EQUAL";
                case T_DIVISION: return "T_DIVISION";
                case T_COLON: return "T_COLON";
                case T_PLUS: return "T_PLUS";
                case T_COMMA: return "T_COMMA";
                case T_CURVED_BRACKET_OPEN: return "T_CURVED_BRACKET_OPEN";
                case T_CURVED_BRACKET_CLOSE: return "T_CURVED_BRACKET_CLOSE";
                case T_SQUARE_BRACKET_OPEN: return "T_SQUARE_BRACKET_OPEN";
                case T_SQUARE_BRACKET_CLOSE: return "T_SQUARE_BRACKET_CLOSE";
                case T_BRACKET_OPEN: return "T_BRACKET_OPEN";
                case T_BRACKET_CLOSE: return "T_BRACKET_CLOSE";
                case T_NEW_LINE: return "T_NEW_LINE";
                case T_EOF: return "T_EOF";
                case T_MULTIPLICATION: return "T_MULTIPLICATION";
                default: return "UNKNOWN_TOKEN";
            }
        }
    };

    inline void from_json(const nlohmann::json &j, TokenType &t) {
        t.type = j.get<TokenTypeEnum>();
        // j.at("token_type").get_to(t.type);
    }

    struct ErrorBase {
        ErrorCode error_code;
        std::optional<std::string> message;
    };

    inline void from_json(const json& j, ErrorBase& eb) {
        j.at("error_code").get_to(eb.error_code);
        // eb.error_code = j.at("error_code").get<ErrorCode>();
        if (j.contains("message")) eb.message = j.at("message").get<std::string>();
    }

    struct TokenInfo {
        TokenType token_type;
        std::string token_string;
    };

    inline void from_json(const json& j, TokenInfo& t) {
        from_json(j, t.token_type);
        j.at("token_type").get_to(t.token_type);
        j.at("token_string").get_to(t.token_string);
    }

    struct CompilerOutput : ErrorBase {
        std::optional<int> line;
        std::optional<int> char_pos;
        std::optional<TokenInfo> token;
        std::optional<TokenType> verified_token_type;
        std::optional<std::string> token_content;
        std::optional<std::string> function_name;
        std::optional<int> expected_func_args;
        std::optional<int> actual_func_args;
        std::optional<ItemType> expected_param_type;
        std::optional<ItemType> actual_param_type;
        std::optional<ItemType> expected_type;
        std::optional<ItemType> actual_type;
        std::optional<std::string> variable_name;
        std::optional<bool> is_function;
        std::optional<bool> is_it_assigment;
    };

    inline void from_json(const json& j, CompilerOutput& output) {
        from_json(j, static_cast<ErrorBase &>(output));
        if (j.contains("line")) {
            output.line = j.at("line").get<int>() - 1;
        }
        if (j.contains("char_pos")) {
            output.char_pos = j.at("char_pos").get<int>() - 1;
        }
        if (j.contains("token_type") && j.contains("token_string"))
            output.token = TokenInfo{j.at("token_type"), j.at("token_string")};
        if (j.contains("verified_token_type")) output.verified_token_type = j.at("verified_token_type").get<TokenType>();
        if (j.contains("token_string")) output.token_content = j.at("token_string").get<std::string>();
        if (j.contains("function_name")) output.function_name = j.at("function_name").get<std::string>();
        if (j.contains("expected_func_args")) output.expected_func_args = j.at("expected_func_args").get<int>();
        if (j.contains("actual_func_args")) output.actual_func_args = j.at("actual_func_args").get<int>();
        if (j.contains("expected_param_type")) output.expected_param_type = j.at("expected_param_type").get<ItemType>();
        if (j.contains("actual_param_type")) output.actual_param_type = j.at("actual_param_type").get<ItemType>();
        if (j.contains("expected_type")) output.expected_type = j.at("expected_type").get<ItemType>();
        if (j.contains("actual_type")) output.actual_type = j.at("actual_type").get<ItemType>();
        if (j.contains("variable_name")) output.variable_name = j.at("variable_name").get<std::string>();
        if (j.contains("is_function")) output.is_function = j.at("is_function").get<bool>();
        if (j.contains("is_it_assigment")) output.is_it_assigment = j.at("is_it_assigment").get<bool>();
    }
}

#endif //COMPILEROUTPUT_H
