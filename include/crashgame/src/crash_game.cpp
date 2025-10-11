#include <future>
#include "../include/crash_game.h"

void CrashGame::_execute(size_t n_sims, WinDist &wd) noexcept
{
  for (size_t i = 0; i < n_sims; i++)
    wd.add_win(_get_multi());
}

void CrashGame::play() noexcept
{
  // Subdivide the tasks
  size_t _thread_task = _n_sims / _n_threads;
  size_t _main_thread_task = _n_sims - _n_threads * _thread_task;

  std::vector<WinDist> _datastructs(_n_threads, WinDist());
  std::vector<std::future<void>> _threads;
  _threads.reserve(_n_threads);

  // Generate dc
  for (size_t i{0}; i < _n_threads; i++)
    _threads.emplace_back(std::async(&CrashGame::_execute, this, std::ref(_thread_task), std::ref(_datastructs[i])));

  _execute(_main_thread_task, _main_wd);

  // Wait for threads
  for (auto &t : _threads)
    t.get();

  // Merge data
  for (auto &data : _datastructs)
    _main_wd += std::move(data);
  _main_wd.sort();
}

[[nodiscard]] std::vector<std::pair<rtp_t, rtp_std_t>> CrashGame::evaluate(const std::vector<strategy_t> &strats) noexcept
{
  std::vector<std::pair<rtp_t, rtp_std_t>> out;
  for (const strategy_t &strat : strats)
    out.push_back(strat(_main_wd));
  std::reverse(out.begin(), out.end());
  return out;
};