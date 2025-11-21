#include <slot/header.h>
#include <iostream>
#include "config.h"

using slot_t = std::array<std::array<sym_t, 3>, 3>;
constexpr sym_t wild = 10;
constexpr double bet = 5.0;
slot_t slot;

scenario_t<slot_t> base_scene = [](slot_t &slot, ChaCha &rng) -> std::pair<state_t, win_t>
{
    win_t win{0};
    ss::spin::weighted_spin(slot, spin_w, rng, index_dict);
    win = ss::win::basic_line_evaluation(slot, paylines, paytable, wild);
    return std::make_pair(state_t{0}, win);
};