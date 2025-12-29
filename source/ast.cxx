#include <iostream>
#include <parser.hxx>
#include <string.hxx>

static void indent(std::ostream &os, int n)
{
    os << std::string(n, ' ');
}

void printAST(const ASTNode *node, int indentLevel)
{
    if (!node)
        return;

    auto ind = [&](int extra = 0)
    {
        indent(std::cout, indentLevel + extra);
    };

    switch (node->type)
    {
    case ASTNodeType::Block:
    {
        auto *blk = static_cast<const BlockNode *>(node);
        ind();
        std::cout << "Block\n";
        for (const auto &child : blk->children)
            printAST(child.get(), indentLevel + 2);
        break;
    }

    case ASTNodeType::Literal:
    {
        auto *lit = static_cast<const LiteralNode *>(node);
        ind();
        std::cout << "Literal: ";

        std::visit([](const auto &v)
                   {
            using T = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<T, bool>)
                std::cout << (v ? "true" : "false");
            else if constexpr (std::is_same_v<T, char>)
            {
                std::cout << "'";
                switch (v)
                {
                case '\n': std::cout << "\\n"; break;
                case '\t': std::cout << "\\t"; break;
                case '\r': std::cout << "\\r"; break;
                case '\\': std::cout << "\\\\"; break;
                case '\'': std::cout << "\\'"; break;
                default:   std::cout << v;
                }
                std::cout << "'";
            }
            else
                std::cout << v; }, lit->value);

        std::cout << "\n";
        break;
    }

    case ASTNodeType::Identifier:
    {
        auto *id = static_cast<const IdentifierNode *>(node);
        ind();
        std::cout << "Identifier: " << id->name << "\n";
        break;
    }

    case ASTNodeType::BinaryExpr:
    {
        auto *bin = static_cast<const BinaryExprNode *>(node);
        ind();
        std::cout << "BinaryExpr '" << bin->op << "'\n";
        printAST(bin->left.get(), indentLevel + 2);
        printAST(bin->right.get(), indentLevel + 2);
        break;
    }

    case ASTNodeType::FunctionDecl:
    {
        auto *fn = static_cast<const FunctionDeclNode *>(node);
        ind();
        std::cout << "FunctionDecl " << fn->name
                  << " [" << fn->access << "] -> "
                  << fn->returnType << "\n";

        ind(2);
        std::cout << "Params:\n";
        for (auto &p : fn->params)
        {
            ind(4);
            std::cout << p.first << " " << p.second << "\n";
        }

        ind(2);
        std::cout << "Body:\n";
        printAST(fn->body.get(), indentLevel + 4);
        break;
    }

    case ASTNodeType::ReturnExpr:
    {
        auto *ret = static_cast<const ReturnExprNode *>(node);
        ind();
        std::cout << "ReturnExpr\n";
        printAST(ret->expr.get(), indentLevel + 2);
        break;
    }

    case ASTNodeType::VarDecl:
    {
        auto *var = static_cast<const VarDeclNode *>(node);
        ind();
        std::cout
            << (var->isConst ? "ConstDecl " : "VarDecl ")
            << var->name << " : " << var->varType
            << " [" << var->access << "]\n";

        if (var->value)
        {
            ind(2);
            std::cout << "Initializer:\n";
            printAST(var->value.get(), indentLevel + 4);
        }
        break;
    }

    case ASTNodeType::IfExpr:
    {
        auto *ifn = static_cast<const IfExprNode *>(node);
        ind();
        std::cout << "IfExpr\n";

        ind(2);
        std::cout << "Condition:\n";
        printAST(ifn->condition.get(), indentLevel + 4);

        ind(2);
        std::cout << "Then:\n";
        printAST(ifn->thenBranch.get(), indentLevel + 4);

        if (ifn->elseBranch)
        {
            ind(2);
            std::cout << "Else:\n";
            printAST(ifn->elseBranch.get(), indentLevel + 4);
        }
        break;
    }

    case ASTNodeType::AssignExpr:
    {
        auto *as = static_cast<const AssignExprNode *>(node);
        ind();
        std::cout << "AssignExpr " << as->name << "\n";
        printAST(as->value.get(), indentLevel + 2);
        break;
    }

    case ASTNodeType::ClassDecl:
    {
        auto *cls = static_cast<const ClassDeclNode *>(node);
        ind();
        std::cout << "ClassDecl " << cls->name
                  << " [" << cls->access << "]\n";

        ind(2);
        std::cout << "Body:\n";
        printAST(cls->body.get(), indentLevel + 4);
        break;
    }

    default:
        ind();
        std::cout << "UnknownNode\n";
    }
}