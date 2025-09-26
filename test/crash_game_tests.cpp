#include <memory>
#include <functional>
#include <catch2/catch_all.hpp>
#include <crashgame/header.h>

TEST_CASE("Crash Game", "[crash game]")
{
    ;
    SECTION("If multiplier is constant and there are no winning rules, the winning dist is n_sims times the multiplier, main", "")
    {
        unsigned n_sims = 103;
        unsigned n_threads = 4;
        unsigned m = 103;
        const auto f = std::function<const unsigned()>([m]() -> const unsigned
                                                       { return m; });
        CrashGame cg(n_threads, n_sims, f);

        REQUIRE(cg.getWinDist().at(m) == n_sims);
    }

    SECTION("If multiplier is constant and there is AUTO winning rules, the rule RTP is the mutlitplier", "")
    {
        unsigned n_sims = 103;
        unsigned n_threads = 4;
        unsigned m = 103;
        const auto f = std::function<const unsigned()>([m]() -> const unsigned
                                                       { return m; });
        CrashGame cg(n_threads, n_sims, f);

        cg.win_rules.emplace_back(WinRule(BetType::AUTO, 1.02, std::make_pair<unsigned, unsigned>(103U, 103U)));
        cg.evaluateWins();

        REQUIRE(cg.win_rules.at(0).RTP == 1.02);
        REQUIRE(cg.win_rules.at(0).RTPstd == 0);
    }
}