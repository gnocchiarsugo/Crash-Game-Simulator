#pragma once

#include <concepts>
#include <string>
#include <vector>
#include <utility>
#include <map>
#include <functional>


#define MAX_UNSIGNED static_cast<unsigned>(-1)
using sym_t = unsigned;

using state_t = unsigned;
using occ_t = size_t;
using win_t = size_t;
using win_dist_t = std::map<win_t, occ_t>;

using stop_t = std::vector<int>;
using point_t = std::pair<unsigned, unsigned>;

using dictionary_t = std::map<std::string, sym_t>;

template <typename T>
struct is_std_array_s : std::false_type
{
};

template <typename T, std::size_t N>
struct is_std_array_s<std::array<T, N>> : std::true_type
{
};

template <typename T>
inline constexpr bool is_std_array = is_std_array_s<T>::value;

template <typename T>
struct is_std_vector_s : std::false_type
{
};

template <typename T, typename Alloc>
struct is_std_vector_s<std::vector<T, Alloc>> : std::true_type
{
};

template <typename T>
inline constexpr bool is_std_vector = is_std_vector_s<T>::value;

template <typename T, typename RequiredI>
concept ConstrainedArray =
    requires { typename T::value_type; } && std::same_as<typename T::value_type, RequiredI> && is_std_array<T>;

/**
 *  Defines either a vector or array with the required data inside
 */ 
template <typename T, typename RequiredI>
concept ConstrainedContainer =
    requires { typename T::value_type; } && std::same_as<typename T::value_type, RequiredI> && (is_std_vector<T> || is_std_array<T>);

template<typename T>
concept Mask = ConstrainedContainer<T, bool>;
    

