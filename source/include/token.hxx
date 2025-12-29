#pragma once

#include <unordered_map>

/**
 * @brief Enumeration of all token types in the language.
 */
enum class TokenType
{
    Illegal, /**< Represents an invalid or unrecognized token */
    Comment, /**< Represents a comment */

    // Literals
    Identifier, /**< Identifier (variable or function name) */
    Integer,    /**< All integer literals */
    Float,      /**< All floating-point literals */
    Unsigned,   /**< Unsigned integer literals */
    Int8,
    Int16,
    Int32,
    Int64, /**< Specific integer types */
    UInt8,
    UInt16,
    UInt32,
    UInt64, /**< Specific unsigned integer types */
    Float32,
    Float64, /**< Floating-point types */
    Boolean, /**< Boolean literal */
    String,  /**< String literal */
    Byte,    /**< Byte literal */
    Void,    /**< Void type */

    // Operators
    Plus,     /**< + */
    Minus,    /**< - */
    Asterisk, /**< * */
    Slash,    /**< / */
    Percent,  /**< % */

    // Delimiters
    LeftParen,    /**< ( */
    RightParen,   /**< ) */
    LeftBrace,    /**< { */
    RightBrace,   /**< } */
    LeftBracket,  /**< [ */
    RightBracket, /**< ] */
    Comma,        /**< , */
    Semicolon,    /**< ; */
    Colon,        /**< : */
    Dot,          /**< . */
    Vbar,         /**< | */

    // Assignment and comparison
    Assign,       /**< = */
    Equal,        /**< == */
    NotEqual,     /**< != */
    LessThan,     /**< < */
    GreaterThan,  /**< > */
    LessEqual,    /**< <= */
    GreaterEqual, /**< >= */

    // Logical
    Not, /**< ! */
    And, /**< && */
    Or,  /**< || */

    // Keywords
    KwPublic,
    KwPrivate,
    KwVirtual,
    KwOverride,
    KwStatic,
    KwConst,
    KwVar,
    KwIf,
    KwElse,
    KwMatch,
    KwFor,
    KwReturn,
    KwStructure,
    KwEnumeration,
    KwDefine,
    KwTypedef,
    KwClass,
    KwInt8,
    KwInt16,
    KwInt32,
    KwInt64,
    KwUInt8,
    KwUInt16,
    KwUInt32,
    KwUInt64,
    KwFloat32,
    KwFloat64,
    KwBoolean,
    KwString,
    KwByte,
    KwVoid,
    EndOfFile /**< End of file marker */
};

/**
 * @brief Represents a lexical token.
 */
struct Token
{
    TokenType Type;      /**< Type of the token */
    std::string Lexeme;  /**< Raw text of the token */
    std::string File;    /**< Source file name */
    size_t Line, Column; /**< Line and column of the token in the source */
};

/** @brief List of all keywords in the language */
inline static const std::unordered_map<std::string_view, TokenType> keywords = {
    {"public", TokenType::KwPublic},
    {"private", TokenType::KwPrivate},
    {"virtual", TokenType::KwVirtual},
    {"override", TokenType::KwOverride},
    {"static", TokenType::KwStatic},
    {"const", TokenType::KwConst},
    {"var", TokenType::KwVar},
    {"if", TokenType::KwIf},
    {"else", TokenType::KwElse},
    {"match", TokenType::KwMatch},
    {"for", TokenType::KwFor},
    {"return", TokenType::KwReturn},
    {"structure", TokenType::KwStructure},
    {"enumeration", TokenType::KwEnumeration},
    {"define", TokenType::KwDefine},
    {"typedef", TokenType::KwTypedef},
    {"class", TokenType::KwClass},
    {"true", TokenType::Boolean},
    {"false", TokenType::Boolean},
    {"int8", TokenType::KwInt8},
    {"int16", TokenType::KwInt16},
    {"int32", TokenType::KwInt32},
    {"int64", TokenType::KwInt64},
    {"uint8", TokenType::KwUInt8},
    {"uint16", TokenType::KwUInt16},
    {"uint32", TokenType::KwUInt32},
    {"uint64", TokenType::KwUInt64},
    {"float32", TokenType::KwFloat32},
    {"float64", TokenType::KwFloat64},
    {"boolean", TokenType::KwBoolean},
    {"string", TokenType::KwString},
    {"byte", TokenType::KwByte},
    {"void", TokenType::KwVoid},
};
