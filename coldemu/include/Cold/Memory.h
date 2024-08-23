#pragma once

#include "Cold/Common.h"
#include "Cold/Instruction.h"

#include <vector>
#include <stdexcept>

namespace cold {

    class Memory {
    public:
        Memory(const s32 size);
        ~Memory() = default;

        void setCode(const std::vector<cold::Instruction>& program);

        [[nodiscard]] u8& readRW(const u32 address);
        [[nodiscard]] cold::Instruction readX(const u32 address) const;

        [[nodiscard]] u32 getRWBegin() const { return mCodeSize; }

    private:
        std::vector<u8> mMemory;
        u32 mCodeSize;
    };

}
