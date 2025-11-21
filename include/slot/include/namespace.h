#pragma once
#include <iostream>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <utility>
#include <string>

namespace ss
{

    /**
     *  @param slot is filled with symbols, it will be populated with the cheat slot
     *  @param cheat_slot is a vector of vectors containing the symbol names in string format.
     *  taken by value since its intended usage is for debugging
     *  @param dict gives a map between symbol names in string format and their number
     *
     */
    template <Slot S>
    void cheat_slot(S &slot, std::vector<std::vector<std::string>> cheat_slot, const dictionary_t &dict)
    {
        assert(cheat_slot.size() == slot.size());

        for (size_t i{0}; i < slot.size(); ++i)
            assert(slot[i].size() == cheat_slot[i].size());

        for (size_t i{0}; i < slot.size(); ++i)
            for (size_t j{0}; j < slot[i].size(); ++j)
                slot[i][j] = dict.at(cheat_slot[i][j]);
    }

    /**
     * @brief Fills the slot with symbol
     * @param slot to modify
     * @param symbol to use
     */
    template <Slot S>
    void replace_all(S &slot, sym_t symbol) noexcept
    {
        for (auto &reel : slot)
            std::ranges::fill(reel, symbol);
    }

    /**
     * @brief Replaces the rectagle defined by the two points with the specified symbol
     * @param slot to modify
     * @param from point specified by (reel, row of said reel)
     * @param to point specified by (reel, row of said reel)
     * @param symbol to use
     */
    template <Slot S>
    void replace(S &slot, point_t &from, point_t &to, sym_t symbol) noexcept
    {
        point_t begin = std::make_pair(std::min(from.first, to.first), std::min(from.second, to.second));
        point_t end = std::make_pair(std::max(from.first, to.first), std::max(from.second, to.second));

        for (const auto &[i, reel] : std::views::enumerate(slot))
            for (const auto &[j, sym] : std::views::enumerate(reel))
                if (begin.first <= i && i <= end.first &&
                    begin.second <= j && j <= end.second)
                    sym = symbol;
    }

    /**
     * @brief Replaces the index either the column or row defined by the axis with the specified symbol
     * @param slot to modify
     * @param index column or row index
     * @param axis whether it's a row or column
     * @param symbol to use substitute
     */
    template <Slot S>
    void replace(S &slot, unsigned index, unsigned axis, sym_t symbol) noexcept
    {
        switch (axis)
        {
        case 0:
            // Columns
            std::ranges::fill(slot.at(index), symbol);
            break;
        case 1:
            // Rows
            for (const auto &[i, reel] : std::views::enumerate(slot))
                slot.at(i).at(index) = symbol;
        }
    }

    /**
     * @brief Replaces the symbols in locations with the specified symbol
     * @param slot to modify
     * @param locations to substitute
     * @param symbol to apply
     */
    template <Slot S>
    void replace(S &slot, std::vector<point_t> &locations, sym_t symbol) noexcept
    {
        for (const point_t &loc : locations)
            slot.at(loc.first).at(loc.second) = symbol;
    }

    /**
     *  @brief Replaces one symbol with another
     *  @param slot to modify
     *  @param sym to search and substitute
     *  @param sub_sym to insert
     */
    template <Slot S>
    void replace(S &slot, sym_t sym, sym_t sub_sym) noexcept
    {
        for (auto &reel : slot)
            std::ranges::replace(reel, sym, sub_sym);
    }

    /**
     *  @brief Substitute all symbol flagged as true in Mask with symbol
     *  @param slot to modify
     *  @param sym to apply
     */
    template <Slot S, Mask M>
    void replace(S &slot, M &mask, sym_t sym)
    {
        for (size_t i{0}; i < slot.size(); ++i)
            for (size_t j{0}; j < slot[i].size(); ++j)
                if (mask[i][j])
                    slot[i][j] = sym;
    }

    /**
     * @brief Clears the slot
     * @param slot to clear
     */
    template <Slot S>
    void clear(S &slot) noexcept
    {
        for (auto &reel : slot)
            std::ranges::fill(reel, MAX_UNSIGNED);
    }

    /**
     *  @brief Counts the symbols in a reel
     *  @param reel the reel from which count
     *  @returns an unordered map of symbols and their occurrence in the reel
     */
    template <SymbolReel R>
    [[nodiscard]] std::unordered_map<sym_t, occ_t> count(const R &reel) noexcept
    {
        std::unordered_map<sym_t, occ_t> out;
        for (const auto &sym : reel)
            ++out[sym];
        return out;
    };

    /**
     * @brief Counts the number of symbol in the slot
     * @param slot to use
     * @param symbol to count
     * @returns The number of symbols in slot
     */
    template <Slot S>
    [[nodiscard]] occ_t count(const S &slot, const sym_t symbol) noexcept
    {
        occ_t out{0};
        for (auto &reel : slot)
            out += std::ranges::count(reel, symbol);
        return out;
    }

    /// TODO: make the tests

    /**
     *  @brief Counts the number of symbols in the specified reel
     */
    template <Slot S, typename R>
    [[nodiscard]] occ_t count(const S &slot, const sym_t symbol, const R reel) noexcept
    {
        return std::ranges::count(slot.at(reel), symbol);
    }

    /**
     * @brief Finds all position in the slot which have sym
     * @param slot to count from
     * @param sym to find
     * @returns The locations in which there is sym. Output is packaged as a vector of (reel, row)
     */
    template <Slot S>
    [[nodiscard]] std::vector<point_t> find(const S &slot, const sym_t symbol) noexcept
    {
        std::vector<point_t> out;

        for (const auto &[i, reel] : std::ranges::enumerate_view(slot))
            for (const auto &[j, sym] : std::ranges::enumerate_view(reel))
                if (sym == symbol)
                    out.push_back(std::make_pair(i, j));
        return out;
    }

    /**
     *  Prints the slot using ASCII characters.
     *  The internal representation is a transposed wrt the printed slot.
     *  Since the slot may have variable number of rows
     *  we first need to get the maximum number of rows, then print row-by-row.
     *  The cells itself self-adjusts to the maximum string length of the symbols
     *  leaving the symbol with the maximum string length with two empty spaces in both sides as padding.
     *  If the slot has variable rows and it needs to print a reel without symbol, the function uses
     *  a substitute: '*'.
     */
    template <Slot S>
    void print_slot(const S &slot) noexcept
    {
        std::vector<unsigned> rows;
        for (const auto &reel : slot)
            rows.push_back(reel.size());

        unsigned max_row = std::ranges::max(rows);
        size_t strlen{0};
        std::string sub = "*";

        for (const auto &reel : slot)
            for (const auto &sym : reel)
                strlen = std::max(strlen, std::to_string(sym).length());
        strlen += 2;
        if (strlen < 10)
            strlen += 5;

        const auto print_table_div = [&]()
        {
            for (size_t i{0}; i < slot.size(); ++i)
                std::cout << '+' << std::string(strlen, '-');
            std::cout << "+\n";
        };

        std::cout << "\nSlot: \n";

        for (size_t row{0}; row < max_row; ++row)
        {
            print_table_div();
            for (size_t reel{0}; reel < slot.size(); ++reel)
            {
                std::cout << '|';
                std::string str_sym;
                if (row >= slot.at(reel).size())
                    str_sym = sub;
                else
                    str_sym = std::to_string(slot.at(reel).at(row));
                size_t head_offset = (strlen - str_sym.length()) / 2;
                size_t tail_offset = strlen - head_offset - str_sym.length();
                std::cout << std::string(head_offset, ' ') << str_sym << std::string(tail_offset, ' ');
            }
            std::cout << "|\n";
        }
        print_table_div();
    }

};

namespace statistic
{

    /**
     *  Computes the RTP, its standard deviation and the std.dev divided by the square root of the number of simuations
     */
    [[nodiscard]] inline const std::vector<double> compute(const win_dist_t &dist, const double bet) noexcept
    {
        size_t n_sims{0};
        win_t win_sum{0};
        win_t win_sum_squared{0};
        std::array<size_t, 3> data = {{n_sims, win_sum, win_sum_squared}};

        data = std::accumulate(dist.begin(), dist.end(),
                               data,
                               [](decltype(data) acc, const auto &pair)
                               {
                                   acc[0] += pair.second;
                                   acc[1] += pair.first * pair.second;
                                   acc[2] += pair.first * pair.first * pair.second;
                                   return acc;
                               });

        double rtp = data[1] / bet / data[0];
        double rtp_std = data[2] / (bet * bet) / data[0] - rtp * rtp;
        rtp_std = std::sqrt(rtp_std);
        double rtp_error = rtp_std / std::sqrt(data[0]);

        return {rtp, rtp_std, rtp_error};
    };

};
