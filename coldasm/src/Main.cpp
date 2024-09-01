#include <fstream>
#include <string>

#include "Cold/Assembly/AssemblySource.h"
#include "Cold/Assembly/Assembler.h"

void assemble(const std::string& inputPath, const std::string& outputPath) {
    std::ifstream inputFile(inputPath);
    if (!inputFile.is_open()) {
        throw std::runtime_error("Failed to open input file");
    }

    const std::string sourceCode{ std::istreambuf_iterator<char>(inputFile), {} };

    cold::assembly::AssemblySource assemblySource(sourceCode);
    cold::assembly::Assembler assembler;
    const std::vector<u8> binary = assembler.assemble(assemblySource);

    std::ofstream outputFile(outputPath, std::ios::binary);
    if (!outputFile.is_open()) {
        throw std::runtime_error("Failed to open output file");
    }

    outputFile.write(reinterpret_cast<const char*>(binary.data()), binary.size());

    inputFile.close();
}

int main() {
    const std::string inputPath = "user.asm";
    const std::string outputPath = "user.cold";

    assemble(inputPath, outputPath);

    return 0;
}
