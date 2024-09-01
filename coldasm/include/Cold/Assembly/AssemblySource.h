#pragma once

#include "Cold/Common.h"

#include <sstream>
#include <vector>

namespace cold::assembly {

    class AssemblySource {
    public:
        AssemblySource(const std::string& sourceCode);
        ~AssemblySource() = default;

        [[nodiscard]] const std::vector<std::string>& getLines() const { return mLines; } // Returns lines after preprocessing

    private:
        void preprocess();

        std::vector<std::string> mLines;
    };

}
