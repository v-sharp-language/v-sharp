%option c++
%option noyywrap
%option nodefault
%option yylineno

%{
#include "token.hxx"
#include "error.hxx"

const std::string* Source = nullptr;
std::string currentFile;
size_t column = 1;

#define YY_USER_ACTION column += yyleng;
%}

DIGIT       [0-9]
LETTER      [a-zA-Z_]
IDENT       {LETTER}({LETTER}|{DIGIT}|')*
INT         {DIGIT}+
FLOAT       {DIGIT}+(\.{DIGIT}+)+
WS          [ \t\r]+

%%

\n                  { column = 1; }
{WS}                { /* skip */ }

"//".*              { return static_cast<int>(TokenType::Comment); }

"public"            { return static_cast<int>(TokenType::KwPublic); }
"private"           { return static_cast<int>(TokenType::KwPrivate); }
"virtual"           { return static_cast<int>(TokenType::KwVirtual); }
"override"          { return static_cast<int>(TokenType::KwOverride); }
"static"            { return static_cast<int>(TokenType::KwStatic); }
"const"             { return static_cast<int>(TokenType::KwConst); }
"var"               { return static_cast<int>(TokenType::KwVar); }
"if"                { return static_cast<int>(TokenType::KwIf); }
"else"              { return static_cast<int>(TokenType::KwElse); }
"match"             { return static_cast<int>(TokenType::KwMatch); }
"for"               { return static_cast<int>(TokenType::KwFor); }
"return"            { return static_cast<int>(TokenType::KwReturn); }
"structure"         { return static_cast<int>(TokenType::KwStructure); }
"enumeration"       { return static_cast<int>(TokenType::KwEnumeration); }
"define"            { return static_cast<int>(TokenType::KwDefine); }
"typedef"           { return static_cast<int>(TokenType::KwTypedef); }
"class"             { return static_cast<int>(TokenType::KwClass); }

"true"|"false"      { return static_cast<int>(TokenType::Boolean); }

"int8"              { return static_cast<int>(TokenType::KwInt8); }
"int16"             { return static_cast<int>(TokenType::KwInt16); }
"int32"             { return static_cast<int>(TokenType::KwInt32); }
"int64"             { return static_cast<int>(TokenType::KwInt64); }

"uint8"             { return static_cast<int>(TokenType::KwUInt8); }
"uint16"            { return static_cast<int>(TokenType::KwUInt16); }
"uint32"            { return static_cast<int>(TokenType::KwUInt32); }
"uint64"            { return static_cast<int>(TokenType::KwUInt64); }

"float32"           { return static_cast<int>(TokenType::KwFloat32); }
"float64"           { return static_cast<int>(TokenType::KwFloat64); }

"boolean"           { return static_cast<int>(TokenType::KwBoolean); }
"string"            { return static_cast<int>(TokenType::KwString); }
"byte"              { return static_cast<int>(TokenType::KwByte); }
"void"              { return static_cast<int>(TokenType::KwVoid); }

{FLOAT}             { 
                        int dotCount = 0;
                        for (int i = 0; i < yyleng; i++)
                            if (yytext[i] == '.') dotCount++;
                        if (dotCount > 1) {
                            Error::lexical(
                                "Malformed float: multiple dots", 
                                Token{TokenType::Illegal, std::string(yytext, yyleng), currentFile, (size_t)yylineno, column},
                                *Source);
                            return static_cast<int>(TokenType::Illegal);
                        }
                        return static_cast<int>(TokenType::Float);
                    }
{INT}"u"            { return static_cast<int>(TokenType::Unsigned); }
{INT}               { return static_cast<int>(TokenType::Integer); }

\"([^\\\n]|\\[nrt"\\'])*\" { return static_cast<int>(TokenType::String); }
\"([^\\\n]|\\.)*           { Error::lexical(
                                "Unterminated string literal",
                                Token{TokenType::Illegal, std::string(yytext, yyleng), currentFile, (size_t)yylineno, column},
                                *Source);
                            }
\'([^\\\n]|\\[nrt"\\'])\' { return static_cast<int>(TokenType::Byte); }
\'([^\\\n]|\\.)*          { Error::lexical(
                                "Unterminated string literal",
                                Token{TokenType::Illegal, std::string(yytext, yyleng), currentFile, (size_t)yylineno, column},
                                *Source);
                            }

{IDENT}             { return static_cast<int>(TokenType::Identifier); }

"=="                { return static_cast<int>(TokenType::Equal); }
"!="                { return static_cast<int>(TokenType::NotEqual); }
"<="                { return static_cast<int>(TokenType::LessEqual); }
">="                { return static_cast<int>(TokenType::GreaterEqual); }
"&&"                { return static_cast<int>(TokenType::And); }
"||"                { return static_cast<int>(TokenType::Or); }

"="                 { return static_cast<int>(TokenType::Assign); }
"<"                 { return static_cast<int>(TokenType::LessThan); }
">"                 { return static_cast<int>(TokenType::GreaterThan); }
"!"                 { return static_cast<int>(TokenType::Not); }

"+"                 { return static_cast<int>(TokenType::Plus); }
"-"                 { return static_cast<int>(TokenType::Minus); }
"*"                 { return static_cast<int>(TokenType::Asterisk); }
"/"                 { return static_cast<int>(TokenType::Slash); }
"%"                 { return static_cast<int>(TokenType::Percent); }

"("                 { return static_cast<int>(TokenType::LeftParen); }
")"                 { return static_cast<int>(TokenType::RightParen); }
"{"                 { return static_cast<int>(TokenType::LeftBrace); }
"}"                 { return static_cast<int>(TokenType::RightBrace); }
"["                 { return static_cast<int>(TokenType::LeftBracket); }
"]"                 { return static_cast<int>(TokenType::RightBracket); }
","                 { return static_cast<int>(TokenType::Comma); }
";"                 { return static_cast<int>(TokenType::Semicolon); }
":"                 { return static_cast<int>(TokenType::Colon); }
"."                 { return static_cast<int>(TokenType::Dot); }
"|"                 { return static_cast<int>(TokenType::Vbar); }

<<EOF>>             { return static_cast<int>(TokenType::EndOfFile); }

. { 
    Error::lexical(
        "Illegal character",
        Token{TokenType::Illegal, std::string(yytext, yyleng), currentFile, (size_t)yylineno, column},
        *Source
    );
}
