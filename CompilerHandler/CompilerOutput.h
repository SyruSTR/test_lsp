//
// Created by grusted on 6/30/25.
//

#ifndef COMPILEROUTPUT_H
#define COMPILEROUTPUT_H
#include "../external/json.hpp"

namespace lsp_test {

    using json = nlohmann::json;

    enum error_code{
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
        ER_INTERNAL      = 99
    };

    enum token_type{
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

    struct compiler_output {
        error_code error_code;
        std::string error_message;
        int line;
        int char_position;
        token_type token_type_;
        std::string token_content;

        std::string get_token_string_representation(const token_type &token) {
            switch (token) {
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

    void inline from_json(const json& j, compiler_output& o) {
        j.at("error_code").get_to(o.error_code);
        j.at("error_message").get_to(o.error_message);
        j.at("line").get_to(o.line);
        j.at("char_position").get_to(o.char_position);
        std::string token_string = o.get_token_string_representation(o.token_type_);
        j.at("token_type").get_to(token_string);
        j.at("token_content").get_to(o.token_content);
    }
}

#endif //COMPILEROUTPUT_H
