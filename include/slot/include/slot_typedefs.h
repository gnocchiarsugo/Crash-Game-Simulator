#pragma once

#include "typedefs.h"
#include "chacha.h"

// Slot
template <typename T>
concept SymbolReel = ConstrainedContainer<T, sym_t>;

//TODO: change to fit certain structs with composite slots
template <typename T>
concept Slot =
    std::ranges::range<T> &&
    SymbolReel<std::ranges::range_value_t<T>>;

template <typename T>
inline constexpr bool is_slot = Slot<T>;

template <typename T>
concept Reels =
    std::ranges::range<T> &&
    SymbolReel<std::ranges::range_value_t<T>>;

template <typename T>
concept Payment = ConstrainedArray<T, win_t>;
    // requires { typename T::value_type; } && std::same_as<typename T::value_type, win_t> && is_std_array<T>;

template <typename T>
concept Paytable =
    std::ranges::range<T> &&
    Payment<std::ranges::range_value_t<T>>;

template <typename T>
concept Payline = ConstrainedArray<T, unsigned>;
    // requires { typename T::value_type; } && std::same_as<typename T::value_type, unsigned> && is_std_array<T>;

template <typename T>
concept Paylines =
    std::ranges::range<T> &&
    Payline<std::ranges::range_value_t<T>>;


/**
 *  Slot functionality
 */

/**
 * @brief function that takes the slot -> modifyies it -> returns the new state to switch to
 */
template <class T>
using scenario_t = std::function<std::pair<state_t, win_t>(T &, ChaCha&)>;
/**
 * @brief maps stats to modes of spinning
 */
template <class T>
using scenarios_t = std::map<state_t, scenario_t<T>>;
/**
 * @brief Checks if the condition coded inside is true/false
 */
// template <Slot T>
// using trigger_t = std::function<bool(const T &)>;
/**
 * @brief Maps of state to set of triggers, since there can be multiple for a single state.
 */
// template <Slot T>
// using triggers_t = std::map<state_t, std::vector<trigger_t<T>>>;
/**
 * @brief Function returning the metric given the slot
 */
// template <Slot T>
// using evaluator_t = std::function<double(const T &)>;
/**
 * @brief Maps state to evaluation methods
 */
// template <Slot T>
// using evaluators_t = std::map<state_t, evaluator_t<T>>;


