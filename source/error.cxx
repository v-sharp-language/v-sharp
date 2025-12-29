#include <iostream>
#include <string.hxx>
#include <error.hxx>

static void printSourceLine(std::ostream &os, std::string_view source, size_t errorLine, size_t errorColumn, size_t tokenLength)
{
    size_t line = 1;
    size_t start = 0;

    for (size_t i = 0; i < source.size(); ++i)
    {
        if (line == errorLine)
        {
            start = i;
            break;
        }
        if (source[i] == '\n')
            ++line;
    }

    size_t end = start;
    while (end < source.size() && source[end] != '\n')
        ++end;

    std::string_view lineText = source.substr(start, end - start);

    os << "  " << errorLine << " | " << lineText << '\n';
    os << "    | ";

    size_t caretPos = errorColumn;
    if (caretPos > 0 && tokenLength > 0)
        caretPos -= 1;
    for (size_t i = 0; i < caretPos; ++i)
        os << ' ';
    os << "^" << '\n';
}

[[noreturn]]
void Error::report(const CompileError &err, std::string_view source)
{
    std::cerr << err.token.File << ":" << err.token.Line << ":"
              << err.token.Column << ": "
              << err.type << ": "
              << err.message << std::endl;

    printSourceLine(std::cerr, source, err.token.Line, err.token.Column, err.token.Lexeme.size());
    std::exit(EXIT_FAILURE);
}

[[noreturn]]
void Error::lexical(std::string message, const Token &token, std::string_view source)
{
    report({ErrorType::Lexical, std::move(message), token}, source);
}

[[noreturn]]
void Error::syntax(std::string message, const Token &token, std::string_view source)
{
    report({ErrorType::Syntax, std::move(message), token}, source);
}

[[noreturn]]
void Error::semantic(std::string message, const Token &token, std::string_view source)
{
    report({ErrorType::Semantic, std::move(message), token}, source);
}

[[noreturn]]
void Error::type(std::string message, const Token &token, std::string_view source)
{
    report({ErrorType::TypeError, std::move(message), token}, source);
}