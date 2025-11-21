#pragma once
#include "typedefs.h"
#include "slot_typedefs.h"
#include "chacha.h"
#include <array>
#include <iostream>
#include <random>
#include <thread>

template <class T>
class Game
{
    T _slot;
    [[nodiscard]] win_t _round();
    const scenarios_t<T> _scenes;
    ChaCha _rng;

public:
    Game(const scenarios_t<T> &s, const T &slot) : _scenes{s}, _slot{slot}
    {
        std::array<uint32_t, 8> key;
        std::array<uint32_t, 3> nonce;
        thread_local std::random_device rd;
        for (auto &w : key)
            w = (rd() << 16) ^ rd();
        for (auto &w : nonce)
            w = (rd() << 16) ^ rd();
        uint64_t tid = std::hash<std::thread::id>{}(std::this_thread::get_id());
        for (size_t i = 0; i < key.size(); ++i)
            key[i] ^= static_cast<uint32_t>(tid >> (i * 4));
        ChaCha rng(key, nonce);
        _rng = rng;
    };

    void play(const size_t, win_dist_t &);
};

/**
 *  Game state machine impl.
 *
 *  After the round doesn't invalidate the slot
 *  -> the slot has leftovers from previous round
 *
 */
template <class T>
[[nodiscard]] win_t Game<T>::_round()
{
    state_t state{1};
    win_t win{0};

    while (state != 0)
    {
        std::pair<state_t, win_t> result = _scenes.at(state)(_slot, _rng);
        state = result.first;
        win += result.second;
    }
    return win;
}

template <class T>
void Game<T>::play(const size_t n_sims, win_dist_t &dist)
{
    for (size_t i{0}; i < n_sims; ++i)
        ++dist[_round()];
}
