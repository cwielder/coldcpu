#pragma once

#include "Cold/Instruction.h"
#include "Cold/Memory.h"
#include "Cold/Processor.h"

#include <vector>

namespace cold {

    class VirtualMachine {
    public:
        VirtualMachine(const std::vector<cold::Instruction>& program, const u32 memorySize);
        ~VirtualMachine() = default;

        void run();

    private:
        cold::Memory mMemory;
        cold::Processor mProcessor;
    };

}
