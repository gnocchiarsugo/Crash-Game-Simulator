#include <catch2/catch_all.hpp>
#include <slot/header.h>
#include "../src/config.h"

using slot_t = std::array<std::array<sym_t, 3>, 3>;
constexpr sym_t no_win = 9;

// Used for game specific tests
// TEST_CASE("cheat slot win")
// {

//     SECTION("win 1")
//     {
//         std::vector<std::vector<std::string>> cheat_slot= {
//             {"GRAPES", "SEVEN","SEVEN","CHERRIES"},
//             {"SEVEN","SEVEN","GRAPES","GRAPES"},
//             {"SEVEN","LEMON","LEMON","ORANGE"},
//             {"BELL","ORANGE","ORANGE","BELL"},
//             {"ORANGE","CHERRIES","CHERRIES","WILD"},
//         };
//         slot_t slot;
//         ss::cheat_slot(slot, cheat_slot, dict);

//         auto win = ss::win::basic_line_evaluation(slot, paylines, paytable, sym_t{8});
//         REQUIRE(win == 50);

//     }

//     SECTION("win 1")
//     {
//         std::vector<std::vector<std::string>> cheat_slot= {
//             {"CHERRIES", "GRAPES","GRAPES","SEVEN"},
//             {"GRAPES","GRAPES","FREE_SPINS","WILD"},
//             {"CHERRIES","CHERRIES","GRAPES","GRAPES"},
//             {"LEMON","PLUM","PLUM","FREE_SPINS"},
//             {"LEMON","LEMON","SEVEN","SEVEN"},
//         };
//         slot_t slot;
//         ss::cheat_slot(slot, cheat_slot, dict);

//         auto win = ss::win::basic_line_evaluation(slot, paylines, paytable, sym_t{8});
//         REQUIRE(win == 40);

//     }

// }

TEST_CASE("cheat slot")
{
    SECTION("pierino win")
    {
        slot_t slot = {{
            {1, 1, 1},
            {2, 3, 4},
            {5, 6, 7},
        }};

        win_t win{0};
        for (sym_t sym : index_dict) // sym = 1, ..., 9
            win += paytable[sym - 1][std::min(ss::count(slot, sym), occ_t{3})];
        win = ss::count(slot, no_win) ? 0 : win;
        REQUIRE(win == 1000);
    }


}