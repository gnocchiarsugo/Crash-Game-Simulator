#pragma once
#include "game.h"
#include "typedefs.h"
#include "slot_typedefs.h"
#include <future>
#include <array>
#include <vector>

template <class T>
class Platform
{
    const size_t _n_sims;
    const unsigned _n_threads;
    win_dist_t _main_dist;

public:
    Platform(unsigned n_threads, size_t n_sims) : _n_sims{n_sims},
                                                  _n_threads{n_threads} {};
    void simulate(const T &, const scenarios_t<T> &) noexcept;

    [[nodiscard]] const win_dist_t &get_dist() const noexcept { return _main_dist; };
};

template <class T>
void Platform<T>::simulate(const T &slot, const scenarios_t<T> &scenes) noexcept
{
    const size_t _thread_task       = _n_sims / _n_threads;
    const size_t _main_thread_task  = _n_sims - _n_threads * _thread_task;

    Game<T> _main_game(scenes, slot);

    if (_n_threads != 1)
    {
        // One Game and result structure per worker thread
        std::vector<Game<T>> _games;
        _games.reserve(_n_threads);
        for (size_t i = 0; i < _n_threads; ++i)
            _games.emplace_back(scenes, slot);

        std::vector<win_dist_t> _datastructs(_n_threads);
        std::vector<std::thread> _threads;
        _threads.reserve(_n_threads);

        // Launch worker threads
        for (size_t i = 0; i < _n_threads; ++i)
        {
            _threads.emplace_back(
                [&game = _games[i], task = _thread_task, &data = _datastructs[i]]()
                {
                    game.play(task, data);
                });
        }

        // Main thread does its share
        _main_game.play(_main_thread_task, _main_dist);

        // Join all threads
        for (auto &t : _threads)
            if (t.joinable())
                t.join();

        // Merge all results into main distribution
        for (const auto &wd : _datastructs)
            for (const auto &[key, value] : wd)
                _main_dist[key] += value;
    }
    else
    {
        _main_game.play(_n_sims, _main_dist);
    }
}
