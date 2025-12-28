#pragma once

#include <vector>
#include <memory>
#include <variant>
#include <string>
#ifdef _MSC_VER 
// MSVC does not define u_intX_t types

    #define u_int8_t uint8_t
    #define u_int16_t uint16_t
    #define u_int32_t uint32_t
    #define u_int64_t uint64_t

#endif // !_MSVC

enum class Type
{
    Void,
    Bool,
    Byte,
    String,
    I8,
    I16,
    I32,
    I64,
    U8,
    U16,
    U32,
    U64,
    F32,
    F64
};

enum class ASTNodeType
{
    Program,
    Block,

    VarDecl,
    ConstDelc,
    FunctionDecl,
    ReturnExpr,
    IfExpr,
    ForExpr,
    MatchExpr,
    BinaryExpr,
    UnaryExpr,
    Literal,
    Identifier,
    FunctionCall
};

struct ASTNode
{
    ASTNodeType type;
    ASTNode(ASTNodeType t) : type(t) {}
    virtual ~ASTNode() = default;
};

using ASTNodePtr = std::unique_ptr<ASTNode>;
using ASTNodeList = std::vector<ASTNodePtr>;

struct BlockNode : ASTNode
{
    ASTNodeList children;
    BlockNode() : ASTNode(ASTNodeType::Block) {}
};

struct LiteralNode : ASTNode
{
    Type literalType;
    std::variant<int8_t, int16_t, int32_t, int64_t, u_int8_t, u_int16_t, u_int32_t, u_int64_t, float, double, bool, char, std::string> value;
    LiteralNode(Type t, std::variant<int8_t, int16_t, int32_t, int64_t, u_int8_t, u_int16_t, u_int32_t, u_int64_t, float, double, bool, char, std::string> v)
        : ASTNode(ASTNodeType::Literal), literalType(t), value(std::move(v)) {}
};

struct TypeNode : ASTNode
{
    Type type;
    TypeNode(Type t) : ASTNode(ASTNodeType::Identifier), type(t) {}
};

struct IdentifierNode : ASTNode
{
    std::string name;
    IdentifierNode(std::string n) : ASTNode(ASTNodeType::Identifier), name(std::move(n)) {}
};

struct BinaryExprNode : ASTNode
{
    std::string op;
    ASTNodePtr left;
    ASTNodePtr right;

    BinaryExprNode(std::string o, ASTNodePtr l, ASTNodePtr r)
        : ASTNode(ASTNodeType::BinaryExpr), op(std::move(o)), left(std::move(l)), right(std::move(r)) {}
};

struct FunctionDeclNode : ASTNode
{
    std::string access;
    std::string name;
    std::vector<std::pair<Type, std::string>> params;
    Type returnType;
    ASTNodePtr body;

    FunctionDeclNode(std::string access, std::string name, std::vector<std::pair<Type, std::string>> params, Type returnType, ASTNodePtr body)
        : ASTNode(ASTNodeType::FunctionDecl), access(std::move(access)), name(std::move(name)), params(std::move(params)), returnType(returnType), body(std::move(body)) {}
};

struct ReturnExprNode : ASTNode
{
    ASTNodePtr expr;
    ReturnExprNode(ASTNodePtr e) : ASTNode(ASTNodeType::ReturnExpr), expr(std::move(e)) {}
};

void printAST(const ASTNode *node, int indent = 0);