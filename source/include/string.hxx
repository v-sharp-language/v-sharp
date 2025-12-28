#pragma once

#include <string_view>

[[nodiscard]]
inline constexpr std::string_view typeToString(Type t) noexcept
{
    switch (t)
    {
    case Type::Void:
        return "void";
    case Type::Bool:
        return "bool";
    case Type::Byte:
        return "byte";
    case Type::String:
        return "string";

    case Type::I8:
        return "i8";
    case Type::I16:
        return "i16";
    case Type::I32:
        return "i32";
    case Type::I64:
        return "i64";

    case Type::U8:
        return "u8";
    case Type::U16:
        return "u16";
    case Type::U32:
        return "u32";
    case Type::U64:
        return "u64";

    case Type::F32:
        return "f32";
    case Type::F64:
        return "f64";
    }

    return "unknown";
}

[[nodiscard]]
inline std::string_view keywordToString(const Keyword &kw) noexcept
{
    return kw.name;
}

inline std::ostream &operator<<(std::ostream &os, Type t)
{
    return os << typeToString(t);
}

inline std::ostream &operator<<(std::ostream &os, const Keyword &kw)
{
    return os << kw.name;
}