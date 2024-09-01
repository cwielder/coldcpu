#include "Cold/Assembly/AssemblySource.h"

#include <algorithm>

namespace coldasm = cold::assembly;

coldasm::AssemblySource::AssemblySource(const std::string& sourceCode)
    : mLines()
{
    std::stringstream codeStream(sourceCode);

    for (std::string line; std::getline(codeStream, line);) {
        mLines.push_back(line);
    }

    this->preprocess();
}

void coldasm::AssemblySource::preprocess() {
    // Remove empty lines and comments
    std::erase_if(mLines, [](const std::string& line) {
        return line.empty() || line[0] == '#';
    });

    // Split lines that contain semi-colons into multiple lines
    for (auto it = mLines.begin(); it != mLines.end(); ++it) {
        auto& line = *it;

        const size_t numSemicolons = std::count(line.begin(), line.end(), ';');

        if (numSemicolons == 0) {
            continue;
        }

        std::vector<std::string> newLines(numSemicolons + 1);

        std::size_t pos = 0;
        for (std::size_t i = 0; i < numSemicolons; ++i) {
            const std::size_t nextPos = line.find(';', pos);
            newLines[i] = line.substr(pos, nextPos - pos);
            pos = nextPos + 1;
        }

        newLines[numSemicolons] = line.substr(pos);

        // Replace the old line with the first new line
        *it = newLines[0];

        // Insert the remaining new lines after the current line
        it = mLines.insert(it + 1, newLines.begin() + 1, newLines.end());
    }

    // Remove spaces at the start of lines, and also remove double spaces
    for (auto& line : mLines) {
        std::size_t pos = 0;
        while (line[pos] == ' ') {
            ++pos;
        }

        line = line.substr(pos);

        pos = 0;
        while ((pos = line.find("  ", pos)) != std::string::npos) {
            line.erase(pos, 1);
        }
    }

    // Resolve labels
    for (auto it = mLines.begin(); it != mLines.end(); ++it) {
        if ((*it)[0] == 'B') {
            // Ignore branch to link register (ends with 'LR')
            // Get the mnemonic of the branch instruction (string before space)
            const std::string mnemonic = it->substr(0, it->find(' '));
            if (mnemonic.back() == 'R' && mnemonic[mnemonic.size() - 2] == 'L') {
                continue;
            }

            // Find the label the branch instruction wants to go to (string after space)
            const std::string label = it->substr(it->find(' ') + 1);

            // Find the line with the label
            const auto labelIt = std::find_if(mLines.begin(), mLines.end(), [&label](const std::string& line) {
                return line.substr(0, line.find(' ')) == label + ':';
            });

            if (labelIt == mLines.end()) {
                throw std::runtime_error("Could not find label: " + label);
            }

            // Calculate the offset from the branch instruction to the label
            const std::ptrdiff_t offset = std::distance(it, labelIt);
            s64 jump = 0;

            // Subtract number of labels between the branch instruction and the label
            {
                const auto first = std::min(it, labelIt);
                const auto last = std::max(it, labelIt);

                const std::size_t numLabels = std::count_if(first, last, [](const std::string& line) {
                    return line.back() == ':';
                });

                jump -= numLabels;
            }

            if (offset > 0) {
                for (auto checkerIt = it; checkerIt != labelIt; checkerIt++) {
                    const std::string& checkerLine = *checkerIt;

                    // If the line ends with a colon, it's a label, so skip it
                    if (checkerLine.back() == ':') {
                        continue;
                    }

                    jump++;
                }
            } else {
                for (auto checkerIt = it; checkerIt != labelIt; checkerIt--) {
                    const std::string& checkerLine = *checkerIt;

                    // If the line ends with a colon, it's a label, so skip it
                    if (checkerLine.back() == ':') {
                        continue;
                    }

                    jump--;
                }

                jump += 2; // The branch instruction itself is not included in the offset
            }

            // Replace the label with the offset
            *it = it->substr(0, it->find(' ')) + " " + std::to_string(jump);
        }
    }

    // Strip labels
    std::erase_if(mLines, [](const std::string& line) {
        return line.back() == ':';
    });
}
