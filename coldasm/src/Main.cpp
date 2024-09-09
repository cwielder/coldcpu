#include <fstream>
#include <string>

#include <argparse/argparse.hpp>

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

int main(int argc, char** argv) {
    argparse::ArgumentParser args("coldasm");
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

    const std::string inputPath = args.get<std::string>("--input");
    const std::string outputPath = args.get<std::string>("--output");

    try {
        assemble(inputPath, outputPath);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
