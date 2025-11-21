#pragma once

#include "typedefs.h"
#include "slot_typedefs.h"
#include "chacha.h"
#include <ranges>
#include <map>

namespace ss
{
    namespace spin
    {
        /**
         *  Standard spin of the slot using the reels and at the stops.
         *  Remember to fill your container at init with all the dummy
         *  values needed to be of the correct dimensions since
         *  this method fills only which is occupied.
         *
         *  Remember that stops is modified during the execution
         */
        template <Slot S, Reels R>
        void basic_spin(S &slot, const R &reels, stop_t &stops) noexcept
        {
            for (const auto &[i, reel] : std::ranges::enumerate_view(reels))
            {
                for (int j = slot.at(i).size() - 1; j >= 0; j--)
                {
                    slot[i][j] = reels[i][stops[i]];
                    stops[i] += stops[i] == 0 ? reels[i].size() - 1 : -1;
                }
            }
        };

        /**
         *  @brief Populates the slot with the extractions per reel givn by the weights in weights
         *  @param slot to populate
         *  @param weights from which etract
         *  @param rng from which extract the random numbers
         *  @param dictionary array to link random indicies and actual symbols, at sym_0 = dict[0] and so on
         * 
         *  Remember to fill your container at init with all the dummy
         *  values needed to be of the correct dimensions since
         *  this method fills only which is occupied.
         */
        template <Slot S, Weights W, SymbolReel R>
        void weighted_spin(S &slot, const W &weights, ChaCha &rng, R dictionary)
        {
            for (size_t reel{0}; reel < slot.size(); ++reel)
                for (size_t row{0}; row < slot[reel].size(); ++row)
                    slot[reel][row] = dictionary[rng.index_from_weights(weights)];
        }



        


    };
};
