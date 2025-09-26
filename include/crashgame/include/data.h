#pragma once

#include <iostream>
#include <map>
#include <utility>

using WinDist = std::map<unsigned, unsigned>;

enum class BetType
{
    AUTO = 0,
    OVER,
    UNDER,
    RANGE,
};

std::ostream &operator<<(std::ostream &, BetType);

class WinRule
{

public:
    const double multi;
    const BetType type;
    const std::pair<unsigned, unsigned> ab;
    double RTP;
    double RTPstd;

    WinRule() = delete;
    WinRule(BetType &&, double &&, std::pair<unsigned, unsigned> &&);

    // Copy
    WinRule(const WinRule &);

    // Move
    WinRule(WinRule &&);

    /// @brief Calculates and updates the RTP and its Std
    /// @param win_dist winning distribution
    /// @param n_sims total number of simulations
    /// @returns Updates RTP and RTPstd
    void computeRTP(const WinDist &, const size_t &);
};

class DataCollector
{
private:
    WinDist _win_dist;

public:
    DataCollector() = default;

    DataCollector &operator+=(DataCollector &&);

    /// @brief Getter for win_dist
    /// @returns win_dist
    const WinDist &getWinDist() const { return _win_dist; };

    /// @brief Updates the winning distribution
    /// @param mult multiplier to update
    /// @returns Adds one to the occurrences of the multiplier
    void addWin(const unsigned &multi) { ++_win_dist[multi]; };
};
