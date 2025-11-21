#pragma once
#include <ranges>
#include <unordered_map>

/**
 *  @brief  Used in custom_line_eval. Given two symbols, the reference and next symbol, return the next symbol.
 *          Either the reference or the next symbol is a special symbol
 */
using sym_algebra_t = std::function<sym_t(sym_t, sym_t)>;

namespace ss
{
    namespace win
    {

        /**
         *  Basic line evaluation.
         *
         *  Wild sustitute all.
         *  Foreach payline we store as a reference the first symbol, if it's a standard symbol, the firt different symbol that is not wild
         *  will break the winning streak and payout.
         *  If the next symbol is a wild, the streak continues while still remembering the first symbol.
         *  Now, if the payline starts with a wild, at the first non wild symbol the current number of wilds and their id is stored inside prop.
         *  The streak then continues using the new symbol, we say that wild has collapsed into that symbol.
         *  Once even that streak ends, if prop is non-empty, we need to payout the max win between the streak of only wilds
         *  or the streak in which the wilds substitue for the symbol that the wild collapsed to.
         *
         *  At the end of the foreach we sum the payouts of all paylines.
         */
        template <Slot S, Paylines P, Paytable T>
        [[nodiscard]] win_t basic_line_evaluation(const S &slot, const P &paylines, const T &paytable, const sym_t wild) noexcept
        {
            win_t win{0};
            std::unordered_map<sym_t, size_t> prop;

            for (size_t payline_index{0}; payline_index < paylines.size(); ++payline_index)
            {
                sym_t starting_sym = slot[0][paylines[payline_index][0]];
                size_t count{1};
                for (size_t reel{1}; reel < slot.size(); ++reel)
                {
                    sym_t next_sym = slot[reel][paylines[payline_index][reel]];
                    if (next_sym != starting_sym)
                    {
                        if (starting_sym == wild)
                        {
                            prop[starting_sym] = count;
                            starting_sym = next_sym;
                            count++;
                        }
                        else if (next_sym == wild)
                            count++;
                        else
                            break;
                    }
                    else
                        count++;
                }
                if (!prop.empty())
                {
                    prop[starting_sym] = count;
                    auto it = std::max_element(
                        prop.begin(),
                        prop.end(),
                        [&paytable](const auto &a, const auto &b)
                        {
                            return paytable[a.first][a.second] < paytable[b.first][b.second];
                        });
                    starting_sym = (*it).first;
                    count = (*it).second;
                }
                win += paytable[starting_sym][count];
                prop.clear();
            }
            return win;
        };


        template <Slot S, Paylines P, Paytable T, SymbolReel SR>
        void custom_line_evaluation(const S &slot, const P &paylines, const T &paytable, const SR &special_syms, const sym_algebra_t &algebra) noexcept
        {
            
        };

        /**
         *  Way evaluation:
         *  Takes a Slot which is evaluated and a Paytable that stores the
         *  symbol multipliers depending on the number of consecutive symbol from left to right.
         *  The Wild symbol substitutes all symbols.
         *
         *  strategy:   Persist a DS that collects the possible winning symbols their ways and a count.
         *              If there are wilds in the first reel, all symbols in the next reel are added to the DS.
         *              The call is applied again if in the second reel there is again a wild.
         *              Go reel by reel and update count -> count + 1 & ways -> ways * (symbol_count + wild_count)
         *              if any of the stored symbols is inside the reel.
         *              symbol_count and wild_count are the number of occurrences in the reel for symbol and wild.
         *              At the end lookup the paytable using the symbol index and the count.
         *              Multiply the lookup with the symbol ways.
         *              Return the sum of all symbol wins.
         *
         *  maintain:
         *       out = HashMap<symbol, <ways, count>>
         */
        template <Slot S, Paytable T>
        [[nodiscard]] win_t basic_way_evaluation(const S &slot, const T &paytable, const sym_t &wild) noexcept
        {

            std::unordered_map<sym_t, std::pair<occ_t, occ_t>> ds;
            // auto pre = std::unordered_map<occ_t, std::unordered_map<sym_t, occ_t>>;

            // Preprocessing:
            auto tmp = count(slot.at(0));
            for (const auto &[sym, count] : tmp)
                ds[sym] = std::make_pair(1, 1);

            bool is_wild_present = tmp.find(wild) != tmp.end();
            size_t reel_n{1};
            while (is_wild_present)
            {
                for (const auto &sym : slot.at(reel_n))
                    if (ds.find(sym) == ds.end())
                        ds[sym] = std::make_pair(1, 1);
                tmp = count(slot.at(reel_n));
                is_wild_present = tmp.find(wild) != tmp.end();
                ++reel_n;
            }

            // Algo:
            for (size_t i{0}; i < slot.size(); ++i)
            {
                tmp = count(slot.at(i));
                for (auto &[symbol, pair] : ds)
                {
                    auto &[ways, count] = pair;
                    if ((tmp.find(symbol) != tmp.end() || tmp.find(wild) != tmp.end()) && count == i + 1)
                    {
                        count++;
                        occ_t wild_count = tmp.find(wild) != tmp.end() ? tmp[wild] : 0;
                        ways *= (tmp[symbol] + wild_count);
                    }
                }
            }

            // Win evaluation
            win_t win{0};
            for (const auto &[sym, pair] : ds)
                win += paytable[sym][pair.second - 1] * pair.first;

            return win;
        };

        /**
         *
         *
         */
    };
};
