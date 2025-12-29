#pragma once

#include <token.hxx>

enum class ErrorType
{
    Lexical,
    Syntax,
    Semantic,
    TypeError
};

struct CompileError
{
    ErrorType type;
    std::string message;
    Token token;

    CompileError(ErrorType type, std::string message, Token token)
        : type(type), message(std::move(message)), token(std::move(token)) {}
};

namespace Error
{
    [[noreturn]]
    void report(const CompileError &err, std::string_view source);

    [[noreturn]] void lexical(std::string message, const Token &token, std::string_view source);
    [[noreturn]] void syntax(std::string message, const Token &token, std::string_view source);
    [[noreturn]] void semantic(std::string message, const Token &token, std::string_view source);
    [[noreturn]] void type(std::string message, const Token &token, std::string_view source);
}