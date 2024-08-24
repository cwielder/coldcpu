#pragma once

#include "Cold/Common.h"

namespace cold {

    class Instruction {
    public:
        enum class Type : u8 { // This is used as an index into a PTMF array in cold::Processor
            SETI,
            
            SYSCALL,
            
            ADD, ADDI,
            SUB, SUBI,
            MUL, MULI,

            AND, ANDI,
            OR, ORI,
            XOR, XORI,

            NOT,

            SHIFTL, SHIFTR,

            FADD,
            FSUB,
            FMUL,
            FDIV,

            CMP,
            FCMP,

            B,
            BGT, BGE,
            BLT, BLE,
            BEQ, BNE,

            BL,
            BGTL, BGEL,
            BLTL, BLEL,
            BEQL, BNEL,

            BLR,
            BGTLR, BGELR,
            BLTLR, BLELR,
            BEQLR, BNELR,

            CMPI,

            LDB, LDH, LDW,
            STB, STH, STW,

            MFLR, MTLR,

            Count
        };

        enum class SyscallType : u8 {
            PRINT,
            HALT,
            QMB, // Query memory begin
            IPRINT,
            FPRINT,

            Count
        };

        [[nodiscard]] u8 getType() const { return mData >> 24 & 0xFF; }
        
        [[nodiscard]] u32 getData() const { return mData; }
        void setData(const u32 data) { mData = data; }

        struct SingleRegBigImmData {
            u8 reg;
            u16 imm;
        };

        [[nodiscard]] SingleRegBigImmData getByteShortData() const {
            return {
                .reg = (u8)(mData >> 16 & 0xFF),
                .imm = (u8)(mData & 0xFFFF)
            };
        }

        struct TripleRegData {
            u8 outReg, inReg1, inReg2;
        };

        [[nodiscard]] TripleRegData getTripleByteData() const {
            return {
                .outReg = (u8)(mData >> 16 & 0xFF),
                .inReg1 = (u8)(mData >> 8 & 0xFF),
                .inReg2 = (u8)(mData & 0xFF)
            };
        }

    private:
        u32 mData;
    };

    static_assert(sizeof(Instruction) == sizeof(u32), "Instruction size mismatch");

}
