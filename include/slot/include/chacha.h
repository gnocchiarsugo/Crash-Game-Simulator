#pragma once

#include <array>
#include <cstdint>
#include <ranges>
#include <numeric>
#include "typedefs.h"

template <typename T>
concept Weights = ConstrainedContainer<T, uint32_t>;

class ChaCha
{
public:
    using result_type = uint32_t;

private:
    std::array<uint32_t, 16> state_;

    static uint32_t _rotr32(uint32_t x, int n) noexcept { return (x >> n) | (x << (32 - n)); }
    static uint32_t _rotl32(uint32_t x, int n) noexcept { return (x << n) | (x >> (32 - n)); }
    static void quarter_round(uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d) noexcept
    {
        a += b;
        d ^= a;
        d = _rotr32(d, 16);
        c += d;
        b ^= c;
        b = _rotr32(b, 12);
        a += b;
        d ^= a;
        d = _rotr32(d, 8);
        c += d;
        b ^= c;
        b = _rotr32(b, 7);
    }

    std::array<uint32_t, 16> generate_block() noexcept
    {
        auto state = state_;
        for (int i = 0; i < 10; ++i)
        {
            quarter_round(state[0], state[4], state[8], state[12]);
            quarter_round(state[1], state[5], state[9], state[13]);
            quarter_round(state[2], state[6], state[10], state[14]);
            quarter_round(state[3], state[7], state[11], state[15]);
            quarter_round(state[0], state[5], state[10], state[15]);
            quarter_round(state[1], state[6], state[11], state[12]);
            quarter_round(state[2], state[7], state[8], state[13]);
            quarter_round(state[3], state[4], state[9], state[14]);
        }
        std::array<uint32_t, 16> output;
        for (int i = 0; i < 16; ++i)
            output[i] = state[i] + state_[i];
        return output;
    }

    std::array<uint32_t, 16> buffer_;
    size_t buffer_index_ = 16;
    [[nodiscard]] result_type _next() noexcept
    {
        if (buffer_index_ >= 16)
        {
            buffer_ = generate_block();
            if (++state_[12] == 0)
                ++state_[13];

            buffer_index_ = 0;
        }
        return buffer_[buffer_index_++];
    }

public:
    static constexpr bool is_secure = true;
    static constexpr result_type min() noexcept { return 0; }
    static constexpr result_type max() noexcept { return UINT32_MAX; }

    [[nodiscard]] const double uniform() noexcept { return (static_cast<double>(_next()) / (static_cast<double>(ChaCha::max()) + 1.0)); };

    [[nodiscard]] const result_type next() noexcept { return _next(); };

    [[nodiscard]] result_type int_between(result_type a, result_type b) noexcept
    {
        if (a == b)
            return a;
        if (a > b)
            std::swap(a, b);

        // uint64_t range = static_cast<uint64_t>(b) - a + 1;
        // uint64_t r = static_cast<uint64_t>(next());
        // uint64_t scaled = (r * range) >> 32;
        // return static_cast<result_type>(a + scaled);

        uint64_t range = static_cast<uint64_t>(b) - a + 1;        // b included
        uint64_t limit = ChaCha::max() - (ChaCha::max() % range); // avoid bias

        result_type r;
        do
        {
            r = next();
        } while (r >= limit);

        return static_cast<result_type>(a + (r % range));
    }

    template <class T>
    void shuffle(std::vector<T> &data) noexcept
    {
        for (size_t i{0}; i < data.size(); ++i)
            std::swap(data[i], data[int_between(0, data.size() - 1)]);
    };

    template <Weights W>
    [[nodiscard]] size_t index_from_weights(const W &weights) noexcept
    {
        result_type tot = std::accumulate(weights.begin(), weights.end(), result_type{0});
        if (tot == 0)
            return 0;

        result_type extracted = int_between(0, tot-1);
        for (size_t i = 0; i < weights.size(); ++i)
        {
            if (extracted < weights[i])
                return i;
            extracted -= weights[i];
        }
        return weights.size() - 1;
    }

    [[nodiscard]] const double check_prob(const double prob) noexcept { return uniform() < prob ? true : false; }

    ChaCha() = default;
    explicit ChaCha(std::array<result_type, 8> seed, std::array<uint32_t, 3> nonce = {0, 0, 0}) noexcept { reseed(seed, nonce); }

    void reseed(std::array<uint32_t, 8> key, std::array<uint32_t, 3> nonce = {0, 0, 0}) noexcept
    {
        state_[0] = 0x61707865;
        state_[1] = 0x3320646e;
        state_[2] = 0x79622d32;
        state_[3] = 0x6b206574;

        for (int i = 0; i < 8; ++i)
            state_[4 + i] = key[i];

        state_[12] = 0;
        state_[13] = nonce[0];
        state_[14] = nonce[1];
        state_[15] = nonce[2];
    }
};
