#include "Cold/VirtualMachine.h"

#include <iostream>

std::ostream& operator<<(std::ostream& stream, cold::Processor::Registers& registers) {
   stream << "    ";

   for (u32 i = 0; i < cold::Processor::Registers::GPRArray::cGPRCount; i++) {
       stream << "r" << i << ": 0x" << std::hex << registers.gpr[i] << ", ";
   }

   stream << "\n    ";

   // float
   for (u32 i = 0; i < cold::Processor::Registers::GPRArray::cGPRCount; i++) {
   //    stream << "f" << i << ": " << *reinterpret_cast<const f32*>(&registers.gpr[i]) << ", ";
   }

   stream << std::endl;

    return stream;
}

cold::VirtualMachine::VirtualMachine(const std::vector<cold::Instruction>& program, const u32 memorySize)
    : mMemory(memorySize)
    , mProcessor(mMemory)
{
    std::vector<cold::Instruction> programEndianSwapped;
    programEndianSwapped.reserve(program.size());

    // Endian swap to little endian
    for (auto& instr : program) {
        u32 data = instr.getData();
        data = ((data >> 24) & 0xFF) | ((data << 8) & 0xFF0000) | ((data >> 8) & 0xFF00) | ((data << 24) & 0xFF000000);
        cold::Instruction instrSwapped;
        instrSwapped.setData(data);
        programEndianSwapped.push_back(instrSwapped);
    }

    // Ensure memory is large enough to hold the program
    if (program.size() > memorySize) [[unlikely]] {
        throw std::runtime_error("Program too large for memory");
    }

    mMemory.setCode(programEndianSwapped);

    // Set stack pointer to the end of the memory
    mProcessor.getRegisters().gpr[Processor::Registers::GPRArray::cStackPointerRegister] = memorySize - 1;
}

void cold::VirtualMachine::run() {
    try {
        while (!mProcessor.isFinished()) [[likely]] {
            const cold::Instruction& instr = mMemory.readX(mProcessor.getRegisters().pc * 4);

            const u8 type = instr.getType();
            if (type >= (int)cold::Instruction::Type::Count) [[unlikely]] {
                throw std::runtime_error("Invalid instruction type");
            }

            const cold::Instruction::Type enumType = static_cast<cold::Instruction::Type>(type);

            const auto handler = cold::Processor::sInstructionHandlers[type]; // function pointer
            (mProcessor.*handler)(instr);

            mProcessor.getRegisters().pc++;
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    std::cout << mProcessor.getRegisters();
}
