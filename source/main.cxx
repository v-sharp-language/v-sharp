#include <iostream>
#include <fstream>
#include "parser.hxx"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file>\n";
        return 1;
    }

    std::ifstream file(argv[1], std::ios::binary);
    if (!file)
    {
        std::cerr << "Cannot open file\n";
        return 1;
    }

    std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    Lexer lexer(std::move(source), argv[1]);

    Parser parser(lexer);
    try
    {
        ASTNodePtr ast = parser.parserProgram();
        printAST(ast.get());
    }
    catch (const std::exception &e)
    {
        std::cerr << "Parser error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}