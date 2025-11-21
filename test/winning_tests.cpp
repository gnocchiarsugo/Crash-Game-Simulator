#include <catch2/catch_all.hpp>
#include <slot/header.h>
#include <vector>
#include <string>

using slot_reel_t = std::vector<sym_t>;
using slot_t = std::vector<std::vector<sym_t>>;

// basic way evaluation
TEST_CASE("way_evaluation")
{

    SECTION("If slot is all same symbol the ways are the product off all the reel sizes")
    {

        using paytable_t = std::array<std::array<win_t, 4>, 1>;
        paytable_t paytable = {{{0, 0, 0, 1}}};
        slot_t slot(3, slot_reel_t(3, 0));

        /**
         *  Slot:
         *
         *  0   0   0
         *  0   0   0
         *  0   0   0
         */

        auto win = ss::win::basic_way_evaluation<slot_t, paytable_t>(slot, paytable, sym_t{MAX_UNSIGNED});
        REQUIRE(win == 27);
    }

    SECTION("If slot is all same symbol, and all wilds in the first reel. The ways are the product off all the reel sizes")
    {

        using paytable_t = std::array<std::array<win_t, 4>, 2>;
        paytable_t paytable = {{
            {0, 0, 0, 1},
            {0, 0, 0, 0},
        }};
        slot_t slot(3, slot_reel_t(3, 0));
        sym_t wild{1};
        slot[0][0] = 1;
        slot[0][1] = 1;
        slot[0][2] = 1;

        /**
         *  Slot:
         *
         *  W   0   0
         *  W   0   0
         *  W   0   0
         */

        auto win = ss::win::basic_way_evaluation<slot_t, paytable_t>(slot, paytable, wild);
        REQUIRE(win == 27);
    }

    SECTION("A wild in a reel different from the first can substitute all symbols")
    {
        using paytable_t = std::array<std::array<win_t, 4>, 3>;
        paytable_t paytable = {{
            {0, 0, 0, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
        }};
        slot_t slot(3, slot_reel_t(3, 1));
        sym_t wild{2};
        slot[0][0] = 0;
        slot[1][0] = wild;
        slot[2][0] = 0;

        /**
         *  Slot:
         *
         *  0   W   0
         *  1   1   1
         *  1   1   1
         */

        auto win = ss::win::basic_way_evaluation(slot, paytable, wild);
        REQUIRE(win == 1);
    }

    SECTION("If there are multiple wild in first reel the wins are multiplied")
    {
        using paytable_t = std::array<std::array<win_t, 4>, 3>;
        paytable_t paytable = {{
            {0, 0, 0, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
        }};
        slot_t slot(3, slot_reel_t(3, 1));
        sym_t wild{2};
        slot[0][0] = 0;
        slot[1][0] = wild;
        slot[1][1] = wild;
        slot[2][0] = 0;

        /**
         *  Slot:
         *
         *  0   W   0
         *  1   W   1
         *  1   1   1
         */

        auto win = ss::win::basic_way_evaluation(slot, paytable, wild);
        REQUIRE(win == 2);
    }

    SECTION("A wild in first reel can substitute all symbols")
    {
        using paytable_t = std::array<std::array<win_t, 4>, 3>;
        paytable_t paytable = {{
            {0, 0, 0, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
        }};
        slot_t slot(3, slot_reel_t(3, 1));
        sym_t wild{2};
        slot[0][0] = wild;
        slot[1][0] = 0;
        slot[2][0] = 0;

        /**
         *  Slot:
         *
         *  W   0   0
         *  1   1   1
         *  1   1   1
         */

        auto win = ss::win::basic_way_evaluation(slot, paytable, wild);
        REQUIRE(win == 1);
    }

    SECTION("If there are multiple wild in first reel the wins are multiplied")
    {
        using paytable_t = std::array<std::array<win_t, 4>, 3>;
        paytable_t paytable = {{
            {0, 0, 0, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
        }};
        slot_t slot(3, slot_reel_t(3, 1));
        sym_t wild{2};
        slot[0][0] = wild;
        slot[0][1] = wild;
        slot[1][0] = 0;
        slot[2][0] = 0;

        /**
         *  Slot:
         *
         *  W   0   0
         *  W   1   1
         *  1   1   1
         */

        auto win = ss::win::basic_way_evaluation(slot, paytable, wild);
        REQUIRE(win == 2);
    }

    SECTION("If the wild is in the first reel together with a non winning symbol, the ways is functional")
    {
        using paytable_t = std::array<std::array<win_t, 4>, 3>;
        paytable_t paytable = {{
            {0, 0, 0, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
        }};
        slot_t slot(3, slot_reel_t(3, 0));
        sym_t wild{2};
        slot[0][0] = 1;
        slot[0][1] = wild;
        // slot[1][0] = 0;
        // slot[2][0] = 0;

        /**
         *  Slot:
         *
         *  1   0   0
         *  W   0   0
         *  0   0   0
         */

        auto win = ss::win::basic_way_evaluation(slot, paytable, wild);

        REQUIRE(win == 18);
    }

    SECTION("Chat found test")
    {
        using paytable_t = std::array<std::array<win_t, 5>, 3>;
        paytable_t paytable = {{
            {0, 0, 0, 10, 20},
            {0, 0, 0, 15, 30},
            {0, 0, 0, 0, 0},
        }};
        slot_t slot(4, slot_reel_t(1, 0));
        sym_t wild{2};
        slot[0][0] = wild;
        slot[1][0] = wild;
        slot[2][0] = 0;
        slot[3][0] = 1;

        /**
         *  Slot:
         *
         *  W   W   0   1
         */

        auto win = ss::win::basic_way_evaluation(slot, paytable, wild);

        REQUIRE(win == 10);
    }

    SECTION("Chat found test 2")
    {
        using paytable_t = std::array<std::array<win_t, 5>, 3>;
        paytable_t paytable = {{
            {0, 0, 0, 10, 20},
            {0, 0, 0, 15, 30},
            {0, 0, 0, 0, 0},
        }};
        slot_t slot(4, slot_reel_t(1, 0));
        sym_t wild{2};
        slot[0][0] = 0;
        slot[1][0] = wild;
        slot[2][0] = wild;
        slot[3][0] = 1;

        /**
         *  Slot:
         *
         *  0   W   W   1
         */

        auto win = ss::win::basic_way_evaluation(slot, paytable, wild);

        REQUIRE(win == 10);
    }

    SECTION("Chat found test 3")
    {
        using paytable_t = std::array<std::array<win_t, 5>, 3>;
        paytable_t paytable = {{
            {0, 0, 0, 10, 20},
            {0, 0, 0, 15, 30},
            {0, 0, 0, 0, 0},
        }};
        slot_t slot(4, slot_reel_t(1, 0));
        sym_t wild{2};
        slot[0][0] = 0;
        slot[1][0] = 1;
        slot[2][0] = wild;
        slot[3][0] = 0;

        /**
         *  Slot:
         *
         *  0   1   W   0
         */

        auto win = ss::win::basic_way_evaluation(slot, paytable, wild);

        REQUIRE(win == 0);
    }

    SECTION("Chat found test 4")
    {
        using paytable_t = std::array<std::array<win_t, 5>, 4>;
        paytable_t paytable = {{
            {0, 0, 0, 10, 20},
            {0, 0, 0, 15, 30},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
        }};
        slot_t slot(4, slot_reel_t(1, 2));
        slot[2].push_back(2);
        sym_t wild{3};
        slot[0][0] = wild;
        slot[1][0] = wild;
        slot[2][0] = 0;
        slot[2][1] = 1;
        slot[3][0] = 1;

        /**
         *  Slot:
         *
         *  W   W   0   1
         *  X   X   1   X
         */

        auto win = ss::win::basic_way_evaluation(slot, paytable, wild);
        REQUIRE(win == 30 + 10);
    }

}

// basic line evaluation
TEST_CASE("basic line eval")
{

    SECTION("Given a slot of one row, a payline, and paytable: if the slot has [0,0,0] winning, the win is defined by the paytable")
    {
        slot_t slot = {{0}, {0}, {0}};

        const std::array<std::array<unsigned, 3>, 1> paylines = {{
            {0, 0, 0},
        }};

        const std::array<std::array<win_t, 4>, 3> paytable = {{
            {0, 0, 0, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
        }};

        auto win = ss::win::basic_line_evaluation(slot, paylines, paytable, sym_t{2});
        REQUIRE(win == 1);
    }

    SECTION("Given a slot of one row, a payline, and paytable: if the slot has winning [wild, symbol, symbol] this counts as 3 x symbol")
    {
        slot_t slot = {{2}, {0}, {0}};

        const std::array<std::array<unsigned, 3>, 1> paylines = {{
            {0, 0, 0},
        }};

        const std::array<std::array<win_t, 4>, 3> paytable = {{
            {0, 0, 0, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
        }};

        auto win = ss::win::basic_line_evaluation(slot, paylines, paytable, sym_t{2});
        REQUIRE(win == 1);
    }

    SECTION("Given a slot of one row, a payline, and paytable: if the slot has winning [symbol, wild, symbol] this counts as 3 x symbol")
    {
        slot_t slot = {{0}, {2}, {0}};

        const std::array<std::array<unsigned, 3>, 1> paylines = {{
            {0, 0, 0},
        }};

        const std::array<std::array<win_t, 4>, 3> paytable = {{
            {0, 0, 0, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
        }};

        auto win = ss::win::basic_line_evaluation(slot, paylines, paytable, sym_t{2});
        REQUIRE(win == 1);
    }

    SECTION("Given a slot of one row, a payline, and paytable: if the slot has winning [symbol, symbol, wild] this counts as 3 x symbol")
    {
        slot_t slot = {{0}, {0}, {2}};

        const std::array<std::array<unsigned, 3>, 1> paylines = {{
            {0, 0, 0},
        }};

        const std::array<std::array<win_t, 4>, 3> paytable = {{
            {0, 0, 0, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
        }};

        auto win = ss::win::basic_line_evaluation(slot, paylines, paytable, sym_t{2});
        REQUIRE(win == 1);
    }

    SECTION("Given a slot of one row, a payline, and paytable: if the slot has winning [wild, wild, symbol] this counts as 3 x symbol")
    {
        slot_t slot = {{2}, {2}, {0}};

        const std::array<std::array<unsigned, 3>, 1> paylines = {{
            {0, 0, 0},
        }};

        const std::array<std::array<win_t, 4>, 3> paytable = {{
            {0, 0, 0, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
        }};

        auto win = ss::win::basic_line_evaluation(slot, paylines, paytable, sym_t{2});
        REQUIRE(win == 1);
    }

    SECTION("Given a slot of one row, a payline, and paytable: if the slot has winning [symbol, wild, wild] this counts as 3 x symbol")
    {
        slot_t slot = {{0}, {2}, {2}};

        const std::array<std::array<unsigned, 3>, 1> paylines = {{
            {0, 0, 0},
        }};

        const std::array<std::array<win_t, 4>, 3> paytable = {{
            {0, 0, 0, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
        }};

        auto win = ss::win::basic_line_evaluation(slot, paylines, paytable, sym_t{2});
        REQUIRE(win == 1);
    }

    SECTION("Given a slot of one row, a payline, and paytable: if the slot has winning [wild, wild, wild] this counts as 3 x wild")
    {
        slot_t slot = {{2}, {2}, {2}};

        const std::array<std::array<unsigned, 3>, 1> paylines = {{
            {0, 0, 0},
        }};

        const std::array<std::array<win_t, 4>, 3> paytable = {{
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 5},
        }};

        auto win = ss::win::basic_line_evaluation(slot, paylines, paytable, sym_t{2});
        REQUIRE(win == 5);
    }

    SECTION("Given a slot of one row, a payline, and paytable: if the slot has winning [wild, symbol, non-paying symbol] this counts as 2 x symbol")
    {
        slot_t slot = {{2}, {0}, {1}};

        const std::array<std::array<unsigned, 3>, 1> paylines = {{
            {0, 0, 0},
        }};

        const std::array<std::array<win_t, 4>, 3> paytable = {{
            {0, 0, 2, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
        }};

        auto win = ss::win::basic_line_evaluation(slot, paylines, paytable, sym_t{2});
        REQUIRE(win == 2);
    }

    SECTION("Given a slot of one row, a payline, and paytable: if the slot has winning [wild, non-paying symbol, symbol] this counts as 1 x wild")
    {
        slot_t slot = {{2}, {1}, {0}};

        const std::array<std::array<unsigned, 3>, 1> paylines = {{
            {0, 0, 0},
        }};

        const std::array<std::array<win_t, 4>, 3> paytable = {{
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 3, 0, 0},
        }};

        auto win = ss::win::basic_line_evaluation(slot, paylines, paytable, sym_t{2});
        REQUIRE(win == 3);
    }

    SECTION("Given a slot of one row, a payline, and paytable: if the slot has winning [wild, wild, symbol] and 2 x wild pay more than 3 x symbol this counts as 2 x wild")
    {
        slot_t slot = {{2}, {2}, {0}};

        const std::array<std::array<unsigned, 3>, 1> paylines = {{
            {0, 0, 0},
        }};

        const std::array<std::array<win_t, 4>, 3> paytable = {{
            {0, 0, 0, 1},
            {0, 0, 0, 0},
            {0, 0, 2, 0},
        }};

        auto win = ss::win::basic_line_evaluation(slot, paylines, paytable, sym_t{2});
        REQUIRE(win == 2);
    }

    SECTION("If multiple payline win the win is summed")
    {
        slot_t slot = {{0}, {0}, {0}};

        const std::array<std::array<unsigned, 3>, 3> paylines = {{
            {0, 0, 0},
            {0, 0, 0},
            {0, 0, 0},
        }};

        const std::array<std::array<win_t, 4>, 3> paytable = {{
            {0, 0, 0, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
        }};

        auto win = ss::win::basic_line_evaluation(slot, paylines, paytable, sym_t{2});
        REQUIRE(win == 3);
    }
}
