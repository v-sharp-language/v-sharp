#pragma once

#include <ast.hxx>
#include <token.hxx>
#include <flex/FlexLexer.h>
#include <cstring>

struct Parser
{
    yyFlexLexer &lexer;
    Token current, nextToken;
    const std::string &Source;

    Parser(yyFlexLexer &lexer, const std::string &source)
        : lexer(lexer), Source(source)
    {
        current = next();
        nextToken = next();
    }
    void advance()
    {
        current = nextToken;
        nextToken = next();
    }

    Token next()
    {
        int tok = lexer.yylex();
        TokenType type = static_cast<TokenType>(tok);
        const char *start = lexer.YYText();
        size_t length = lexer.YYLeng();

        ptrdiff_t offset = start - Source.data();
        if (offset < 0 || static_cast<size_t>(offset) + length > Source.size())
        {
            return Token{type, std::string(start, length)};
        }
        std::string_view lexeme(Source.data() + offset, length);
        return Token { type, lexeme };
    }

    void expect(TokenType type);
    Token peekToken() const { return nextToken; }
    ASTNodePtr parserProgram();
    ASTNodePtr parseExpression(int minPrec = 1);
    ASTNodePtr parsePrimary();
    ASTNodePtr parseFunction(ASTNode *parent = nullptr);
    Type parseType();
    ASTNodePtr parseVarDecl(ASTNode *parent = nullptr);
    ASTNodePtr parseIfExpr();
    ASTNodePtr parseClassDecl();
    AccessType parseAccessModifier();
    ModifierType parseModifiers();
    ASTNodePtr parseBody(TokenType endCase = TokenType::LeftBrace, ASTNode *pc = nullptr, bool shouldAdvance = true);
    int precedence(TokenType type) const;

private:
    int getPrecedence() const { return precedence(current.Type); }
};