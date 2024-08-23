#include "Cold/Memory.h"

cold::Memory::Memory(const s32 size)
    : mMemory(size)
    , mCodeSize(0)
{ }

void cold::Memory::setCode(const std::vector<cold::Instruction>& program) {
    std::memcpy(mMemory.data(), program.data(), program.size() * sizeof(Instruction));
    mCodeSize = static_cast<s32>(program.size()) * sizeof(Instruction);
}

u8& cold::Memory::readRW(const u32 address) {
    if (address >= mMemory.size() || address < mCodeSize) [[unlikely]] {
        throw std::runtime_error("Out of bounds memory access");
    }

    return mMemory[address];
}

cold::Instruction cold::Memory::readX(const u32 address) const {
    if (address >= mCodeSize) [[unlikely]] {
        throw std::runtime_error("Cannot read executable memory from non-executable address space");
    }

    if (address >= mMemory.size()) [[unlikely]] {
        throw std::runtime_error("Out of bounds memory access");
    }
    
    return *reinterpret_cast<const cold::Instruction*>(&mMemory[address]);
}
