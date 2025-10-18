#pragma once

#include <utility>
#include <vector>
#include <functional>
#include <unordered_map>
#include <map>
#include <future>
#include <algorithm>

#include <concepts>

typedef unsigned occ_t;
typedef double metric_t;

template <typename T>
concept Hashable = requires(T a) {
    { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
};

template <Hashable T>
using win_dist_t = std::unordered_map<T, occ_t>;

template <Hashable T>
using generator_t = std::function<T()>;

template <Hashable T>
using strategy_t = std::function<std::vector<metric_t>(const win_dist_t<T> &)>;

template <Hashable T>
class CrashGame
{
private:
    const size_t _n_sims;
    const size_t _n_threads;
    const generator_t<T> _get_multi;
    win_dist_t<T> _main_dist;

    void _play(const size_t, win_dist_t<T> &) noexcept;

public:
    CrashGame(size_t n_threads,
              size_t n_sims,
              const generator_t<T> &get_multi)
        : _n_threads{n_threads},
          _n_sims{n_sims},
          _get_multi{get_multi} {};

    void simulate() noexcept;
    [[nodiscard]] const win_dist_t<T> &get_wd() noexcept { return _main_dist; };
};

/**
 *  CrashGame definitions
 */

template <Hashable T>
void CrashGame<T>::_play(const size_t sims, win_dist_t<T> &dist) noexcept
{
    for (size_t i{0}; i < sims; ++i)
        ++dist[_get_multi()];
}

template <Hashable T>
void CrashGame<T>::simulate() noexcept
{
    size_t thread_sims = _n_sims / _n_threads;
    size_t main_sims = _n_sims - _n_threads * thread_sims;

    std::vector<win_dist_t<T>> _ds(_n_threads, win_dist_t<T>());
    _ds.shrink_to_fit();
    std::vector<std::future<void>> _threads;

    for (size_t i{0}; i < _n_threads; i++)
        _threads.emplace_back(std::async(&CrashGame::_play, this, thread_sims, std::ref(_ds[i])));
    _play(main_sims, _main_dist);

    for (auto &t : _threads)
        t.wait();

    for (auto &data : _ds)
        for (const auto &[key, value] : data)
            _main_dist[key] += value;
}
