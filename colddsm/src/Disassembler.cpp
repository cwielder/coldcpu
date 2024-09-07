#include "Cold/Disassembly/Disassembler.h"

#include <sstream>

namespace colddsm = cold::disassembly;

namespace {

    std::string immediatePrettify(const s32 imm) {
        // if the immediate is a printable character, return it as a string
        // otherwise, return it as a hex value
        // this heuristic probably needs improvement

        if (imm >= 0x20 && imm <= 0x7E) {
            return "'" + std::string(1, (char)imm) + "'";
        }

        return std::to_string(imm);
    }

}

colddsm::Disassembler::Disassembler(std::vector<cold::Instruction>& program)
    : mProgram(&program)
{ }

std::string colddsm::Disassembler::disassemble() const {
    std::stringstream result;

    for (const auto instr : *mProgram) {
        const auto type = instr.getType();

        if (type >= (int)cold::Instruction::Type::Count) {
            result << "Unknown opcode: 0x" << std::hex << instr.getData() << std::dec << "\n";
        } else {
            DisassemblerFunc disassembler = sDisassemblerFuncs[(int)type];
            result << (this->*disassembler)(instr) << "\n";
        }
    }

    return result.str();
}

const colddsm::Disassembler::DisassemblerFunc colddsm::Disassembler::sDisassemblerFuncs[(int)cold::Instruction::Type::Count] = {
    &colddsm::Disassembler::disasmSETI,
    &colddsm::Disassembler::disasmSYSCALL,
    &colddsm::Disassembler::disasmADD,
    &colddsm::Disassembler::disasmADDI,
    &colddsm::Disassembler::disasmSUB,
    &colddsm::Disassembler::disasmSUBI,
    &colddsm::Disassembler::disasmMUL,
    &colddsm::Disassembler::disasmMULI,
    &colddsm::Disassembler::disasmAND,
    &colddsm::Disassembler::disasmANDI,
    &colddsm::Disassembler::disasmOR,
    &colddsm::Disassembler::disasmORI,
    &colddsm::Disassembler::disasmXOR,
    &colddsm::Disassembler::disasmXORI,
    &colddsm::Disassembler::disasmNOT,
    &colddsm::Disassembler::disasmSHIFTL,
    &colddsm::Disassembler::disasmSHIFTR,
    &colddsm::Disassembler::disasmFADD,
    &colddsm::Disassembler::disasmFSUB,
    &colddsm::Disassembler::disasmFMUL,
    &colddsm::Disassembler::disasmFDIV,
    &colddsm::Disassembler::disasmCMP,
    &colddsm::Disassembler::disasmFCMP,
    &colddsm::Disassembler::disasmB,
    &colddsm::Disassembler::disasmBGT,
    &colddsm::Disassembler::disasmBGE,
    &colddsm::Disassembler::disasmBLT,
    &colddsm::Disassembler::disasmBLE,
    &colddsm::Disassembler::disasmBEQ,
    &colddsm::Disassembler::disasmBNE,
    &colddsm::Disassembler::disasmBL,
    &colddsm::Disassembler::disasmBGTL,
    &colddsm::Disassembler::disasmBGEL,
    &colddsm::Disassembler::disasmBLTL,
    &colddsm::Disassembler::disasmBLEL,
    &colddsm::Disassembler::disasmBEQL,
    &colddsm::Disassembler::disasmBNEL,
    &colddsm::Disassembler::disasmBLR,
    &colddsm::Disassembler::disasmBGTLR,
    &colddsm::Disassembler::disasmBGELR,
    &colddsm::Disassembler::disasmBLTLR,
    &colddsm::Disassembler::disasmBLELR,
    &colddsm::Disassembler::disasmBEQLR,
    &colddsm::Disassembler::disasmBNELR,
    &colddsm::Disassembler::disasmCMPI,
    &colddsm::Disassembler::disasmLDB,
    &colddsm::Disassembler::disasmLDH,
    &colddsm::Disassembler::disasmLDW,
    &colddsm::Disassembler::disasmSTB,
    &colddsm::Disassembler::disasmSTH,
    &colddsm::Disassembler::disasmSTW,
    &colddsm::Disassembler::disasmMFLR,
    &colddsm::Disassembler::disasmMTLR,
    &colddsm::Disassembler::disasmSET
};

std::string colddsm::Disassembler::disasmSETI(const cold::Instruction& instr) const {
    const auto [reg, imm] = instr.getByteShortData();

    return "SETI r" + std::to_string((u32)reg) + ", " + immediatePrettify(imm);
}

std::string colddsm::Disassembler::disasmSYSCALL(const cold::Instruction& instr) const {
    const auto syscallType = cold::Instruction::SyscallType(instr.getData() >> 16 & 0xFF);

    std::string result = "SYSCALL ";

    switch (syscallType) {
        case cold::Instruction::SyscallType::QMB: {
            const u8 targetReg = instr.getData() >> 8 & 0xFF;

            result += "QMB, r" + std::to_string((u32)targetReg);

            break;
        }

        case cold::Instruction::SyscallType::PRINT: {
            const u8 targetReg = instr.getData() >> 8 & 0xFF;

            result += "PRINT, r" + std::to_string((u32)targetReg);

            break;
        }

        case cold::Instruction::SyscallType::IPRINT: {
            const u8 targetReg = instr.getData() >> 8 & 0xFF;

            result += "IPRINT, r" + std::to_string((u32)targetReg);

            break;
        }

        case cold::Instruction::SyscallType::FPRINT: {
            const u8 targetReg = instr.getData() >> 8 & 0xFF;

            result += "FPRINT, r" + std::to_string((u32)targetReg);

            break;
        }

        case cold::Instruction::SyscallType::HALT: {
            result += "HALT";

            break;
        }

        default: result += "Unknown"; break;
    }

    return result;
}

std::string colddsm::Disassembler::disasmADD(const cold::Instruction& instr) const {
    const auto [outReg, inReg1, inReg2] = instr.getTripleByteData();

    return "ADD r" + std::to_string((u32)outReg) + ", r" + std::to_string((u32)inReg1) + ", r" + std::to_string((u32)inReg2);
}

std::string colddsm::Disassembler::disasmADDI(const cold::Instruction& instr) const {
    const auto [outReg, inReg, value] = instr.getTripleByteData();

    return "ADDI r" + std::to_string((u32)outReg) + ", r" + std::to_string((u32)inReg) + ", " + immediatePrettify(value);
}

std::string colddsm::Disassembler::disasmSUB(const cold::Instruction& instr) const {
    const auto [outReg, inReg1, inReg2] = instr.getTripleByteData();

    return "SUB r" + std::to_string((u32)outReg) + ", r" + std::to_string((u32)inReg1) + ", r" + std::to_string((u32)inReg2);
}

std::string colddsm::Disassembler::disasmSUBI(const cold::Instruction& instr) const {
    const auto [outReg, inReg, value] = instr.getTripleByteData();

    return "SUBI r" + std::to_string((u32)outReg) + ", r" + std::to_string((u32)inReg) + ", " + immediatePrettify(value);
}

std::string colddsm::Disassembler::disasmMUL(const cold::Instruction& instr) const {
    const auto [outReg, inReg1, inReg2] = instr.getTripleByteData();

    return "MUL r" + std::to_string((u32)outReg) + ", r" + std::to_string((u32)inReg1) + ", r" + std::to_string((u32)inReg2);
}

std::string colddsm::Disassembler::disasmMULI(const cold::Instruction& instr) const {
    const auto [outReg, inReg, value] = instr.getTripleByteData();

    return "MULI r" + std::to_string((u32)outReg) + ", r" + std::to_string((u32)inReg) + ", " + immediatePrettify(value);
}

std::string colddsm::Disassembler::disasmAND(const cold::Instruction& instr) const {
    const auto [outReg, inReg1, inReg2] = instr.getTripleByteData();

    return "AND r" + std::to_string((u32)outReg) + ", r" + std::to_string((u32)inReg1) + ", r" + std::to_string((u32)inReg2);
}

std::string colddsm::Disassembler::disasmANDI(const cold::Instruction& instr) const {
    const auto [outReg, inReg, value] = instr.getTripleByteData();

    return "ANDI r" + std::to_string((u32)outReg) + ", r" + std::to_string((u32)inReg) + ", " + immediatePrettify(value);
}

std::string colddsm::Disassembler::disasmOR(const cold::Instruction& instr) const {
    const auto [outReg, inReg1, inReg2] = instr.getTripleByteData();

    return "OR r" + std::to_string((u32)outReg) + ", r" + std::to_string((u32)inReg1) + ", r" + std::to_string((u32)inReg2);
}

std::string colddsm::Disassembler::disasmORI(const cold::Instruction& instr) const {
    const auto [outReg, inReg, value] = instr.getTripleByteData();

    return "ORI r" + std::to_string((u32)outReg) + ", r" + std::to_string((u32)inReg) + ", " + immediatePrettify(value);
}

std::string colddsm::Disassembler::disasmXOR(const cold::Instruction& instr) const {
    const auto [outReg, inReg1, inReg2] = instr.getTripleByteData();

    return "XOR r" + std::to_string((u32)outReg) + ", r" + std::to_string((u32)inReg1) + ", r" + std::to_string((u32)inReg2);
}

std::string colddsm::Disassembler::disasmXORI(const cold::Instruction& instr) const {
    const auto [outReg, inReg, value] = instr.getTripleByteData();

    return "XORI r" + std::to_string((u32)outReg) + ", r" + std::to_string((u32)inReg) + ", " + immediatePrettify(value);
}

std::string colddsm::Disassembler::disasmNOT(const cold::Instruction& instr) const {
    const auto [outReg, inReg, unused] = instr.getTripleByteData();

    return "NOT r" + std::to_string((u32)outReg) + ", r" + std::to_string((u32)inReg);
}

std::string colddsm::Disassembler::disasmSHIFTL(const cold::Instruction& instr) const {
    const auto [outReg, inReg, shiftAmount] = instr.getTripleByteData();

    return "SHIFTL r" + std::to_string((u32)outReg) + ", r" + std::to_string((u32)inReg) + ", " + immediatePrettify(shiftAmount);
}

std::string colddsm::Disassembler::disasmSHIFTR(const cold::Instruction& instr) const {
    const auto [outReg, inReg, shiftAmount] = instr.getTripleByteData();

    return "SHIFTR r" + std::to_string((u32)outReg) + ", r" + std::to_string((u32)inReg) + ", " + immediatePrettify(shiftAmount);
}

std::string colddsm::Disassembler::disasmFADD(const cold::Instruction& instr) const {
    const auto [outReg, inReg1, inReg2] = instr.getTripleByteData();

    return "FADD f" + std::to_string((u32)outReg) + ", f" + std::to_string((u32)inReg1) + ", f" + std::to_string((u32)inReg2);
}

std::string colddsm::Disassembler::disasmFSUB(const cold::Instruction& instr) const {
    const auto [outReg, inReg1, inReg2] = instr.getTripleByteData();

    return "FSUB f" + std::to_string((u32)outReg) + ", f" + std::to_string((u32)inReg1) + ", f" + std::to_string((u32)inReg2);
}

std::string colddsm::Disassembler::disasmFMUL(const cold::Instruction& instr) const {
    const auto [outReg, inReg1, inReg2] = instr.getTripleByteData();

    return "FMUL f" + std::to_string((u32)outReg) + ", f" + std::to_string((u32)inReg1) + ", f" + std::to_string((u32)inReg2);
}

std::string colddsm::Disassembler::disasmFDIV(const cold::Instruction& instr) const {
    const auto [outReg, inReg1, inReg2] = instr.getTripleByteData();

    return "FDIV f" + std::to_string((u32)outReg) + ", f" + std::to_string((u32)inReg1) + ", f" + std::to_string((u32)inReg2);
}

std::string colddsm::Disassembler::disasmCMP(const cold::Instruction& instr) const {
    const auto [reg1, reg2, unused] = instr.getTripleByteData();

    return "CMP r" + std::to_string((u32)reg1) + ", r" + std::to_string((u32)reg2);
}

std::string colddsm::Disassembler::disasmFCMP(const cold::Instruction& instr) const {
    const auto [reg1, reg2, unused] = instr.getTripleByteData();

    return "FCMP f" + std::to_string((u32)reg1) + ", f" + std::to_string((u32)reg2);
}

std::string colddsm::Disassembler::disasmB(const cold::Instruction& instr) const {
    const s32 target = instr.getS24Data();

    return "B " + std::to_string((s32)target);
}

std::string colddsm::Disassembler::disasmBGT(const cold::Instruction& instr) const {
    const s32 target = instr.getS24Data();

    return "BGT " + std::to_string((s32)target);
}

std::string colddsm::Disassembler::disasmBGE(const cold::Instruction& instr) const {
    const s32 target = instr.getS24Data();

    return "BGE " + std::to_string((s32)target);
}

std::string colddsm::Disassembler::disasmBLT(const cold::Instruction& instr) const {
    const s32 target = instr.getS24Data();

    return "BLT " + std::to_string((s32)target);
}

std::string colddsm::Disassembler::disasmBLE(const cold::Instruction& instr) const {
    const s32 target = instr.getS24Data();

    return "BLE " + std::to_string((s32)target);
}

std::string colddsm::Disassembler::disasmBEQ(const cold::Instruction& instr) const {
    const s32 target = instr.getS24Data();

    return "BEQ " + std::to_string((s32)target);
}

std::string colddsm::Disassembler::disasmBNE(const cold::Instruction& instr) const {
    const s32 target = instr.getS24Data();

    return "BNE " + std::to_string((s32)target);
}

std::string colddsm::Disassembler::disasmBL(const cold::Instruction& instr) const {
    const s32 target = instr.getS24Data();

    return "BL " + std::to_string((s32)target);
}

std::string colddsm::Disassembler::disasmBGTL(const cold::Instruction& instr) const {
    const s32 target = instr.getS24Data();

    return "BGTL " + std::to_string((s32)target);
}

std::string colddsm::Disassembler::disasmBGEL(const cold::Instruction& instr) const {
    const s32 target = instr.getS24Data();

    return "BGEL " + std::to_string((s32)target);
}

std::string colddsm::Disassembler::disasmBLTL(const cold::Instruction& instr) const {
    const s32 target = instr.getS24Data();

    return "BLTL " + std::to_string((s32)target);
}

std::string colddsm::Disassembler::disasmBLEL(const cold::Instruction& instr) const {
    const s32 target = instr.getS24Data();

    return "BLEL " + std::to_string((s32)target);
}

std::string colddsm::Disassembler::disasmBEQL(const cold::Instruction& instr) const {
    const s32 target = instr.getS24Data();

    return "BEQL " + std::to_string((s32)target);
}

std::string colddsm::Disassembler::disasmBNEL(const cold::Instruction& instr) const {
    const s32 target = instr.getS24Data();

    return "BNEL " + std::to_string((s32)target);
}

std::string colddsm::Disassembler::disasmBLR(const cold::Instruction& instr) const {
    return "BLR";
}

std::string colddsm::Disassembler::disasmBGTLR(const cold::Instruction& instr) const {
    return "BGTLR";
}

std::string colddsm::Disassembler::disasmBGELR(const cold::Instruction& instr) const {
    return "BGELR";
}

std::string colddsm::Disassembler::disasmBLTLR(const cold::Instruction& instr) const {
    return "BLTLR";
}

std::string colddsm::Disassembler::disasmBLELR(const cold::Instruction& instr) const {
    return "BLELR";
}

std::string colddsm::Disassembler::disasmBEQLR(const cold::Instruction& instr) const {
    return "BEQLR";
}

std::string colddsm::Disassembler::disasmBNELR(const cold::Instruction& instr) const {
    return "BNELR";
}

std::string colddsm::Disassembler::disasmCMPI(const cold::Instruction& instr) const {
    const auto [reg, imm] = instr.getByteShortData();

    return "CMPI r" + std::to_string((u32)reg) + ", " + immediatePrettify(imm);
}

std::string colddsm::Disassembler::disasmLDB(const cold::Instruction& instr) const {
    const auto [outReg, addrReg, offsetUnsigned] = instr.getTripleByteData();
    const s8 offset = static_cast<s8>(offsetUnsigned);

    return "LDB r" + std::to_string((u32)outReg) + ", r" + std::to_string((u32)addrReg) + ", " + immediatePrettify(offset);
}

std::string colddsm::Disassembler::disasmLDH(const cold::Instruction& instr) const {
    const auto [outReg, addrReg, offsetUnsigned] = instr.getTripleByteData();
    const s8 offset = static_cast<s8>(offsetUnsigned);

    return "LDH r" + std::to_string((u32)outReg) + ", r" + std::to_string((u32)addrReg) + ", " + immediatePrettify(offset);
}

std::string colddsm::Disassembler::disasmLDW(const cold::Instruction& instr) const {
    const auto [outReg, addrReg, offsetUnsigned] = instr.getTripleByteData();
    const s8 offset = static_cast<s8>(offsetUnsigned);

    return "LDW r" + std::to_string((u32)outReg) + ", r" + std::to_string((u32)addrReg) + ", " + immediatePrettify(offset);
}

std::string colddsm::Disassembler::disasmSTB(const cold::Instruction& instr) const {
    const auto [outReg, addrReg, offsetUnsigned] = instr.getTripleByteData();
    const s8 offset = static_cast<s8>(offsetUnsigned);

    return "STB r" + std::to_string((u32)outReg) + ", r" + std::to_string((u32)addrReg) + ", " + immediatePrettify(offset);
}

std::string colddsm::Disassembler::disasmSTH(const cold::Instruction& instr) const {
    const auto [outReg, addrReg, offsetUnsigned] = instr.getTripleByteData();
    const s8 offset = static_cast<s8>(offsetUnsigned);

    return "STH r" + std::to_string((u32)outReg) + ", r" + std::to_string((u32)addrReg) + ", " + immediatePrettify(offset);
}

std::string colddsm::Disassembler::disasmSTW(const cold::Instruction& instr) const {
    const auto [outReg, addrReg, offsetUnsigned] = instr.getTripleByteData();
    const s8 offset = static_cast<s8>(offsetUnsigned);

    return "STW r" + std::to_string((u32)outReg) + ", r" + std::to_string((u32)addrReg) + ", " + immediatePrettify(offset);
}

std::string colddsm::Disassembler::disasmMFLR(const cold::Instruction& instr) const {
    const auto [outReg, unused, unused2] = instr.getTripleByteData();

    return "MFLR r" + std::to_string((u32)outReg);
}

std::string colddsm::Disassembler::disasmMTLR(const cold::Instruction& instr) const {
    const auto [inReg, unused, unused2] = instr.getTripleByteData();

    return "MTLR r" + std::to_string((u32)inReg);
}

std::string colddsm::Disassembler::disasmSET(const cold::Instruction& instr) const {
    const auto [outReg, inReg, unused] = instr.getTripleByteData();

    return "SET r" + std::to_string((u32)outReg) + ", r" + std::to_string((u32)inReg);
}
