#pragma once

#include "Cold/Common.h"

#include <string>

namespace cold::assembly {

    class ParameterStream {
    public:
        explicit ParameterStream(const std::string& txt);
        ~ParameterStream() = default;

        [[nodiscard]] s32 getRegisterParam();
        [[nodiscard]] std::string getStringParam();
        [[nodiscard]] s32 getImmediateParam();

    private:
        const std::string mTxt;
        const char* mRemainder;
    };

}
