#pragma once

#include "Cold/Common.h"
#include "Cold/Instruction.h"
#include "Cold/Assembly/ParameterStream.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace cold::assembly {

    class AssemblySource;

    class Assembler {
    public:
        Assembler() = default;
        ~Assembler() = default;

        std::vector<u8> assemble(const AssemblySource& source);

    private:
        using AssemblerFunc = void (Assembler::*)(std::vector<u8>& out, ParameterStream line);
        static const std::unordered_map<std::string, AssemblerFunc> sAssemblerFuncs;

        void compileSETI(std::vector<u8>& out, ParameterStream line);
        void compileSYSCALL(std::vector<u8>& out, ParameterStream line);
        void compileADD(std::vector<u8>& out, ParameterStream line);
        void compileADDI(std::vector<u8>& out, ParameterStream line);
        void compileSUB(std::vector<u8>& out, ParameterStream line);
        void compileSUBI(std::vector<u8>& out, ParameterStream line);
        void compileMUL(std::vector<u8>& out, ParameterStream line);
        void compileMULI(std::vector<u8>& out, ParameterStream line);
        void compileAND(std::vector<u8>& out, ParameterStream line);
        void compileANDI(std::vector<u8>& out, ParameterStream line);
        void compileOR(std::vector<u8>& out, ParameterStream line);
        void compileORI(std::vector<u8>& out, ParameterStream line);
        void compileXOR(std::vector<u8>& out, ParameterStream line);
        void compileXORI(std::vector<u8>& out, ParameterStream line);
        void compileNOT(std::vector<u8>& out, ParameterStream line);
        void compileSHIFTL(std::vector<u8>& out, ParameterStream line);
        void compileSHIFTR(std::vector<u8>& out, ParameterStream line);
        void compileFADD(std::vector<u8>& out, ParameterStream line);
        void compileFSUB(std::vector<u8>& out, ParameterStream line);
        void compileFMUL(std::vector<u8>& out, ParameterStream line);
        void compileFDIV(std::vector<u8>& out, ParameterStream line);
        void compileCMP(std::vector<u8>& out, ParameterStream line);
        void compileFCMP(std::vector<u8>& out, ParameterStream line);
        void compileCMPI(std::vector<u8>& out, ParameterStream line);
        void compileLDB(std::vector<u8>& out, ParameterStream line);
        void compileLDH(std::vector<u8>& out, ParameterStream line);
        void compileLDW(std::vector<u8>& out, ParameterStream line);
        void compileSTB(std::vector<u8>& out, ParameterStream line);
        void compileSTH(std::vector<u8>& out, ParameterStream line);
        void compileSTW(std::vector<u8>& out, ParameterStream line);
        void compileB(std::vector<u8>& out, ParameterStream line);
        void compileBGT(std::vector<u8>& out, ParameterStream line);
        void compileBGE(std::vector<u8>& out, ParameterStream line);
        void compileBLT(std::vector<u8>& out, ParameterStream line);
        void compileBLE(std::vector<u8>& out, ParameterStream line);
        void compileBEQ(std::vector<u8>& out, ParameterStream line);
        void compileBNE(std::vector<u8>& out, ParameterStream line);
        void compileBL(std::vector<u8>& out, ParameterStream line);
        void compileBGTL(std::vector<u8>& out, ParameterStream line);
        void compileBGEL(std::vector<u8>& out, ParameterStream line);
        void compileBLTL(std::vector<u8>& out, ParameterStream line);
        void compileBLEL(std::vector<u8>& out, ParameterStream line);
        void compileBEQL(std::vector<u8>& out, ParameterStream line);
        void compileBNEL(std::vector<u8>& out, ParameterStream line);
        void compileBLR(std::vector<u8>& out, ParameterStream line);
        void compileBGTLR(std::vector<u8>& out, ParameterStream line);
        void compileBGELR(std::vector<u8>& out, ParameterStream line);
        void compileBLTLR(std::vector<u8>& out, ParameterStream line);
        void compileBLELR(std::vector<u8>& out, ParameterStream line);
        void compileBEQLR(std::vector<u8>& out, ParameterStream line);
        void compileBNELR(std::vector<u8>& out, ParameterStream line);
        void compileMFLR(std::vector<u8>& out, ParameterStream line);
        void compileMTLR(std::vector<u8>& out, ParameterStream line);
        void compileSET(std::vector<u8>& out, ParameterStream line);

        // Helper functions
        void compileEmpty(std::vector<u8>& out, const cold::Instruction::Type opcode);
        void compile24Imm(std::vector<u8>& out, ParameterStream line, const cold::Instruction::Type opcode);
        void compileSingleReg(std::vector<u8>& out, ParameterStream line, const cold::Instruction::Type opcode);
        void compileSingleReg16Imm(std::vector<u8>& out, ParameterStream line, const cold::Instruction::Type opcode);
        void compileDoubleReg(std::vector<u8>& out, ParameterStream line, const cold::Instruction::Type opcode);
        void compileDoubleReg8Imm(std::vector<u8>& out, ParameterStream line, const cold::Instruction::Type opcode);
        void compileTripleReg(std::vector<u8>& out, ParameterStream line, const cold::Instruction::Type opcode);
    };

}
