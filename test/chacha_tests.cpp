#include <catch2/catch_all.hpp>
#include <slot/header.h>
#include <array>

TEST_CASE("index_from_weight")
{

    SECTION("If there are two weights, and one is 0, the return is always the other index")
    {
        size_t N = 100;
        std::array<ChaCha::result_type, 2> w = {0, 1};
        size_t acc = 0;

        // RNG init
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

        for (size_t i{0}; i < N; ++i)
            acc += rng.index_from_weights(w);
        REQUIRE(acc == N);
    }

    SECTION("If the vector of weights has size l the random index is between 0 and l-1")
    {
        size_t N = 100;
        constexpr size_t l = 10;
        std::array<ChaCha::result_type, l> w;
        std::array<size_t, l + 1> occ;
        occ.fill(0);

        w.fill(1);

        // RNG init
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

        for (size_t i{0}; i < N; ++i)
            ++occ[rng.index_from_weights(w)];

        for (size_t i{0}; i < l; ++i)
            REQUIRE(occ[i] >= 0);

        REQUIRE(occ[l] == 0);
    }

    SECTION("If there are two equal weights, the indices have almost equal probability")
    {
        size_t N = 10000000;
        std::array<ChaCha::result_type, 2> w = {1, 1};
        size_t acc = 0;

        // RNG init
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

        for (size_t i{0}; i<N;++i)
            acc += rng.index_from_weights(w);
        REQUIRE_THAT(acc/static_cast<double>(N), Catch::Matchers::WithinRel(0.5, 1e-3));
    }
}