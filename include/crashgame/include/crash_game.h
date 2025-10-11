#pragma once

#include <utility>
#include <vector>
#include <functional>
#include <unordered_map>
#include <map>
#include <algorithm>

typedef unsigned mult_t;
typedef unsigned occ_t;
typedef std::unordered_map<unsigned, unsigned> win_dist_t;
typedef std::map<unsigned, unsigned> sorted_win_dist_t;
typedef double rtp_t;
typedef double rtp_std_t;

struct WinDist
{
    win_dist_t win_dist;
    sorted_win_dist_t sorted_win_dist;

    void add_win(const mult_t multiplier) noexcept { ++win_dist[multiplier]; };

    void sort() noexcept { sorted_win_dist.insert_range(win_dist); };

    WinDist &operator+=(WinDist &&other) noexcept
    {
        if (!other.win_dist.empty())
        {
            win_dist.merge(other.win_dist);
            for (const auto &[mult, occ] : other.win_dist)
                win_dist[mult] += occ;
        }

        if (!other.sorted_win_dist.empty())
        {
            sorted_win_dist.merge(other.sorted_win_dist);
            for (const auto &[mult, occ] : other.sorted_win_dist)
                sorted_win_dist[mult] += occ;
        }
        return *this;
    };
};

typedef std::function<const std::pair<rtp_t, rtp_std_t>(const WinDist &)> strategy_t;

class CrashGame
{
private:
    const size_t _n_sims;
    const size_t _n_threads;
    const std::function<const mult_t()> _get_multi;
    WinDist _main_wd;

    /**
     * @brief Executes the simulations
     */
    void _execute(size_t, WinDist &) noexcept;

public:
    /**
     * @brief CrashGame constructor
     * @param  n_threads number of simulations
     *  @param  n_sims of DataCollector type
     * @param  func const std::function<const unsigned()> getMultiplier
     */

    CrashGame(size_t n_threads,
              size_t n_sims,
              const std::function<const unsigned()> &get_multi)
        : _n_threads{n_threads},
          _n_sims{n_sims},
          _get_multi{get_multi} {};

    /**
     *  @brief Plays the game and stores data inside _main_dc
     */
    void play() noexcept;

    /**
     * @brief Evaluate wins if _win_rules is populated
     */

    [[nodiscard]] std::vector<std::pair<rtp_t, rtp_std_t>> evaluate(const std::vector<strategy_t> &strats) noexcept;

    const WinDist &get_wd() noexcept { return _main_wd; };
};
