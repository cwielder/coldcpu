#include "Cold/Assembly/Assembler.h"
#include "Cold/Assembly/AssemblySource.h"

#include <ranges>
#include <stdexcept>

namespace coldasm = cold::assembly;

namespace {

    std::string toUpper(const std::string& str) {
        const auto strRange = str | std::views::transform(std::toupper);
        return std::string(strRange.begin(), strRange.end());
    }

    std::vector<u8>& operator<<(std::vector<u8>& out, const u8 byte) {
        out.push_back(byte);
        return out;
    }

    std::vector<u8>& operator<<(std::vector<u8>& out, const cold::Instruction::Type type) {
        out << static_cast<u8>(type);
        return out;
    }

    std::vector<u8>& operator<<(std::vector<u8>& out, const cold::Instruction::SyscallType type) {
        out << static_cast<u8>(type);
        return out;
    }

}

std::vector<u8> coldasm::Assembler::assemble(const AssemblySource& source) {
    std::vector<u8> out;

    for (const auto& line : source.getLines()) {
        const std::string mnemonic = line.substr(0, line.find(' '));

        const auto it = sAssemblerFuncs.find(toUpper(mnemonic));
        if (it == sAssemblerFuncs.end()) {
            throw std::runtime_error("Unknown mnemonic: " + mnemonic);
        }

        const auto [_, func] = *it;
        (this->*func)(out, ParameterStream{ line });
    }

    return out;
}

const std::unordered_map<std::string, coldasm::Assembler::AssemblerFunc> coldasm::Assembler::sAssemblerFuncs = {
    { "SETI", &coldasm::Assembler::compileSETI },
    { "SYSCALL", &coldasm::Assembler::compileSYSCALL },
    { "ADD", &coldasm::Assembler::compileADD },
    { "ADDI", &coldasm::Assembler::compileADDI },
    { "SUB", &coldasm::Assembler::compileSUB },
    { "SUBI", &coldasm::Assembler::compileSUBI },
    { "MUL", &coldasm::Assembler::compileMUL },
    { "MULI", &coldasm::Assembler::compileMULI },
    { "AND", &coldasm::Assembler::compileAND },
    { "ANDI", &coldasm::Assembler::compileANDI },
    { "OR", &coldasm::Assembler::compileOR },
    { "ORI", &coldasm::Assembler::compileORI },
    { "XOR", &coldasm::Assembler::compileXOR },
    { "XORI", &coldasm::Assembler::compileXORI },
    { "NOT", &coldasm::Assembler::compileNOT },
    { "SHIFTL", &coldasm::Assembler::compileSHIFTL },
    { "SHIFTR", &coldasm::Assembler::compileSHIFTR },
    { "FADD", &coldasm::Assembler::compileFADD },
    { "FSUB", &coldasm::Assembler::compileFSUB },
    { "FMUL", &coldasm::Assembler::compileFMUL },
    { "FDIV", &coldasm::Assembler::compileFDIV },
    { "CMP", &coldasm::Assembler::compileCMP },
    { "FCMP", &coldasm::Assembler::compileFCMP },
    { "CMPI", &coldasm::Assembler::compileCMPI },
    { "LDB", &coldasm::Assembler::compileLDB },
    { "LDH", &coldasm::Assembler::compileLDH },
    { "LDW", &coldasm::Assembler::compileLDW },
    { "STB", &coldasm::Assembler::compileSTB },
    { "STH", &coldasm::Assembler::compileSTH },
    { "STW", &coldasm::Assembler::compileSTW },
    { "B", &coldasm::Assembler::compileB },
    { "BGT", &coldasm::Assembler::compileBGT },
    { "BGE", &coldasm::Assembler::compileBGE },
    { "BLT", &coldasm::Assembler::compileBLT },
    { "BLE", &coldasm::Assembler::compileBLE },
    { "BEQ", &coldasm::Assembler::compileBEQ },
    { "BNE", &coldasm::Assembler::compileBNE },
    { "BL", &coldasm::Assembler::compileBL },
    { "BGTL", &coldasm::Assembler::compileBGTL },
    { "BGEL", &coldasm::Assembler::compileBGEL },
    { "BLTL", &coldasm::Assembler::compileBLTL },
    { "BLEL", &coldasm::Assembler::compileBLEL },
    { "BEQL", &coldasm::Assembler::compileBEQL },
    { "BNEL", &coldasm::Assembler::compileBNEL },
    { "BLR", &coldasm::Assembler::compileBLR },
    { "BGTLR", &coldasm::Assembler::compileBGTLR },
    { "BGELR", &coldasm::Assembler::compileBGELR },
    { "BLTLR", &coldasm::Assembler::compileBLTLR },
    { "BLELR", &coldasm::Assembler::compileBLELR },
    { "BEQLR", &coldasm::Assembler::compileBEQLR },
    { "BNELR", &coldasm::Assembler::compileBNELR },
    { "MFLR", &coldasm::Assembler::compileMFLR },
    { "MTLR", &coldasm::Assembler::compileMTLR }
};

// Helper functions

void coldasm::Assembler::compileEmpty(std::vector<u8>& out, const cold::Instruction::Type opcode) {
    out << opcode;
    out << '\0';
    out << '\0';
    out << '\0';
}

void coldasm::Assembler::compile24Imm(std::vector<u8>& out, ParameterStream line, const cold::Instruction::Type opcode) {
    s32 imm = line.getImmediateParam();

    // fix sign bit
    if (imm < 0) {
        imm = 0x1000000 + imm;
    }

    out << opcode;
    out << ((imm >> 16) & 0xFF);
    out << ((imm >> 8) & 0xFF);
    out << (imm & 0xFF);
}

void coldasm::Assembler::compileSingleReg(std::vector<u8>& out, ParameterStream line, const cold::Instruction::Type opcode) {
    const u8 reg = line.getRegisterParam();

    out << opcode;
    out << reg;
    out << '\0';
    out << '\0';
}

void coldasm::Assembler::compileSingleReg16Imm(std::vector<u8>& out, ParameterStream line, const cold::Instruction::Type opcode) {
    const u8 reg = line.getRegisterParam();
    const s32 imm = line.getImmediateParam();

    out << opcode;
    out << reg;
    out << ((imm >> 8) & 0xFF);
    out << (imm & 0xFF);
}

void coldasm::Assembler::compileDoubleReg(std::vector<u8>& out, ParameterStream line, const cold::Instruction::Type opcode) {
    const u8 reg1 = line.getRegisterParam();
    const u8 reg2 = line.getRegisterParam();

    out << opcode;
    out << reg1;
    out << reg2;
    out << '\0';
}

void coldasm::Assembler::compileDoubleReg8Imm(std::vector<u8>& out, ParameterStream line, const cold::Instruction::Type opcode) {
    const u8 reg1 = line.getRegisterParam();
    const u8 reg2 = line.getRegisterParam();
    const s32 imm = line.getImmediateParam();

    out << opcode;
    out << reg1;
    out << reg2;
    out << (imm & 0xFF);
}

void coldasm::Assembler::compileTripleReg(std::vector<u8>& out, ParameterStream line, const cold::Instruction::Type opcode) {
    const u8 reg1 = line.getRegisterParam();
    const u8 reg2 = line.getRegisterParam();
    const u8 reg3 = line.getRegisterParam();

    out << opcode;
    out << reg1;
    out << reg2;
    out << reg3;
}

// Instructions


void coldasm::Assembler::compileSETI(std::vector<u8>& out, ParameterStream line) {
    this->compileSingleReg16Imm(out, line, cold::Instruction::Type::SETI);
}

void coldasm::Assembler::compileSYSCALL(std::vector<u8>& out, ParameterStream line) {
    static const std::unordered_map<std::string, cold::Instruction::SyscallType> syscallTypes = {
        { "PRINT", cold::Instruction::SyscallType::PRINT },
        { "HALT", cold::Instruction::SyscallType::HALT },
        { "QMB", cold::Instruction::SyscallType::QMB },
        { "IPRINT", cold::Instruction::SyscallType::IPRINT },
        { "FPRINT", cold::Instruction::SyscallType::FPRINT }
    };

    const cold::Instruction::SyscallType type = syscallTypes.find(line.getStringParam())->second;

    out << cold::Instruction::Type::SYSCALL;
    out << type;

    switch (type) {
        case cold::Instruction::SyscallType::QMB:
        case cold::Instruction::SyscallType::IPRINT:
        case cold::Instruction::SyscallType::FPRINT:
        case cold::Instruction::SyscallType::PRINT: {
            s32 reg = line.getRegisterParam();

            out << (u8)reg;
            out << '\0';

            break;
        }

        case cold::Instruction::SyscallType::HALT: {
            out << '\0' << '\0';

            break;
        }
    }
}

void coldasm::Assembler::compileADD(std::vector<u8>& out, ParameterStream line) {
    this->compileTripleReg(out, line, cold::Instruction::Type::ADD);
}

void coldasm::Assembler::compileADDI(std::vector<u8>& out, ParameterStream line) {
    this->compileDoubleReg8Imm(out, line, cold::Instruction::Type::ADDI);
}

void coldasm::Assembler::compileSUB(std::vector<u8>& out, ParameterStream line) {
    this->compileTripleReg(out, line, cold::Instruction::Type::SUB);
}

void coldasm::Assembler::compileSUBI(std::vector<u8>& out, ParameterStream line) {
    this->compileDoubleReg8Imm(out, line, cold::Instruction::Type::SUBI);
}

void coldasm::Assembler::compileMUL(std::vector<u8>& out, ParameterStream line) {
    this->compileTripleReg(out, line, cold::Instruction::Type::MUL);
}

void coldasm::Assembler::compileMULI(std::vector<u8>& out, ParameterStream line) {
    this->compileDoubleReg8Imm(out, line, cold::Instruction::Type::MULI);
}

void coldasm::Assembler::compileAND(std::vector<u8>& out, ParameterStream line) {
    this->compileTripleReg(out, line, cold::Instruction::Type::AND);
}

void coldasm::Assembler::compileANDI(std::vector<u8>& out, ParameterStream line) {
    this->compileDoubleReg8Imm(out, line, cold::Instruction::Type::ANDI);
}

void coldasm::Assembler::compileOR(std::vector<u8>& out, ParameterStream line) {
    this->compileTripleReg(out, line, cold::Instruction::Type::OR);
}

void coldasm::Assembler::compileORI(std::vector<u8>& out, ParameterStream line) {
    this->compileDoubleReg8Imm(out, line, cold::Instruction::Type::ORI);
}

void coldasm::Assembler::compileXOR(std::vector<u8>& out, ParameterStream line) {
    this->compileTripleReg(out, line, cold::Instruction::Type::XOR);
}

void coldasm::Assembler::compileXORI(std::vector<u8>& out, ParameterStream line) {
    this->compileDoubleReg8Imm(out, line, cold::Instruction::Type::XORI);
}

void coldasm::Assembler::compileNOT(std::vector<u8>& out, ParameterStream line) {
    this->compileDoubleReg(out, line, cold::Instruction::Type::NOT);
}

void coldasm::Assembler::compileSHIFTL(std::vector<u8>& out, ParameterStream line) {
    this->compileDoubleReg8Imm(out, line, cold::Instruction::Type::SHIFTL);
}

void coldasm::Assembler::compileSHIFTR(std::vector<u8>& out, ParameterStream line) {
    this->compileDoubleReg8Imm(out, line, cold::Instruction::Type::SHIFTR);
}

void coldasm::Assembler::compileFADD(std::vector<u8>& out, ParameterStream line) {
    this->compileTripleReg(out, line, cold::Instruction::Type::FADD);
}

void coldasm::Assembler::compileFSUB(std::vector<u8>& out, ParameterStream line) {
    this->compileTripleReg(out, line, cold::Instruction::Type::FSUB);
}

void coldasm::Assembler::compileFMUL(std::vector<u8>& out, ParameterStream line) {
    this->compileTripleReg(out, line, cold::Instruction::Type::FMUL);
}

void coldasm::Assembler::compileFDIV(std::vector<u8>& out, ParameterStream line) {
    this->compileTripleReg(out, line, cold::Instruction::Type::FDIV);
}

void coldasm::Assembler::compileCMP(std::vector<u8>& out, ParameterStream line) {
    this->compileDoubleReg(out, line, cold::Instruction::Type::CMP);
}

void coldasm::Assembler::compileFCMP(std::vector<u8>& out, ParameterStream line) {
    this->compileDoubleReg(out, line, cold::Instruction::Type::FCMP);
}

void coldasm::Assembler::compileCMPI(std::vector<u8>& out, ParameterStream line) {
    this->compileSingleReg16Imm(out, line, cold::Instruction::Type::CMPI);
}

void coldasm::Assembler::compileLDB(std::vector<u8>& out, ParameterStream line) {
    this->compileDoubleReg8Imm(out, line, cold::Instruction::Type::LDB);
}

void coldasm::Assembler::compileLDH(std::vector<u8>& out, ParameterStream line) {
    this->compileDoubleReg8Imm(out, line, cold::Instruction::Type::LDH);
}

void coldasm::Assembler::compileLDW(std::vector<u8>& out, ParameterStream line) {
    this->compileDoubleReg8Imm(out, line, cold::Instruction::Type::LDW);
}

void coldasm::Assembler::compileSTB(std::vector<u8>& out, ParameterStream line) {
    this->compileDoubleReg8Imm(out, line, cold::Instruction::Type::STB);
}

void coldasm::Assembler::compileSTH(std::vector<u8>& out, ParameterStream line) {
    this->compileDoubleReg8Imm(out, line, cold::Instruction::Type::STH);
}

void coldasm::Assembler::compileSTW(std::vector<u8>& out, ParameterStream line) {
    this->compileDoubleReg8Imm(out, line, cold::Instruction::Type::STW);
}

void coldasm::Assembler::compileB(std::vector<u8>& out, ParameterStream line) {
    this->compile24Imm(out, line, cold::Instruction::Type::B);
}

void coldasm::Assembler::compileBGT(std::vector<u8>& out, ParameterStream line) {
    this->compile24Imm(out, line, cold::Instruction::Type::BGT);
}

void coldasm::Assembler::compileBGE(std::vector<u8>& out, ParameterStream line) {
    this->compile24Imm(out, line, cold::Instruction::Type::BGE);
}

void coldasm::Assembler::compileBLT(std::vector<u8>& out, ParameterStream line) {
    this->compile24Imm(out, line, cold::Instruction::Type::BLT);
}

void coldasm::Assembler::compileBLE(std::vector<u8>& out, ParameterStream line) {
    this->compile24Imm(out, line, cold::Instruction::Type::BLE);
}

void coldasm::Assembler::compileBEQ(std::vector<u8>& out, ParameterStream line) {
    this->compile24Imm(out, line, cold::Instruction::Type::BEQ);
}

void coldasm::Assembler::compileBNE(std::vector<u8>& out, ParameterStream line) {
    this->compile24Imm(out, line, cold::Instruction::Type::BNE);
}

void coldasm::Assembler::compileBL(std::vector<u8>& out, ParameterStream line) {
    this->compile24Imm(out, line, cold::Instruction::Type::BL);
}

void coldasm::Assembler::compileBGTL(std::vector<u8>& out, ParameterStream line) {
    this->compile24Imm(out, line, cold::Instruction::Type::BGTL);
}

void coldasm::Assembler::compileBGEL(std::vector<u8>& out, ParameterStream line) {
    this->compile24Imm(out, line, cold::Instruction::Type::BGEL);
}

void coldasm::Assembler::compileBLTL(std::vector<u8>& out, ParameterStream line) {
    this->compile24Imm(out, line, cold::Instruction::Type::BLTL);
}

void coldasm::Assembler::compileBLEL(std::vector<u8>& out, ParameterStream line) {
    this->compile24Imm(out, line, cold::Instruction::Type::BLEL);
}

void coldasm::Assembler::compileBEQL(std::vector<u8>& out, ParameterStream line) {
    this->compile24Imm(out, line, cold::Instruction::Type::BEQL);
}

void coldasm::Assembler::compileBNEL(std::vector<u8>& out, ParameterStream line) {
    this->compile24Imm(out, line, cold::Instruction::Type::BNEL);
}

void coldasm::Assembler::compileBLR(std::vector<u8>& out, ParameterStream line) {
    this->compileEmpty(out, cold::Instruction::Type::BLR);
}

void coldasm::Assembler::compileBGTLR(std::vector<u8>& out, ParameterStream line) {
    this->compileEmpty(out, cold::Instruction::Type::BGTLR);
}

void coldasm::Assembler::compileBGELR(std::vector<u8>& out, ParameterStream line) {
    this->compileEmpty(out, cold::Instruction::Type::BGELR);
}

void coldasm::Assembler::compileBLTLR(std::vector<u8>& out, ParameterStream line) {
    this->compileEmpty(out, cold::Instruction::Type::BLTLR);
}

void coldasm::Assembler::compileBLELR(std::vector<u8>& out, ParameterStream line) {
    this->compileEmpty(out, cold::Instruction::Type::BLELR);
}

void coldasm::Assembler::compileBEQLR(std::vector<u8>& out, ParameterStream line) {
    this->compileEmpty(out, cold::Instruction::Type::BEQLR);
}

void coldasm::Assembler::compileBNELR(std::vector<u8>& out, ParameterStream line) {
    this->compileEmpty(out, cold::Instruction::Type::BNELR);
}

void coldasm::Assembler::compileMFLR(std::vector<u8>& out, ParameterStream line) {
    this->compileSingleReg(out, line, cold::Instruction::Type::MFLR);
}

void coldasm::Assembler::compileMTLR(std::vector<u8>& out, ParameterStream line) {
    this->compileSingleReg(out, line, cold::Instruction::Type::MTLR);
}

