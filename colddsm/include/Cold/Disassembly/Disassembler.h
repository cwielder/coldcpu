#pragma once

#include <Cold/Instruction.h>
#include <string>
#include <vector>

namespace cold::disassembly {

    class Disassembler {
    public:
        Disassembler(std::vector<cold::Instruction>& program);
        ~Disassembler() = default;

        std::string disassemble() const;

    private:
        using DisassemblerFunc = std::string (Disassembler::*)(const cold::Instruction& instruction) const;
        static const DisassemblerFunc sDisassemblerFuncs[(int)Instruction::Type::Count];

        std::string disasmSETI(const cold::Instruction& instr) const;
        std::string disasmSYSCALL(const cold::Instruction& instr) const;
        std::string disasmADD(const cold::Instruction& instr) const;
        std::string disasmADDI(const cold::Instruction& instr) const;
        std::string disasmSUB(const cold::Instruction& instr) const;
        std::string disasmSUBI(const cold::Instruction& instr) const;
        std::string disasmMUL(const cold::Instruction& instr) const;
        std::string disasmMULI(const cold::Instruction& instr) const;
        std::string disasmAND(const cold::Instruction& instr) const;
        std::string disasmANDI(const cold::Instruction& instr) const;
        std::string disasmOR(const cold::Instruction& instr) const;
        std::string disasmORI(const cold::Instruction& instr) const;
        std::string disasmXOR(const cold::Instruction& instr) const;
        std::string disasmXORI(const cold::Instruction& instr) const;
        std::string disasmNOT(const cold::Instruction& instr) const;
        std::string disasmSHIFTL(const cold::Instruction& instr) const;
        std::string disasmSHIFTR(const cold::Instruction& instr) const;
        std::string disasmFADD(const cold::Instruction& instr) const;
        std::string disasmFSUB(const cold::Instruction& instr) const;
        std::string disasmFMUL(const cold::Instruction& instr) const;
        std::string disasmFDIV(const cold::Instruction& instr) const;
        std::string disasmCMP(const cold::Instruction& instr) const;
        std::string disasmFCMP(const cold::Instruction& instr) const;
        std::string disasmB(const cold::Instruction& instr) const;
        std::string disasmBGT(const cold::Instruction& instr) const;
        std::string disasmBGE(const cold::Instruction& instr) const;
        std::string disasmBLT(const cold::Instruction& instr) const;
        std::string disasmBLE(const cold::Instruction& instr) const;
        std::string disasmBEQ(const cold::Instruction& instr) const;
        std::string disasmBNE(const cold::Instruction& instr) const;
        std::string disasmBL(const cold::Instruction& instr) const;
        std::string disasmBGTL(const cold::Instruction& instr) const;
        std::string disasmBGEL(const cold::Instruction& instr) const;
        std::string disasmBLTL(const cold::Instruction& instr) const;
        std::string disasmBLEL(const cold::Instruction& instr) const;
        std::string disasmBEQL(const cold::Instruction& instr) const;
        std::string disasmBNEL(const cold::Instruction& instr) const;
        std::string disasmBLR(const cold::Instruction& instr) const;
        std::string disasmBGTLR(const cold::Instruction& instr) const;
        std::string disasmBGELR(const cold::Instruction& instr) const;
        std::string disasmBLTLR(const cold::Instruction& instr) const;
        std::string disasmBLELR(const cold::Instruction& instr) const;
        std::string disasmBEQLR(const cold::Instruction& instr) const;
        std::string disasmBNELR(const cold::Instruction& instr) const;
        std::string disasmCMPI(const cold::Instruction& instr) const;
        std::string disasmLDB(const cold::Instruction& instr) const;
        std::string disasmLDH(const cold::Instruction& instr) const;
        std::string disasmLDW(const cold::Instruction& instr) const;
        std::string disasmSTB(const cold::Instruction& instr) const;
        std::string disasmSTH(const cold::Instruction& instr) const;
        std::string disasmSTW(const cold::Instruction& instr) const;
        std::string disasmMFLR(const cold::Instruction& instr) const;
        std::string disasmMTLR(const cold::Instruction& instr) const;
        std::string disasmSET(const cold::Instruction& instr) const;

        std::vector<cold::Instruction>* mProgram;
    };

}
