#include <iostream>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <config.hxx>
#include <parser.hxx>

#include <flex/FlexLexer.h>

extern const std::string *Source;

void printHelp()
{
    std::cout << "TODO" << std::endl;
}

void printVersion()
{
    std::cout << "VSharp Compiler v" << VSHARP_VERSION << std::endl;
}

void compileFile(const std::string &filename, const std::vector<std::string> &flags)
{
    if (!std::filesystem::exists(filename))
    {
        std::cerr << "File does not exist: " << filename << std::endl;
    }

    std::ifstream file(filename, std::ios::binary);
    if (!file)
    {
        std::cerr << "Cannot open file: " << filename << std::endl;
    }

    std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    if (source.empty())
    {
        std::cerr << "File is empty: " << filename << std::endl;
    }

    Source = &source;
    currentFile = filename;

    std::istringstream ss(source);
    yyFlexLexer lexer(&ss);
    Parser parser(lexer, source);

    try
    {
        ASTNodePtr ast = parser.parserProgram();

        if (std::find(flags.begin(), flags.end(), "--emit-ast") != flags.end())
        {
            printAST(ast.get());
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}