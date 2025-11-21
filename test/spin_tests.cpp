#include <catch2/catch_all.hpp>
#include <slot/header.h>

using slot_reel_t = std::vector<sym_t>;
using slot_t = std::vector<std::vector<sym_t>>;

/** basic_spin(reels, stops)
 *
 */
TEST_CASE("basic_spin")
{

    SECTION("If the reels have the same symbol, the slot will have the same symbol with any stop position")
    {
        using reels_t = std::array<std::array<sym_t, 7>, 3>;
        reels_t reels = {{
            {1, 1, 1, 1, 1, 1, 1},
            {1, 1, 1, 1, 1, 1, 1},
            {1, 1, 1, 1, 1, 1, 1},
        }};
        stop_t stop{1, 2, 1};
        slot_t slot(3, slot_reel_t(3, MAX_UNSIGNED));

        ss::spin::basic_spin<slot_t, reels_t>(slot, reels, stop);

        for (const auto &reel : slot)
            for (const auto &sym : reel)
                REQUIRE(sym == sym_t{1});
    }

    SECTION("If the reels have the same symbol and same dimension as the slot, the slot will have the same symbol")
    {
        using reels_t = std::array<std::array<sym_t, 3>, 3>;
        reels_t reels = {{
            {1, 1, 1},
            {1, 1, 1},
            {1, 1, 1},
        }};
        stop_t stop{1, 2, 1};
        slot_t slot(3, slot_reel_t(3, MAX_UNSIGNED));

        ss::spin::basic_spin(slot, reels, stop);

        for (const auto &reel : slot)
            for (const auto &sym : reel)
                REQUIRE(sym == sym_t{1});
    }

    SECTION("If stop position is at a particular symbol, that symbol will be at the bottom of _slot")
    {
        using reels_t = std::array<std::array<sym_t, 3>, 3>;
        reels_t reels = {{
            {0, 0, 1},
            {1, 0, 0},
            {0, 1, 0},
        }};
        stop_t stop{2, 0, 1};
        slot_t slot(3, slot_reel_t(3, MAX_UNSIGNED));
        ss::spin::basic_spin<slot_t, reels_t>(slot, reels, stop);

        REQUIRE(slot.at(0).at(2) == sym_t{1});
        REQUIRE(slot.at(1).at(2) == sym_t{1});
        REQUIRE(slot.at(2).at(2) == sym_t{1});
    }

    SECTION("If a stop position is at the start, then the symbols will circle back from the bottom")
    {
        using reels_t = std::array<std::array<sym_t, 3>, 3>;
        reels_t reels = {{
            {0, 0, 1},
            {1, 0, 0},
            {0, 1, 0},
        }};
        stop_t stop{1, 2, 0};

        slot_t slot(3, slot_reel_t(3, MAX_UNSIGNED));
        ss::spin::basic_spin(slot, reels, stop);

        REQUIRE(slot.at(0).at(0) == sym_t{1});
        REQUIRE(slot.at(1).at(0) == sym_t{1});
        REQUIRE(slot.at(2).at(0) == sym_t{1});
    }

    SECTION("After spin, the slot is populated starting from stop indices and going upwards.")
    {
        using reels_t = std::array<std::array<sym_t, 9>, 2>;
        reels_t reels = {{
            {1, 2, 3, 4, 5, 6, 7, 8, 9},
            {1, 2, 3, 4, 5, 6, 7, 8, 9},
        }};
        stop_t stop{5, 1};

        slot_t slot(2, slot_reel_t(2, MAX_UNSIGNED));
        ss::spin::basic_spin(slot, reels, stop);

        REQUIRE(slot.at(0).at(0) == sym_t{5});
        REQUIRE(slot.at(0).at(1) == sym_t{6});
        REQUIRE(slot.at(1).at(0) == sym_t{1});
        REQUIRE(slot.at(1).at(1) == sym_t{2});
    }
}

/// TODO: add weighted spin tests
TEST_CASE("weighted_spin")
{
    // SECTION("")
}