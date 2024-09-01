#pragma once

#include "Cold/Common.h"
#include "Cold/Instruction.h"

#include <stdexcept>

namespace cold {

    class Memory;

    class Processor {
    public:        
        struct Registers {
        public:
            class GPRArray {
            public:
                static constexpr u32 cGPRCount = 32;
                static constexpr u32 cStackPointerRegister = 0;

            public:
                u32& operator[](const u32 index) {
                    if (index >= cGPRCount) [[unlikely]] {
                        throw std::runtime_error("Out of bounds GPR access");
                    }

                    return mGPRs[index];
                }

            private:
                u32 mGPRs[cGPRCount];
            };
        
            class CompareRegister {
            public:
                enum class Flags {
                    GreaterThan = 1 << 0,
                    LessThan = 1 << 1,
                    Equal = 1 << 2
                };

                friend u32 operator&(const CompareRegister& cr, const Flags flag) {
                    return static_cast<u32>(cr.mFlags) & static_cast<u32>(flag);
                }

            private:
                friend class Processor;

                friend void operator|=(CompareRegister& cr, const Flags flag) {
                    cr.mFlags |= static_cast<u32>(flag);
                }

                void operator=(const u32 flags) {
                    mFlags = flags;
                }

                u32 mFlags = 0;
            };
        
        public:
            GPRArray gpr;
            CompareRegister cr;
            u32 pc = 0;
            u32 lr = 0;
        };

    public:
        Processor(Memory& memory);
        ~Processor() = default;

        using InstructionHandler = void (Processor::*)(const cold::Instruction&);
        static const InstructionHandler sInstructionHandlers[(int)Instruction::Type::Count];

        [[nodiscard]] Registers& getRegisters() { return mRegisters; }
        [[nodiscard]] bool isFinished() const { return mFinished; }

    private:
        void handleSETI(const cold::Instruction& instr);
        void handleSYSCALL(const cold::Instruction& instr);
        void handleADD(const cold::Instruction& instr);
        void handleADDI(const cold::Instruction& instr);
        void handleSUB(const cold::Instruction& instr);
        void handleSUBI(const cold::Instruction& instr);
        void handleMUL(const cold::Instruction& instr);
        void handleMULI(const cold::Instruction& instr);
        void handleAND(const cold::Instruction& instr);
        void handleANDI(const cold::Instruction& instr);
        void handleOR(const cold::Instruction& instr);
        void handleORI(const cold::Instruction& instr);
        void handleXOR(const cold::Instruction& instr);
        void handleXORI(const cold::Instruction& instr);
        void handleNOT(const cold::Instruction& instr);
        void handleSHIFTL(const cold::Instruction& instr);
        void handleSHIFTR(const cold::Instruction& instr);
        void handleFADD(const cold::Instruction& instr);
        void handleFSUB(const cold::Instruction& instr);
        void handleFMUL(const cold::Instruction& instr);
        void handleFDIV(const cold::Instruction& instr);
        void handleCMP(const cold::Instruction& instr);
        void handleFCMP(const cold::Instruction& instr);
        void handleB(const cold::Instruction& instr);
        void handleBGT(const cold::Instruction& instr);
        void handleBGE(const cold::Instruction& instr);
        void handleBLT(const cold::Instruction& instr);
        void handleBLE(const cold::Instruction& instr);
        void handleBEQ(const cold::Instruction& instr);
        void handleBNE(const cold::Instruction& instr);
        void handleBL(const cold::Instruction& instr);
        void handleBGTL(const cold::Instruction& instr);
        void handleBGEL(const cold::Instruction& instr);
        void handleBLTL(const cold::Instruction& instr);
        void handleBLEL(const cold::Instruction& instr);
        void handleBEQL(const cold::Instruction& instr);
        void handleBNEL(const cold::Instruction& instr);
        void handleBLR(const cold::Instruction& instr);
        void handleBGTLR(const cold::Instruction& instr);
        void handleBGELR(const cold::Instruction& instr);
        void handleBLTLR(const cold::Instruction& instr);
        void handleBLELR(const cold::Instruction& instr);
        void handleBEQLR(const cold::Instruction& instr);
        void handleBNELR(const cold::Instruction& instr);
        void handleCMPI(const cold::Instruction& instr);
        void handleLDB(const cold::Instruction& instr);
        void handleLDH(const cold::Instruction& instr);
        void handleLDW(const cold::Instruction& instr);
        void handleSTB(const cold::Instruction& instr);
        void handleSTH(const cold::Instruction& instr);
        void handleSTW(const cold::Instruction& instr);
        void handleMFLR(const cold::Instruction& instr);
        void handleMTLR(const cold::Instruction& instr);
        void handleSET(const cold::Instruction& instr);

    private:
        Registers mRegisters;
        Memory* mMemory;
        bool mFinished;
    };

}
