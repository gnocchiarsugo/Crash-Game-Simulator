#pragma once

#include <string>
#include "crashgame/header.h"

namespace cg
{
    /// @brief Searches for the lowest item in the vector that is >= than target. Otherwise called least upper bound.
    /// @param F the CDF such that: it starts from != 0, ends in = 1
    /// @param target uniform random between 0 and 1
    /// @returns The index for which F.at(index) >= target
    const unsigned binarySearch(const std::vector<double> &, const double &);

    /// @brief Print all the evaluated win rules to stream
    /// @param crash_game CrashGame from which to print the rules
    void printRules(const CrashGame &);

    /// @brief Writes the winning distribution to file
    /// @param win_dist winning distribution to write
    void writeWinDist(const WinDist &, std::string&&);

} // namespace cg
