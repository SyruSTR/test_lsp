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

    NLOHMANN_JSON_SERIALIZE_ENUM(ItemType, {
        {IT_DOUBLE, 0},
        {IT_INT, 1},
        {IT_STRING, 2},
        {IT_ANY, 3},
        {IT_NIL, 4},
        {IT_UNDEF, 5},
        {IT_BOOL, 6},
        {IT_NOT_NIL, 7}
    })

    inline std::string to_string(const ItemType type) {
        switch (type) {
            case IT_DOUBLE:   return "Double";
            case IT_INT:      return "Int";
            case IT_STRING:   return "String";
            case IT_BOOL:     return "Bool";
            case IT_ANY:      return "any";
            case IT_NIL:      return "nil";
            case IT_UNDEF:    return "undef";
            case IT_NOT_NIL:  return "not_nil";
            default:          return "unknown";
        }
    }

    enum TokenType {
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

    inline std::string to_string(const TokenType type) {
         switch (type) {
                case T_ITS_NOT_A_TOKEN: return "T_ITS_NOT_A_TOKEN";
                case T_EXPONENT: return "T_EXPONENT";
                case T_DECIMAL: return "T_DECIMAL";
                case T_INT: return "T_INT";
                case T_EQUALS: return "T_EQUALS";
                case T_ASSIGMENT: return "T_ASSIGMENT";
                case T_UNDERLINE: return "T_UNDERLINE";
                case T_KEYWORD: return "T_KEYWORD";
                case T_ID: return "T_IDENTIFIER";
                case T_KEYWORD_NIL_POSSIBILITY: return "T_KEYWORD_NIL_POSSIBILITY";
                case T_EXCLAMATION_MARK: return "T_EXCLAMATION_MARK";
                case T_STRING: return "T_STRING";
                case T_MORE: return "T_MORE";
                case T_MORE_EQUAL: return "T_MORE_EQUAL";
                case T_LESS: return "T_LESS";
                case T_LESS_EQUAL: return "T_LESS_EQUAL";
                case T_MINUS: return "T_MINUS";
                case T_ARROW: return "T_ARROW";
                case T_TERN: return "T_TERNARY";
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
                case T_EOF: return "T_End_Of_File(EOF)";
                case T_MULTIPLICATION: return "T_MULTIPLICATION";
                default: return "UNKNOWN_TOKEN";
            }
    }

    struct ErrorBase {
        ErrorCode error_code;
        std::optional<std::string> message;
    };

    inline void from_json(const json& j, ErrorBase& e) {
        j.at("error_code").get_to(e.error_code);
        if (j.contains("message")) e.message = j.at("message").get<std::string>();
    }

    struct SourceLocation {
        int64_t line;
        int64_t char_pos;
    };

    inline void from_json(const json& j, SourceLocation& loc) {
        loc.line = j.at("line").get<int64_t>() - 1;
        loc.char_pos = j.at("char_pos").get<int64_t>() - 1;
    }

    struct TokenInfo {
        TokenType type;
        std::string token_string;

        /**
         *
         * @return length of token, if can resolve from token_type, otherwise it will return 0
         */
        int64_t get_token_length() const {
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
    };

    inline void from_json(const json& j, TokenInfo& t) {
        j.at("token_type").get_to(t.type);
        t.token_string = j.at("token_string").get<std::string>();
    }

    struct TokenError {
        TokenInfo token;
        std::optional<TokenType> verified_token_type;
        std::optional<std::string> token_content;
    };

    inline void from_json(const json& j, TokenError& e) {
        from_json(j, e.token);
        if (j.contains("verified_token_type")) e.verified_token_type = j.at("verified_token_type").get<TokenType>();
        if (j.contains("token_string")) e.token_content = j.at("token_string").get<std::string>();
    }

    struct IdentifierInfo {
        bool is_function;
    };

    inline void from_json(const json& j, IdentifierInfo& i) {
        j.at("is_function").get_to(i.is_function);
    }

    struct FunctionName {
        std::string name;
    };

    inline void from_json(const json& j, FunctionName& f) {
        j.at("function_name").get_to(f.name);
    }

    struct TypeComparison {
        ItemType expected_type;
        ItemType actual_type;
    };

    inline void from_json(const json& j, TypeComparison& t) {
        j.at("expected_type").get_to(t.expected_type);
        j.at("actual_type").get_to(t.actual_type);
    }

    struct FunctionTypeMismatch {
        FunctionName func;
        TypeComparison types;
        bool actual_is_nil_possibility;
        bool expected_is_nil_possibility;
    };

    inline void from_json(const json& j, FunctionTypeMismatch& f) {
        from_json(j, f.func);
        from_json(j, f.types);
        j.at("actual_is_nil_possibility").get_to(f.actual_is_nil_possibility);
        j.at("expected_is_nil_possibility").get_to(f.expected_is_nil_possibility);
    }

    struct ArgumentMismatch {
        FunctionName func;
        int64_t expected_func_args;
        int64_t actual_func_args;
    };

    inline void from_json(const json& j, ArgumentMismatch& a) {
        from_json(j, a.func);
        j.at("expected_func_args").get_to(a.expected_func_args);
        j.at("actual_func_args").get_to(a.actual_func_args);
    }

    struct VariableInfo {
        std::string variable_name;
        bool is_it_assigment;
    };

    inline void from_json(const json& j, VariableInfo& v) {
        j.at("variable_name").get_to(v.variable_name);
        j.at("is_it_assigment").get_to(v.is_it_assigment);
    }

    struct CompilerOutput : ErrorBase {
        std::optional<SourceLocation> location;
        std::optional<TokenError> token_error; // for ER_SYNTAX
        std::optional<IdentifierInfo> identifier_info; // for ER_ER_UNDEF_FUNC_OR_REDEF_VAR
        std::optional<FunctionTypeMismatch> func_type_mismatch; // for ER_PARAMS_TYPE_MISMATCH
        std::optional<ArgumentMismatch> argument_mismatch; // for ER_PARAMS_ARGS_MISMATCH
        std::optional<FunctionName> function_name; // for ER_INFERENCE
        std::optional<VariableInfo> variable_info; // for ER_UNDEF_VAR_OR_NOTINIT_VAR
        std::optional<TypeComparison> type_comparison; // for ER_TYPE_COMP
    };

    inline void from_json(const json& j, CompilerOutput& output) {
        from_json(j, static_cast<ErrorBase&>(output));
        if (j.contains("line") && j.contains("char_pos")) {
            output.location = j.get<SourceLocation>();
        }
        if ((output.message.has_value() && output.message.value() != "Unresolved error")
            || !output.message.has_value()) {
            switch (output.error_code) {
                case ER_UNDEF_FUNC_OR_REDEF_VAR:
                    output.identifier_info = j.get<IdentifierInfo>();
                    // dont need break
                case ER_SYNTAX:
                    output.token_error = j.get<TokenError>();
                    break;

                case ER_PARAMS_ARGS_MISMATCH:
                    output.argument_mismatch = j.get<ArgumentMismatch>();
                    break;

                case ER_PARAMS_TYPE_MISMATCH:
                    output.func_type_mismatch = j.get<FunctionTypeMismatch>();
                    break;

                case ER_UNDEF_VAR_OR_NOTINIT_VAR:
                    output.variable_info = j.get<VariableInfo>();
                    break;

                case ER_TYPE_COMP:
                    output.type_comparison = j.get<TypeComparison>();
                    break;

                default:
                    break;
            }
        }
    }
}

#endif //COMPILEROUTPUT_H
