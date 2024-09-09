#include <iostream>
#include <fstream>

#include <argparse/argparse.hpp>

#include "Cold/VirtualMachine.h"

void startProgram(const std::string& path, const u32 memorySize) {
    std::ifstream file(path, std::ios::binary | std::ios::in);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file");
    }

    file.seekg(0, std::ios::end);
    const std::size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // Load program into memory
    std::vector<cold::Instruction> program(fileSize / sizeof(cold::Instruction));
    file.read(reinterpret_cast<char*>(program.data()), fileSize);
    file.close();

    try {
        cold::VirtualMachine vm(program, memorySize);
        vm.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

int main(int argc, char** argv) {
    argparse::ArgumentParser args("coldemu");
    args.add_argument("-p", "--path")
        .help("path to the program file")
        .required();
    
    args.add_argument("-m", "--memory")
        .help("memory size in bytes")
        .default_value(1024) // 1 KB
        .scan<'i', s32>();

    try {
        args.parse_args(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << args;
        return 1;
    }

    const std::string path = args.get<std::string>("--path");
    const u32 memorySize = args.get<s32>("--memory");

    try {
        startProgram(path, memorySize);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
