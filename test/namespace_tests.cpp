#include <catch2/catch_all.hpp>
#include <slot/header.h>
#include <algorithm>
#include <ranges>

using slot_reel_t = std::vector<sym_t>;
using slot_t = std::vector<std::vector<sym_t>>;

// replace_all(symbol)
TEST_CASE("replace_all")
{

    SECTION("functionality")
    {
        // std::vector<reel_t> slot(3, reel_t(3, MAX_UNSIGNED));

        std::array<std::array<unsigned, 3>, 3> slot;
        std::array<unsigned, 3> reel;
        reel.fill(MAX_UNSIGNED);
        slot.fill(reel);

        sym_t symbol{2};
        ss::replace_all(slot, symbol);
        bool condition = std::ranges::all_of(slot, [&symbol](auto reel)
                                             { return std::ranges::all_of(reel, [&symbol](sym_t sym)
                                                                          { return (sym == symbol) ? true : false; }); });
        REQUIRE(condition);
    }
}

// replace(from, to, symbol)
TEST_CASE("replace(from, to, symbol)")
{
    SECTION("functionality")
    {
        slot_t slot(3, slot_reel_t(3, MAX_UNSIGNED));

        point_t from(0, 0);
        point_t to(1, 1);
        sym_t symbol = 3;

        ss::replace(slot, from, to, symbol);
        REQUIRE(slot.at(0).at(0) == symbol);
        REQUIRE(slot.at(0).at(1) == symbol);
        REQUIRE(slot.at(1).at(0) == symbol);
        REQUIRE(slot.at(1).at(1) == symbol);
    }
}

// replace(index, axis, symbol)
TEST_CASE("replace(index, axis, symbol)")
{

    SECTION("Functionality columns")
    {
        slot_t slot(3, slot_reel_t(3, MAX_UNSIGNED));
        unsigned index{0};

        ss::replace(slot, index, 0, sym_t{3});

        for (const auto &sym : slot.at(index))
            REQUIRE(sym == sym_t{3});
    }

    SECTION("Functionality rows")
    {
        slot_t slot(3, slot_reel_t(3, MAX_UNSIGNED));
        unsigned index{1};

        ss::replace(slot, index, 1, sym_t{3});

        for (size_t i{0}; i < 3; ++i)
            REQUIRE(slot[i][1] == sym_t{3});
    }
}

// replace(locations, symbol)
TEST_CASE("replace(locations, symbol)")
{
    SECTION("Functionality")
    {
        slot_t slot(2, slot_reel_t(2, MAX_UNSIGNED));

        std::vector<point_t> locs = {
            point_t(0, 1),
            point_t(1, 0),
        };

        ss::replace(slot, locs, sym_t{3});

        REQUIRE(slot[0][0] != sym_t{3});
        REQUIRE(slot[0][1] == sym_t{3});
        REQUIRE(slot[1][0] == sym_t{3});
        REQUIRE(slot[1][1] != sym_t{3});
    }
}



// count(symbol)
TEST_CASE("count")
{

    SECTION("functionality")
    {
        slot_t slot(3, slot_reel_t(3, MAX_UNSIGNED));
        sym_t symbol{1};

        std::vector<point_t> locs = {
            {0, 0},
            {2, 1},
        };

        for (const point_t &loc : locs)
            slot.at(loc.first).at(loc.second) = symbol;

        occ_t out = ss::count(slot, symbol);
        REQUIRE(out == locs.size());
    }
}

// find(symbol)
TEST_CASE("find")
{

    SECTION("functionality")
    {
        slot_t slot(3, slot_reel_t(3, MAX_UNSIGNED));
        sym_t symbol{1};

        std::vector<point_t> locs = {
            {2, 0},
            {2, 1},
        };
        for (const point_t &loc : locs)
            slot.at(loc.first).at(loc.second) = symbol;

        auto out = ss::find(slot, symbol);

        for (const auto &[i, reel] : std::views::enumerate(slot))
        {
            for (const auto &[j, sym] : std::views::enumerate(reel))
            {
                if (std::ranges::find(out, std::make_pair(i, j)) != out.end())
                    REQUIRE(slot.at(i).at(j) == symbol);
                else
                    REQUIRE(slot.at(i).at(j) != symbol);
            }
        }
    }
}
