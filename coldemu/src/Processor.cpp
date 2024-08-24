#include "Cold/Processor.h"
#include "Cold/Memory.h"

#include <iostream>

using enum cold::Processor::Registers::CompareRegister::Flags;

cold::Processor::Processor(cold::Memory& memory)
    : mRegisters()
    , mMemory(&memory)
    , mFinished(false)
{ }

const cold::Processor::InstructionHandler cold::Processor::sInstructionHandlers[] = {
    &Processor::handleSETI,
    &Processor::handleSYSCALL,
    &Processor::handleADD,
    &Processor::handleADDI,
    &Processor::handleSUB,
    &Processor::handleSUBI,
    &Processor::handleMUL,
    &Processor::handleMULI,
    &Processor::handleAND,
    &Processor::handleANDI,
    &Processor::handleOR,
    &Processor::handleORI,
    &Processor::handleXOR,
    &Processor::handleXORI,
    &Processor::handleNOT,
    &Processor::handleSHIFTL,
    &Processor::handleSHIFTR,
    &Processor::handleFADD,
    &Processor::handleFSUB,
    &Processor::handleFMUL,
    &Processor::handleFDIV,
    &Processor::handleCMP,
    &Processor::handleFCMP,
    &Processor::handleB,
    &Processor::handleBGT,
    &Processor::handleBGE,
    &Processor::handleBLT,
    &Processor::handleBLE,
    &Processor::handleBEQ,
    &Processor::handleBNE,
    &Processor::handleBL,
    &Processor::handleBGTL,
    &Processor::handleBGEL,
    &Processor::handleBLTL,
    &Processor::handleBLEL,
    &Processor::handleBEQL,
    &Processor::handleBNEL,
    &Processor::handleBLR,
    &Processor::handleBGTLR,
    &Processor::handleBGELR,
    &Processor::handleBLTLR,
    &Processor::handleBLELR,
    &Processor::handleBEQLR,
    &Processor::handleBNELR,
    &Processor::handleCMPI,
    &Processor::handleLDB,
    &Processor::handleLDH,
    &Processor::handleLDW,
    &Processor::handleSTB,
    &Processor::handleSTH,
    &Processor::handleSTW,
    &Processor::handleMFLR,
    &Processor::handleMTLR
};

// note: byte 0 is always the instruction type

void cold::Processor::handleSETI(const cold::Instruction& instr) {
    // byte 0: 0x00
    // byte 1: target reg
    // byte 2-3: value

    const auto [reg, value] = instr.getByteShortData();

    mRegisters.gpr[reg] = value;
}

void cold::Processor::handleSYSCALL(const cold::Instruction& instr) {
    // byte 0: 0x01
    // byte 1: type
    // byte 2-3: reserved

    const auto syscallType = Instruction::SyscallType(instr.getData() >> 16 & 0xFF);

    switch (syscallType) {
        case Instruction::SyscallType::PRINT: {
            // byte 2: data reg
            // byte 3: unused

            const u8 targetReg = instr.getData() >> 8 & 0xFF;
            const char character = mRegisters.gpr[targetReg] & 0xFF;

            std::cout << character;
            
            break;
        }

        case Instruction::SyscallType::HALT: {
            // byte 2-3: unused

            mFinished = true;

            break;
        }

        case Instruction::SyscallType::QMB: {
            // byte 2: output reg
            // byte 3: unused

            const u8 outReg = instr.getData() >> 8 & 0xFF;
            mRegisters.gpr[outReg] = mMemory->getRWBegin();

            break;
        }

        case Instruction::SyscallType::IPRINT: {
            // byte 2: data reg
            // byte 3: unused

            const u8 targetReg = instr.getData() >> 8 & 0xFF;
            const u32 value = mRegisters.gpr[targetReg];

            std::cout << value;

            break;
        }

        case Instruction::SyscallType::FPRINT: {
            // byte 2: data reg
            // byte 3: unused

            const u8 targetReg = instr.getData() >> 8 & 0xFF;
            const f32 value = *reinterpret_cast<const f32*>(&mRegisters.gpr[targetReg]);

            std::cout << value;

            break;
        }

        default: {
            throw std::runtime_error("Invalid syscall type");
        }
    }
}

void cold::Processor::handleADD(const cold::Instruction& instr) {
    // byte 0: 0x02
    // byte 1: out reg
    // byte 2: in reg 1
    // byte 3: in reg 2
    
    const auto [outReg, inReg1, inReg2] = instr.getTripleByteData();

    mRegisters.gpr[outReg] = mRegisters.gpr[inReg1] + mRegisters.gpr[inReg2];
}

void cold::Processor::handleADDI(const cold::Instruction& instr) {
    // byte 0: 0x03
    // byte 1: out reg
    // byte 2: in reg
    // byte 3: value

    const auto [outReg, inReg, value] = instr.getTripleByteData();

    mRegisters.gpr[outReg] = mRegisters.gpr[inReg] + value;
}

void cold::Processor::handleSUB(const cold::Instruction& instr) {
    // byte 0: 0x04
    // byte 1: out reg
    // byte 2: in reg 1
    // byte 3: in reg 2

    const auto [outReg, inReg1, inReg2] = instr.getTripleByteData();

    mRegisters.gpr[outReg] = mRegisters.gpr[inReg1] - mRegisters.gpr[inReg2];
}

void cold::Processor::handleSUBI(const cold::Instruction& instr) {
    // byte 0: 0x05
    // byte 1: out reg
    // byte 2: in reg
    // byte 3: value

    const auto [outReg, inReg, value] = instr.getTripleByteData();

    mRegisters.gpr[outReg] = mRegisters.gpr[inReg] - value;
}

void cold::Processor::handleMUL(const cold::Instruction& instr) {
    // byte 0: 0x06
    // byte 1: out reg
    // byte 2: in reg 1
    // byte 3: in reg 2

    const auto [outReg, inReg1, inReg2] = instr.getTripleByteData();

    mRegisters.gpr[outReg] = mRegisters.gpr[inReg1] * mRegisters.gpr[inReg2];
}

void cold::Processor::handleMULI(const cold::Instruction& instr) {
    // byte 0: 0x07
    // byte 1: out reg
    // byte 2: in reg
    // byte 3: value

    const auto [outReg, inReg, value] = instr.getTripleByteData();

    mRegisters.gpr[outReg] = mRegisters.gpr[inReg] * value;
}

void cold::Processor::handleAND(const cold::Instruction& instr) {
    // byte 0: 0x08
    // byte 1: out reg
    // byte 2: in reg 1
    // byte 3: in reg 2

    const auto [outReg, inReg1, inReg2] = instr.getTripleByteData();

    mRegisters.gpr[outReg] = mRegisters.gpr[inReg1] & mRegisters.gpr[inReg2];
}

void cold::Processor::handleANDI(const cold::Instruction& instr) {
    // byte 0: 0x09
    // byte 1: out reg
    // byte 2: in reg
    // byte 3: value

    const auto [outReg, inReg, value] = instr.getTripleByteData();

    mRegisters.gpr[outReg] = mRegisters.gpr[inReg] & value;
}

void cold::Processor::handleOR(const cold::Instruction& instr) {
    // byte 0: 0x0A
    // byte 1: out reg
    // byte 2: in reg 1
    // byte 3: in reg 2

    const auto [outReg, inReg1, inReg2] = instr.getTripleByteData();

    mRegisters.gpr[outReg] = mRegisters.gpr[inReg1] | mRegisters.gpr[inReg2];
}

void cold::Processor::handleORI(const cold::Instruction& instr) {
    // byte 0: 0x0B
    // byte 1: out reg
    // byte 2: in reg
    // byte 3: value

    const auto [outReg, inReg, value] = instr.getTripleByteData();

    mRegisters.gpr[outReg] = mRegisters.gpr[inReg] | value;
}

void cold::Processor::handleXOR(const cold::Instruction& instr) {
    // byte 0: 0x0C
    // byte 1: out reg
    // byte 2: in reg 1
    // byte 3: in reg 2

    const auto [outReg, inReg1, inReg2] = instr.getTripleByteData();

    mRegisters.gpr[outReg] = mRegisters.gpr[inReg1] ^ mRegisters.gpr[inReg2];
}

void cold::Processor::handleXORI(const cold::Instruction& instr) {
    // byte 0: 0x0D
    // byte 1: out reg
    // byte 2: in reg
    // byte 3: value

    const auto [outReg, inReg, value] = instr.getTripleByteData();

    mRegisters.gpr[outReg] = mRegisters.gpr[inReg] ^ value;
}

void cold::Processor::handleNOT(const cold::Instruction& instr) {
    // byte 0: 0x0E
    // byte 1: out reg
    // byte 2: in reg
    // byte 3: unused

    const auto [outReg, inReg, _] = instr.getTripleByteData();

    mRegisters.gpr[outReg] = ~mRegisters.gpr[inReg];
}

void cold::Processor::handleSHIFTL(const cold::Instruction& instr) {
    // byte 0: 0x0F
    // byte 1: out reg
    // byte 2: in reg
    // byte 3: shift amount

    const auto [outReg, inReg, shiftAmount] = instr.getTripleByteData();

    mRegisters.gpr[outReg] = mRegisters.gpr[inReg] << shiftAmount;
}

void cold::Processor::handleSHIFTR(const cold::Instruction& instr) {
    // byte 0: 0x10
    // byte 1: out reg
    // byte 2: in reg
    // byte 3: shift amount

    const auto [outReg, inReg, shiftAmount] = instr.getTripleByteData();

    mRegisters.gpr[outReg] = mRegisters.gpr[inReg] >> shiftAmount;
}   

void cold::Processor::handleFADD(const cold::Instruction& instr) {
    // byte 0: 0x11
    // byte 1: out reg
    // byte 2: in reg 1
    // byte 3: in reg 2

    const auto [outReg, inReg1, inReg2] = instr.getTripleByteData();
    
    const f32 inReg1f = *reinterpret_cast<f32*>(&mRegisters.gpr[inReg1]);
    const f32 inReg2f = *reinterpret_cast<f32*>(&mRegisters.gpr[inReg2]);

    const f32 result = inReg1f + inReg2f;

    mRegisters.gpr[outReg] = *reinterpret_cast<const u32*>(&result);
}

void cold::Processor::handleFSUB(const cold::Instruction& instr) {
    // byte 0: 0x12
    // byte 1: out reg
    // byte 2: in reg 1
    // byte 3: in reg 2

    const auto [outReg, inReg1, inReg2] = instr.getTripleByteData();
    
    const f32 inReg1f = *reinterpret_cast<f32*>(&mRegisters.gpr[inReg1]);
    const f32 inReg2f = *reinterpret_cast<f32*>(&mRegisters.gpr[inReg2]);

    const f32 result = inReg1f - inReg2f;

    mRegisters.gpr[outReg] = *reinterpret_cast<const u32*>(&result);
}

void cold::Processor::handleFMUL(const cold::Instruction& instr) {
    // byte 0: 0x13
    // byte 1: out reg
    // byte 2: in reg 1
    // byte 3: in reg 2

    const auto [outReg, inReg1, inReg2] = instr.getTripleByteData();
    
    const f32 inReg1f = *reinterpret_cast<f32*>(&mRegisters.gpr[inReg1]);
    const f32 inReg2f = *reinterpret_cast<f32*>(&mRegisters.gpr[inReg2]);

    const f32 result = inReg1f * inReg2f;

    mRegisters.gpr[outReg] = *reinterpret_cast<const u32*>(&result);
}

void cold::Processor::handleFDIV(const cold::Instruction& instr) {
    // byte 0: 0x14
    // byte 1: out reg
    // byte 2: in reg 1
    // byte 3: in reg 2

    const auto [outReg, inReg1, inReg2] = instr.getTripleByteData();
    
    const f32 inReg1f = *reinterpret_cast<f32*>(&mRegisters.gpr[inReg1]);
    const f32 inReg2f = *reinterpret_cast<f32*>(&mRegisters.gpr[inReg2]);

    const f32 result = inReg1f / inReg2f;

    mRegisters.gpr[outReg] = *reinterpret_cast<const u32*>(&result);
}

void cold::Processor::handleCMP(const cold::Instruction& instr) {
    // byte 0: 0x15
    // byte 1: in reg 1
    // byte 2: in reg 2
    // byte 3: unused

    mRegisters.cr = 0;

    const auto [inReg1, inReg2, _] = instr.getTripleByteData();

    const u32 inReg1u = mRegisters.gpr[inReg1];
    const u32 inReg2u = mRegisters.gpr[inReg2];

    if (inReg1u > inReg2u) {
        mRegisters.cr |= GreaterThan;
    }
    
    if (inReg1u < inReg2u) {
        mRegisters.cr |= LessThan;
    }
    
    if (inReg1u == inReg2u) {
        mRegisters.cr |= Equal;
    }
}

void cold::Processor::handleFCMP(const cold::Instruction& instr) {
    // byte 0: 0x16
    // byte 1: in reg 1
    // byte 2: in reg 2
    // byte 3: unused

    const auto [inReg1, inReg2, _] = instr.getTripleByteData();

    const f32 inReg1f = *reinterpret_cast<f32*>(&mRegisters.gpr[inReg1]);
    const f32 inReg2f = *reinterpret_cast<f32*>(&mRegisters.gpr[inReg2]);

    if (inReg1f > inReg2f) {
        mRegisters.cr |= GreaterThan;
    }

    if (inReg1f < inReg2f) {
        mRegisters.cr |= LessThan;
    }

    if (inReg1f == inReg2f) {
        mRegisters.cr |= Equal;
    }
}

void cold::Processor::handleB(const cold::Instruction& instr) {
    // byte 0: 0x17
    // bytes 1-3: relative number of instructions to jump

    s32 jmpTarget = instr.getData() & 0xFFFFFF;

    // fix sign bit (jmpTarget is s24)
    if (jmpTarget & 0x800000) {
        jmpTarget |= 0xFF000000;
    }

    mRegisters.pc += jmpTarget - 1;
}

void cold::Processor::handleBGT(const cold::Instruction& instr) {
    // byte 0: 0x18
    // bytes 1-3: relative number of instructions to jump

    if (mRegisters.cr & GreaterThan) {
        this->handleB(instr);
    }
}

void cold::Processor::handleBGE(const cold::Instruction& instr) {
    // byte 0: 0x19
    // bytes 1-3: relative number of instructions to jump

    if (mRegisters.cr & GreaterThan || mRegisters.cr & Equal) {
        this->handleB(instr);
    }
}

void cold::Processor::handleBLT(const cold::Instruction& instr) {
    // byte 0: 0x1A
    // bytes 1-3: relative number of instructions to jump

    if (mRegisters.cr & LessThan) {
        this->handleB(instr);
    }
}

void cold::Processor::handleBLE(const cold::Instruction& instr) {
    // byte 0: 0x1B
    // bytes 1-3: relative number of instructions to jump

    if (mRegisters.cr & LessThan || mRegisters.cr & Equal) {
        this->handleB(instr);
    }
}

void cold::Processor::handleBEQ(const cold::Instruction& instr) {
    // byte 0: 0x1C
    // bytes 1-3: relative number of instructions to jump

    if (mRegisters.cr & Equal) {
        this->handleB(instr);
    }
}

void cold::Processor::handleBNE(const cold::Instruction& instr) {
    // byte 0: 0x1D
    // bytes 1-3: relative number of instructions to jump

    if (!(mRegisters.cr & Equal)) {
        this->handleB(instr);
    }
}

void cold::Processor::handleBL(const cold::Instruction& instr) {
    // byte 0: 0x1E
    // bytes 1-3: relative number of instructions to jump

    mRegisters.lr = mRegisters.pc;

    this->handleB(instr);
}

void cold::Processor::handleBGTL(const cold::Instruction& instr) {
    // byte 0: 0x1F
    // bytes 1-3: relative number of instructions to jump

    if (mRegisters.cr & GreaterThan) {
        this->handleBL(instr);
    }
}

void cold::Processor::handleBGEL(const cold::Instruction& instr) {
    // byte 0: 0x20
    // bytes 1-3: relative number of instructions to jump

    if (mRegisters.cr & GreaterThan || mRegisters.cr & Equal) {
        this->handleBL(instr);
    }
}

void cold::Processor::handleBLTL(const cold::Instruction& instr) {
    // byte 0: 0x21
    // bytes 1-3: relative number of instructions to jump

    if (mRegisters.cr & LessThan) {
        this->handleBL(instr);
    }
}

void cold::Processor::handleBLEL(const cold::Instruction& instr) {
    // byte 0: 0x22
    // bytes 1-3: relative number of instructions to jump

    if (mRegisters.cr & LessThan || mRegisters.cr & Equal) {
        this->handleBL(instr);
    }
}

void cold::Processor::handleBEQL(const cold::Instruction& instr) {
    // byte 0: 0x23
    // bytes 1-3: relative number of instructions to jump

    if (mRegisters.cr & Equal) {
        this->handleBL(instr);
    }
}

void cold::Processor::handleBNEL(const cold::Instruction& instr) {
    // byte 0: 0x24
    // bytes 1-3: relative number of instructions to jump

    if (!(mRegisters.cr & Equal)) {
        this->handleBL(instr);
    }
}

void cold::Processor::handleBLR(const cold::Instruction&) {
    // byte 0: 0x25
    // bytes 1-3: unused

    mRegisters.pc = mRegisters.lr;
}

void cold::Processor::handleBGTLR(const cold::Instruction& instr) {
    // byte 0: 0x26
    // bytes 1-3: unused

    if (mRegisters.cr & GreaterThan) {
        this->handleBLR(instr);
    }
}

void cold::Processor::handleBGELR(const cold::Instruction& instr) {
    // byte 0: 0x27
    // bytes 1-3: unused

    if (mRegisters.cr & GreaterThan || mRegisters.cr & Equal) {
        this->handleBLR(instr);
    }
}

void cold::Processor::handleBLTLR(const cold::Instruction& instr) {
    // byte 0: 0x28
    // bytes 1-3: unused

    if (mRegisters.cr & LessThan) {
        this->handleBLR(instr);
    }
}

void cold::Processor::handleBLELR(const cold::Instruction& instr) {
    // byte 0: 0x29
    // bytes 1-3: unused

    if (mRegisters.cr & LessThan || mRegisters.cr & Equal) {
        this->handleBLR(instr);
    }
}

void cold::Processor::handleBEQLR(const cold::Instruction& instr) {
    // byte 0: 0x2A
    // bytes 1-3: unused

    if (mRegisters.cr & Equal) {
        this->handleBLR(instr);
    }
}

void cold::Processor::handleBNELR(const cold::Instruction& instr) {
    // byte 0: 0x2B
    // bytes 1-3: unused

    if (!(mRegisters.cr & Equal)) {
        this->handleBLR(instr);
    }
}

void cold::Processor::handleCMPI(const cold::Instruction& instr) {
    // byte 0: 0x2C
    // byte 1: in reg
    // byte 2-3: value

    mRegisters.cr = 0;

    const u8 inReg = instr.getData() >> 16 & 0xFF;

    const u32 inRegu = mRegisters.gpr[inReg];
    const u32 value = instr.getData() & 0xFFFF;

    if (inRegu > value) {
        mRegisters.cr |= GreaterThan;
    }

    if (inRegu < value) {
        mRegisters.cr |= LessThan;
    }

    if (inRegu == value) {
        mRegisters.cr |= Equal;
    }
}

void cold::Processor::handleLDB(const cold::Instruction& instr) {
    // byte 0: 0x2D
    // byte 1: output reg
    // byte 2: addr reg
    // byte 3: offset (signed)

    const auto [outReg, addrReg, offsetUnsigned] = instr.getTripleByteData();
    const s8 offset = offsetUnsigned;

    mRegisters.gpr[outReg] = mMemory->readRW(mRegisters.gpr[addrReg] + offset);
}

void cold::Processor::handleLDH(const cold::Instruction& instr) {
    // byte 0: 0x2E
    // byte 1: output reg
    // byte 2: addr reg
    // byte 3: offset (signed)

    const auto [outReg, addrReg, offsetUnsigned] = instr.getTripleByteData();
    const s8 offset = offsetUnsigned;

    const u8 readByte1 = mMemory->readRW(mRegisters.gpr[addrReg] + offset);
    const u8 readByte2 = mMemory->readRW(mRegisters.gpr[addrReg] + offset + 1);

    mRegisters.gpr[outReg] = (readByte1 << 8) | readByte2;
}

void cold::Processor::handleLDW(const cold::Instruction& instr) {
    // byte 0: 0x2F
    // byte 1: output reg
    // byte 2: addr reg
    // byte 3: offset (signed)

    const auto [outReg, addrReg, offsetUnsigned] = instr.getTripleByteData();
    const s8 offset = offsetUnsigned;

    const u8 readByte1 = mMemory->readRW(mRegisters.gpr[addrReg] + offset);
    const u8 readByte2 = mMemory->readRW(mRegisters.gpr[addrReg] + offset + 1);
    const u8 readByte3 = mMemory->readRW(mRegisters.gpr[addrReg] + offset + 2);
    const u8 readByte4 = mMemory->readRW(mRegisters.gpr[addrReg] + offset + 3);

    mRegisters.gpr[outReg] = (readByte1 << 24) | (readByte2 << 16) | (readByte3 << 8) | readByte4;
}

void cold::Processor::handleSTB(const cold::Instruction& instr) {
    // byte 0: 0x30
    // byte 1: input reg
    // byte 2: addr reg
    // byte 3: offset (signed)

    const auto [inReg, addrReg, offsetUnsigned] = instr.getTripleByteData();
    const u32 inRegu = mRegisters.gpr[inReg];
    const s8 offset = offsetUnsigned;

    mMemory->readRW(mRegisters.gpr[addrReg] + offset) = mRegisters.gpr[inReg] & 0xFF;
}

void cold::Processor::handleSTH(const cold::Instruction& instr) {
    // byte 0: 0x31
    // byte 1: input reg
    // byte 2: addr reg
    // byte 3: offset (signed)

    const auto [inReg, addrReg, offsetUnsigned] = instr.getTripleByteData();
    const u32 inRegu = mRegisters.gpr[inReg];
    const s8 offset = offsetUnsigned;

    mMemory->readRW(mRegisters.gpr[addrReg] + offset) = inRegu >> 8 & 0xFF;
    mMemory->readRW(mRegisters.gpr[addrReg] + offset + 1) = inRegu & 0xFF;
}

void cold::Processor::handleSTW(const cold::Instruction& instr) {
    // byte 0: 0x32
    // byte 1: input reg
    // byte 2: addr reg
    // byte 3: offset (signed)

    const auto [inReg, addrReg, offsetUnsigned] = instr.getTripleByteData();
    const u32 inRegu = mRegisters.gpr[inReg];
    const s8 offset = offsetUnsigned;

    mMemory->readRW(mRegisters.gpr[addrReg] + offset) = inRegu >> 24 & 0xFF;
    mMemory->readRW(mRegisters.gpr[addrReg] + offset + 1) = inRegu >> 16 & 0xFF;
    mMemory->readRW(mRegisters.gpr[addrReg] + offset + 2) = inRegu >> 8 & 0xFF;
    mMemory->readRW(mRegisters.gpr[addrReg] + offset + 3) = inRegu & 0xFF;
}

void cold::Processor::handleMFLR(const cold::Instruction& instr) {
    // byte 0: 0x33
    // byte 1: output reg
    // byte 2-3: unused

    const auto [outReg, _] = instr.getByteShortData();

    mRegisters.gpr[outReg] = mRegisters.lr;
}

void cold::Processor::handleMTLR(const cold::Instruction& instr) {
    // byte 0: 0x34
    // byte 1: input reg
    // byte 2-3: unused

    const auto [inReg, _] = instr.getByteShortData();

    mRegisters.lr = mRegisters.gpr[inReg];
}

