#include <iostream>
#include <fstream>
#include <sstream>
#include "lexer.hpp"
#include "parser.hpp"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: supernova <source-file>\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    std::stringstream buffer;
    buffer << file.rdbuf();

    Lexer lexer(buffer.str());
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    parser.run();

    return 0;
}
