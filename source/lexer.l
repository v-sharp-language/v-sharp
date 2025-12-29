%option c++
%option noyywrap
%option nodefault
%option yylineno

%{
#include "token.hxx"
#include "error.hxx"

const std::string* Source = nullptr;
std::string currentFile;
static size_t column = 1;

#define YY_USER_ACTION column += yyleng;
%}

DIGIT       [0-9]
LETTER      [a-zA-Z_]
IDENT       {LETTER}({LETTER}|{DIGIT}|')*
INT         {DIGIT}+
FLOAT       {DIGIT}+"."{DIGIT}+
WS          [ \t\r]+

%%

\n                  { column = 1; }
{WS}                { /* skip */ }

"//".*              { return (int)TokenType::Comment; }

"public"            { return (int)TokenType::KwPublic; }
"private"           { return (int)TokenType::KwPrivate; }
"virtual"           { return (int)TokenType::KwVirtual; }
"override"          { return (int)TokenType::KwOverride; }
"static"            { return (int)TokenType::KwStatic; }
"const"             { return (int)TokenType::KwConst; }
"var"               { return (int)TokenType::KwVar; }
"if"                { return (int)TokenType::KwIf; }
"else"              { return (int)TokenType::KwElse; }
"match"             { return (int)TokenType::KwMatch; }
"for"               { return (int)TokenType::KwFor; }
"return"            { return (int)TokenType::KwReturn; }
"structure"         { return (int)TokenType::KwStructure; }
"enumeration"       { return (int)TokenType::KwEnumeration; }
"define"            { return (int)TokenType::KwDefine; }
"typedef"           { return (int)TokenType::KwTypedef; }
"class"             { return (int)TokenType::KwClass; }

"true"|"false"      { return (int)TokenType::Boolean; }

"int8"              { return (int)TokenType::KwInt8; }
"int16"             { return (int)TokenType::KwInt16; }
"int32"             { return (int)TokenType::KwInt32; }
"int64"             { return (int)TokenType::KwInt64; }

"uint8"             { return (int)TokenType::KwUInt8; }
"uint16"            { return (int)TokenType::KwUInt16; }
"uint32"            { return (int)TokenType::KwUInt32; }
"uint64"            { return (int)TokenType::KwUInt64; }

"float32"           { return (int)TokenType::KwFloat32; }
"float64"           { return (int)TokenType::KwFloat64; }

"boolean"           { return (int)TokenType::KwBoolean; }
"string"            { return (int)TokenType::KwString; }
"byte"              { return (int)TokenType::KwByte; }
"void"              { return (int)TokenType::KwVoid; }

{FLOAT}             { return (int)TokenType::Float; }
{INT}"u"            { return (int)TokenType::Unsigned; }
{INT}               { return (int)TokenType::Integer; }

\"([^\\\n]|\\[nrt"\\'])*\" { return (int)TokenType::String; }
\'([^\\\n]|\\[nrt"\\'])\' { return (int)TokenType::Byte; }

{IDENT}             { return (int)TokenType::Identifier; }

"=="                { return (int)TokenType::Equal; }
"!="                { return (int)TokenType::NotEqual; }
"<="                { return (int)TokenType::LessEqual; }
">="                { return (int)TokenType::GreaterEqual; }
"&&"                { return (int)TokenType::And; }
"||"                { return (int)TokenType::Or; }

"="                 { return (int)TokenType::Assign; }
"<"                 { return (int)TokenType::LessThan; }
">"                 { return (int)TokenType::GreaterThan; }
"!"                 { return (int)TokenType::Not; }

"+"                 { return (int)TokenType::Plus; }
"-"                 { return (int)TokenType::Minus; }
"*"                 { return (int)TokenType::Asterisk; }
"/"                 { return (int)TokenType::Slash; }
"%"                 { return (int)TokenType::Percent; }

"("                 { return (int)TokenType::LeftParen; }
")"                 { return (int)TokenType::RightParen; }
"{"                 { return (int)TokenType::LeftBrace; }
"}"                 { return (int)TokenType::RightBrace; }
"["                 { return (int)TokenType::LeftBracket; }
"]"                 { return (int)TokenType::RightBracket; }
","                 { return (int)TokenType::Comma; }
";"                 { return (int)TokenType::Semicolon; }
":"                 { return (int)TokenType::Colon; }
"."                 { return (int)TokenType::Dot; }
"|"                 { return (int)TokenType::Vbar; }

<<EOF>>             { return (int)TokenType::EndOfFile; }

. { 
    Error::lexical(
        "Illegal character",
        Token{TokenType::Illegal, std::string_view(yytext, 1), currentFile, (size_t)yylineno, column},
        *Source
    );
}
