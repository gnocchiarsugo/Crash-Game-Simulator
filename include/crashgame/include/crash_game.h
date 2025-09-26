#pragma once

#include <vector>
#include <functional>
#include "data.h"

class CrashGame
{
private:
    const size_t _n_sims;
    const size_t _n_threads;
    std::function<const unsigned()> _getMulti;
    DataCollector _main_dc;

    /// @brief Plays the game and stores data inside _dc_ptr
    /// @returns Ownership of the data
    void _play(size_t &, DataCollector &);

    /// @brief Executes the simulations
    void _execute();

public:
    std::vector<WinRule> win_rules;

    /// @brief CrashGame constructor
    /// @param  n_threads number of simulations
    /// @param  n_sims of DataCollector type
    /// @param  func const std::function<const unsigned()> getMultiplier
    CrashGame(size_t &&, size_t &&, const std::function<const unsigned()> &);
    CrashGame(const size_t &, const size_t &, const std::function<const unsigned()> &);

    const WinDist &getWinDist() const { return _main_dc.getWinDist(); };
    const size_t &getSims() const { return _n_sims; };

    /// @brief Evaluate wins if _win_rules is populated
    void evaluateWins();
};
