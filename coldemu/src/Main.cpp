#include <iostream>
#include <fstream>

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

int main() {
    const std::string path = "user.cold";
    const u32 memorySize = 1024; // 1KB

    startProgram(path, memorySize);
}
