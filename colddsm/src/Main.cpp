#include <string>
#include <fstream>
#include <iostream>
#include <vector>

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

int main() {
    const std::string inputFile = "fibonacci.cold";
    const std::string outputFile = "fibonacci.asm";

    disassemble(inputFile, outputFile);

    return 0;
}
