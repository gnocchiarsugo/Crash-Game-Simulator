#include <future>
#include <iostream>
#include <array>
#include "crashgame/include/crash_game.h"

CrashGame::CrashGame(size_t &&n_threads, size_t &&n_sims, const std::function<const unsigned()> &func)
    : _n_threads(std::move(n_threads)),
      _n_sims(std::move(n_sims)),
      _getMulti(func)
{
  _execute();
};

CrashGame::CrashGame(const size_t &n_threads, const size_t &n_sims, const std::function<const unsigned()> &func)
    : _n_threads(n_threads),
      _n_sims(n_sims),
      _getMulti(func)
{
  _execute();
};

void CrashGame::_play(size_t &n_sims, DataCollector &_dc)
{
  for (size_t i = 0; i < n_sims; i++)
    _dc.addWin(_getMulti());
}

void CrashGame::_execute()
{
  // Subdivide the tasks
  size_t _thread_task = _n_sims / _n_threads;
  size_t _main_thread_task = _n_sims - _n_threads * _thread_task;

  std::vector<DataCollector> _datastructs(_n_threads, DataCollector());
  std::vector<std::future<void>> _threads;
  _threads.reserve(_n_threads);

  // Generate dc
  for (size_t i{0}; i < _n_threads; i++)
    _threads.emplace_back(std::async(&CrashGame::_play, this, std::ref(_thread_task), std::ref(_datastructs[i])));

  _play(_main_thread_task, _main_dc);

  // Wait for threads
  for (auto &t : _threads)
    t.get();

  // Merge data
  for (auto &data : _datastructs)
    _main_dc += std::move(data);
}

void CrashGame::evaluateWins()
{
  if (win_rules.empty())
    return;
  for (auto &win_rule : win_rules)
    win_rule.computeRTP(_main_dc.getWinDist(), _n_sims);
}