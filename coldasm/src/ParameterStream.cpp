#include "Cold/Assembly/ParameterStream.h"
#include "Cold/Processor.h"

#include <ranges>
#include <stdexcept>

namespace coldasm = cold::assembly;

namespace {

    std::string toUpper(const std::string& str) {
        const auto strRange = str | std::views::transform(std::toupper);
        return std::string(strRange.begin(), strRange.end());
    }

}

coldasm::ParameterStream::ParameterStream(const std::string& txt)
    : mTxt(txt)
    , mRemainder(mTxt.c_str())
{ }

s32 coldasm::ParameterStream::getRegisterParam() {
    // Find register in the form of rX
    const char* regStart = std::strchr(mRemainder, 'r');
    if (regStart == nullptr) {
        throw std::runtime_error("Expected register parameter");
    }

    // Find end of register (comma or end of string)
    const char* regEnd = std::strchr(regStart, ',');
    if (regEnd == nullptr) {
        regEnd = std::strchr(regStart, '\0');
    }

    // Get register number
    const std::string regStr(regStart + 1, regEnd);
    const s32 reg = std::stoi(regStr);

    if (reg >= cold::Processor::Registers::GPRArray::cGPRCount) [[unlikely]] {
        throw std::runtime_error("Invalid register number: " + regStr);
    }

    // Update remainder
    mRemainder = regEnd;

    return reg;
}

std::string coldasm::ParameterStream::getStringParam() {
    // Find string in the form of text with a comma or end of string
    // Note: It does not have quotes around it
    const char* strStart = std::strchr(mRemainder, ' ');
    if (strStart == nullptr) {
        throw std::runtime_error("Expected string parameter");
    }

    // Find end of string (comma or end of string)
    const char* strEnd = std::strchr(strStart, ',');
    if (strEnd == nullptr) {
        strEnd = std::strchr(strStart, '\0');
    }

    // Get string
    const std::string str(strStart + 1, strEnd);

    // Update remainder
    mRemainder = strEnd;

    return toUpper(str);
}

s32 coldasm::ParameterStream::getImmediateParam() {
    const std::string& remainder = std::string{ mRemainder };

    // Find immediate in the form of decimal number, hex number, or character (1, 0xFF, '\n')
    const std::size_t immStart = remainder.find_first_of("-0123456789\'");
    const std::size_t immEnd = remainder.find(' ', immStart);
    const std::string imm = remainder.substr(immStart, immEnd - immStart);
    // If it's a character literal, convert it to a number
    // If it's a hexadecimal literal, convert it to a number
    // Otherwise, assume it's a decimal literal
    s32 immVal = 0;
    if (imm[0] == '\'') {
        // Character literal
        if (imm[1] == '\\') {
            // Escape sequence
            switch (imm[2]) {
                case 'n': immVal = '\n'; break;
                case 't': immVal = '\t'; break;
                case 'r': immVal = '\r'; break;
                case '0': immVal = '\0'; break;
                case '\\': immVal = '\\'; break;
                case '\'': immVal = '\''; break;
                default: throw std::runtime_error("Invalid escape sequence: " + imm);
            }
        } else if (imm.size() != 3) {
            throw std::runtime_error("Invalid character literal: " + imm);
        } else {
            immVal = imm[1];
        }
    }
    else if (imm[0] == '0' && imm[1] == 'x') {
        // Hexadecimal literal
        immVal = std::stoi(imm, nullptr, 16);
    }
    else if (imm[0] == '-') {
        // Negative literal
        if (imm[1] == '0' && imm[2] == 'x') {
            // Hexadecimal negative literal
            char* literalNumbers = new char[imm.size() - 3];
            std::copy(imm.begin() + 3, imm.end(), literalNumbers);
            immVal = -std::stoi(literalNumbers, nullptr, 16);
            delete[] literalNumbers;
        }
        else {
            // Decimal negative literal
            char* literalNumbers = new char[imm.size() - 1];
            std::copy(imm.begin() + 1, imm.end(), literalNumbers);
            immVal = -std::stoi(literalNumbers, nullptr, 10);
            delete[] literalNumbers;
        }
    }
    else {
        // Decimal literal
        immVal = std::stoi(imm, nullptr, 10);
    }

    // Update remainder
    mRemainder = remainder.c_str() + immEnd;

    return immVal;
}
