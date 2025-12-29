#pragma once

#include <ast.hxx>
#include <token.hxx>
#include <flex/FlexLexer.h>

extern std::string currentFile;
extern size_t column;

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
        return Token{type, lexer.YYText(), currentFile, static_cast<size_t>(lexer.lineno()), column};
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