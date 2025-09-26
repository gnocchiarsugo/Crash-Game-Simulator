#include <utility>
#include <catch2/catch_all.hpp>
#include <crashgame/header.h>

// WinRules

TEST_CASE("DataCollector constructor", "[datacollector, constructor]")
{
    ;
    SECTION("Constructor creates empty WinDist", "[constructor]")
    {
        DataCollector dc = DataCollector();
        REQUIRE(dc.getWinDist().empty());
    }
};

TEST_CASE("Conditions on addwin", "[addwin]")
{
    ;
    SECTION("addsWin from lvalue", "[addwin]")
    {
        unsigned m = 103;
        DataCollector dc;
        dc.addWin(m);
        REQUIRE(dc.getWinDist().at(m) == 1U);
    }

    SECTION("if mult is in win_dist add one to value", "[addwin]")
    {
        DataCollector dc;
        unsigned N = 3;
        unsigned m = 103;
        for (int i = 0; i < N; i++)
            dc.addWin(m);
        REQUIRE(dc.getWinDist().at(m) == N);
    }
}

TEST_CASE("Operator Overload += rvalue", "[self-addition]")
{
    ;
    SECTION("If other win_dist is empty return the same win_dist", "[self-addition]")
    {
        DataCollector dc1;
        DataCollector dc2;
        dc1 += std::move(dc2);
        REQUIRE(dc1.getWinDist().empty());
    }

    SECTION("If other win_dist has different mult, insert new mult in empty win_dist.", "[self-addition]")
    {
        DataCollector dc1;
        DataCollector dc2;
        unsigned m = 101;
        dc2.addWin(m);
        dc1 += std::move(dc2);
        REQUIRE(dc1.getWinDist().at(m) == 1U);
        REQUIRE_THROWS(dc2.getWinDist().at(m));
    }

    SECTION("If other win_dist has different mult, insert new mult", "[self-addition]")
    {
        DataCollector dc1;
        DataCollector dc2;
        unsigned m1 = 101;
        unsigned m2 = 102;
        dc1.addWin(m1);
        dc2.addWin(m2);
        dc1 += std::move(dc2);
        REQUIRE(dc1.getWinDist().at(m1) == 1U);
        REQUIRE(dc1.getWinDist().at(m2) == 1U);
        REQUIRE_THROWS(dc2.getWinDist().at(m2));
    }

    SECTION("If other win_dist has same mult, add occs.", "[self-addition]")
    {
        DataCollector dc1;
        DataCollector dc2;
        unsigned m1 = 101;
        dc1.addWin(m1);
        dc2.addWin(m1);
        dc2.addWin(m1);
        dc1 += std::move(dc2);
        REQUIRE(dc1.getWinDist().at(m1) == 3U);
        REQUIRE_THROWS(dc2.getWinDist().at(m1));
    }
}