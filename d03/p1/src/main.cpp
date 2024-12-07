#include <iostream>
#include <fstream>
#include <chrono>
#include "Parser.h"

int main(int argc, char** argv) {
    std::cout << "Starting parser...\n";
    if (argc < 2) {
        std::cerr << "Enter an input filename.\n";
        return 1;
    } else if (argc > 2) {
        std::cerr << "Too many arguments.\n";
    }

    std::string filename = argv[1];
    std::ifstream input_file {filename};

    auto start {std::chrono::steady_clock::now()};
    std::string input, r_str;
    while (getline(input_file, r_str, '\n')) {
        input += r_str + '\n';
    }

    Parser parser {input};

    std::vector<Instruction*> instructions = parser.parse();

    int total = 0;
    for (Instruction* instruction : instructions) {
        total += instruction->execute();
    }

    auto end {std::chrono::steady_clock::now()};

    std::chrono::duration<double> elapsed_time {end - start};
    std::cout << "Found and executed " << instructions.size() << " instructions in "
        << elapsed_time.count() << ".\n";
    std::cout << "Result of valid instructions: " << total << "\n";

    for (Instruction* instruction : instructions) {
        delete instruction;
    }
}
