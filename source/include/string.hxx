#pragma once

#include <string_view>
#include <ast.hxx>
#include <error.hxx>

[[nodiscard]]
inline constexpr std::string_view toString_Access(AccessType access)
{
    switch (access)
    {
    case AccessType::Default:
        return "Default";
        break;
    case AccessType::Public:
        return "Public";
        break;
    case AccessType::Private:
        return "Private";
        break;
    default:
        return "Unknown";
        break;
    }
}

[[nodiscard]]
inline constexpr std::string_view toString_Modifier(ModifierType mod)
{
    switch (mod)
    {
    case ModifierType::None:
        return "None";
        break;
    case ModifierType::Static:
        return "Static";
        break;
    case ModifierType::Virtual:
        return "Virtual";
        break;
    case ModifierType::Override:
        return "Override";
        break;
    default:
        return "Unknown";
        break;
    }
}

[[nodiscard]]
inline constexpr std::string_view toString_Type(Type t) noexcept
{
    switch (t)
    {
    case Type::Void:
        return "void";
    case Type::Boolean:
        return "boolean";
    case Type::Byte:
        return "byte";
    case Type::String:
        return "string";

    case Type::Int8:
        return "int8";
    case Type::Int16:
        return "int16";
    case Type::Int32:
        return "int32";
    case Type::Int64:
        return "int64";

    case Type::Uint8:
        return "uint8";
    case Type::Uint16:
        return "uint16";
    case Type::Uint32:
        return "uint32";
    case Type::Uint64:
        return "uint64";

    case Type::Float32:
        return "float32";
    case Type::Float64:
        return "float64";
    }

    return "unknown";
}

[[nodiscard]]
inline std::string_view toString_Error(ErrorType type)
{
    switch (type)
    {
    case ErrorType::Lexical:
        return "Lexical Error";
    case ErrorType::Syntax:
        return "Syntax Error";
    case ErrorType::Semantic:
        return "Semantic Error";
    case ErrorType::TypeError:
        return "Type Error";
    default:
        return "Unknown Error";
    }
}
[[nodiscard]]
inline constexpr std::string_view toString_Token(TokenType type) noexcept
{
    switch (type)
    {
    case TokenType::Illegal:
        return "illegal";
    case TokenType::Comment:
        return "comment";
    case TokenType::Identifier:
        return "identifier";
    case TokenType::Integer:
        return "integer literal";
    case TokenType::Unsigned:
        return "unsigned integer literal";
    case TokenType::Float:
        return "floating-point literal";
    case TokenType::Boolean:
        return "boolean literal";
    case TokenType::String:
        return "string literal";
    case TokenType::Byte:
        return "byte literal";
    case TokenType::Int8:
        return "int8 literal";
    case TokenType::Int16:
        return "int16 literal";
    case TokenType::Int32:
        return "int32 literal";
    case TokenType::Int64:
        return "int64 literal";
    case TokenType::UInt8:
        return "uint8 literal";
    case TokenType::UInt16:
        return "uint16 literal";
    case TokenType::UInt32:
        return "uint32 literal";
    case TokenType::UInt64:
        return "uint64 literal";
    case TokenType::Float32:
        return "float32 literal";
    case TokenType::Float64:
        return "float64 literal";
    case TokenType::Void:
        return "void literal";
    case TokenType::KwMatch:
        return "match";
    case TokenType::KwFor:
        return "for";
    case TokenType::KwStructure:
        return "structure";
    case TokenType::KwEnumeration:
        return "enumeration";
    case TokenType::KwDefine:
        return "define";
    case TokenType::KwTypedef:
        return "typedef";
    case TokenType::KwIf:
        return "if";
    case TokenType::KwElse:
        return "else";
    case TokenType::KwReturn:
        return "return";
    case TokenType::KwClass:
        return "class";
    case TokenType::KwVar:
        return "var";
    case TokenType::KwConst:
        return "const";
    case TokenType::KwPublic:
        return "public";
    case TokenType::KwPrivate:
        return "private";
    case TokenType::KwStatic:
        return "static";
    case TokenType::KwVirtual:
        return "virtual";
    case TokenType::KwOverride:
        return "override";
    case TokenType::KwVoid:
        return "void";
    case TokenType::KwBoolean:
        return "boolean";
    case TokenType::KwByte:
        return "byte";
    case TokenType::KwString:
        return "string";
    case TokenType::KwInt8:
        return "int8";
    case TokenType::KwInt16:
        return "int16";
    case TokenType::KwInt32:
        return "int32";
    case TokenType::KwInt64:
        return "int64";
    case TokenType::KwUInt8:
        return "uint8";
    case TokenType::KwUInt16:
        return "uint16";
    case TokenType::KwUInt32:
        return "uint32";
    case TokenType::KwUInt64:
        return "uint64";
    case TokenType::KwFloat32:
        return "float32";
    case TokenType::KwFloat64:
        return "float64";
    case TokenType::Assign:
        return "=";
    case TokenType::Plus:
        return "+";
    case TokenType::Minus:
        return "-";
    case TokenType::Asterisk:
        return "*";
    case TokenType::Slash:
        return "/";
    case TokenType::Percent:
        return "%";
    case TokenType::Equal:
        return "==";
    case TokenType::NotEqual:
        return "!=";
    case TokenType::LessThan:
        return "<";
    case TokenType::LessEqual:
        return "<=";
    case TokenType::GreaterThan:
        return ">";
    case TokenType::GreaterEqual:
        return ">=";
    case TokenType::And:
        return "&&";
    case TokenType::Or:
        return "||";
    case TokenType::Vbar:
        return "|";
    case TokenType::Colon:
        return ":";
    case TokenType::Dot:
        return ".";
    case TokenType::Not:
        return "!";
    case TokenType::LeftParen:
        return "(";
    case TokenType::RightParen:
        return ")";
    case TokenType::LeftBrace:
        return "{";
    case TokenType::RightBrace:
        return "}";
    case TokenType::LeftBracket:
        return "[";
    case TokenType::RightBracket:
        return "]";
    case TokenType::Comma:
        return ",";
    case TokenType::Semicolon:
        return ";";
    case TokenType::EndOfFile:
        return "end of file";
    }

    return "unknown token";
}

inline std::ostream &operator<<(std::ostream &os, Type t)
{
    return os << toString_Type(t);
}

inline std::ostream &operator<<(std::ostream &os, ErrorType t)
{
    return os << toString_Error(t);
}

inline std::ostream &operator<<(std::ostream &os, const AccessType &ac)
{
    return os << toString_Access(ac);
}

inline std::ostream &operator<<(std::ostream &os, const ModifierType &mod)
{
    return os << toString_Modifier(mod);
}

inline std::ostream &operator<<(std::ostream &os, TokenType t)
{
    return os << toString_Token(t);
}
