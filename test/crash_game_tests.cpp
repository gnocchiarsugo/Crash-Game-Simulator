#include <functional>
#include <numeric>
#include <catch2/catch_all.hpp>
#include <crashgame/header.h>

TEST_CASE("Crash Game")
{
    ;
    SECTION("If multiplier is constant and there are no winning rules, the winning dist is n_sims times the multiplier, main", "")
    {
        size_t n_sims = 1000;
        size_t n_threads = 4;
        mult_t m = 103;
        const auto f = std::function<const mult_t()>([m]() -> const unsigned
                                                     { return m; });
        CrashGame cg(n_threads, n_sims, f);
        cg.play();
        REQUIRE(cg.get_wd().sorted_win_dist.at(m) == n_sims);
    }

    SECTION("If multiplier is constant and the OVER stategy has a lower treshold, the RTP is the multiplier", "")
    {
        size_t n_sims = 1000;
        size_t n_threads = 4;
        mult_t m = 140;
        const auto f = std::function<const mult_t()>([m]() -> const unsigned
                                                     { return m; });
        CrashGame cg(n_threads, n_sims, f);
        cg.play();

        const strategy_t over([&](const WinDist &wd) -> const std::pair<rtp_t, rtp_std_t>
                              {
                              mult_t over_mult = 102U;
                              size_t n_sims = std::accumulate(wd.win_dist.begin(),
                                                                wd.win_dist.end(),
                                                                0U, [&](unsigned acc, const std::pair<mult_t, occ_t> &p)
                                                                { return acc += p.second; });

                              size_t n_wins = std::accumulate(wd.win_dist.begin(),
                                                                wd.win_dist.end(),
                                                                0U, [&](unsigned acc, const std::pair<mult_t, occ_t> &p)
                                                                {
                                                                    if (p.first >= over_mult)
                                                                        return acc + p.second;
                                                                    else
                                                                        return acc; });
                              rtp_t rtp = (over_mult * n_wins) / static_cast<double>(100 * n_sims);
                              rtp_std_t rtp_std = std::sqrt((std::pow(over_mult,2) * n_wins * (n_sims-n_wins)) / static_cast<double>(std::pow(100,2) * std::pow(n_sims,2)));
                              return std::make_pair(rtp, rtp_std); });

        auto out = cg.evaluate(std::vector<strategy_t>{over});

        REQUIRE(out.at(0).first == 1.02);
        REQUIRE(out.at(0).second == 0);
    }
}