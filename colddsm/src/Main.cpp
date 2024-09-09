#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include <argparse/argparse.hpp>

#include "Cold/Disassembly/Disassembler.h"
#include <Cold/Instruction.h>

void disassemble(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream file(inputFile, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + inputFile);
    }

    file.seekg(0, std::ios::end);
    const size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // Load program
    std::vector<cold::Instruction> program(fileSize / sizeof(cold::Instruction));
    file.read(reinterpret_cast<char*>(program.data()), fileSize);

    file.close();

    // Endian swap every 4 bytes
    for (cold::Instruction& instruction : program) {
        u32 data = instruction.getData();
        data = ((data >> 24) & 0xFF) | ((data << 8) & 0xFF0000) | ((data >> 8) & 0xFF00) | ((data << 24) & 0xFF000000);
        instruction.setData(data);
    }

    // Disassemble program
    std::ofstream output(outputFile);
    if (!output.is_open()) {
        throw std::runtime_error("Failed to open file: " + outputFile);
    }

    cold::disassembly::Disassembler disassembler(program);
    output << disassembler.disassemble();

    output.close();

    std::cout << "Disassembled " << inputFile << " to " << outputFile << std::endl;
}

int main(int argc, char** argv) {
    argparse::ArgumentParser args("colddsm");
    args.add_argument("-i", "--input")
        .help("input file")
        .required();

    args.add_argument("-o", "--output")
        .help("output file")
        .required();

    try {
        args.parse_args(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << args;
        return 1;
    }

    const std::string inputFile = args.get<std::string>("--input");
    const std::string outputFile = args.get<std::string>("--output");

    try {
        disassemble(inputFile, outputFile);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
