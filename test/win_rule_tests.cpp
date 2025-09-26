#include <utility>
#include <cmath>
#include <catch2/catch_all.hpp>
#include <crashgame/header.h>

// WinRules

TEST_CASE("WinRule constructor")
{
    // Valid multiplier can't be 100 since it's money laundering
    ;
    SECTION("Constructor throws if given multiplier is < 1", "[constructor]")
    {
        REQUIRE_THROWS(WinRule(BetType::AUTO, 0.99, std::make_pair(101U, 102U)));
        REQUIRE_THROWS(WinRule(BetType::OVER, 0, std::make_pair(101U, 102U)));
        REQUIRE_THROWS(WinRule(BetType::UNDER, 0.8, std::make_pair(101U, 102U)));
        REQUIRE_THROWS(WinRule(BetType::RANGE, 0.1, std::make_pair(101U, 102U)));
    }

    SECTION("Constructor throws if A > B", "[constructor]")
    {
        REQUIRE_THROWS(WinRule(BetType::AUTO, 1, std::make_pair(2U, 1U)));
        REQUIRE_THROWS(WinRule(BetType::OVER, 1, std::make_pair(2U, 1U)));
        REQUIRE_THROWS(WinRule(BetType::UNDER, 1, std::make_pair(100U, 10U)));
        REQUIRE_THROWS(WinRule(BetType::RANGE, 1, std::make_pair(211U, 21U)));
    }

    SECTION("Constructor throws if A is < 100", "[constructor]")
    {
        REQUIRE_THROWS(WinRule(BetType::AUTO, 101, std::make_pair(10U, 101U)));
        REQUIRE_THROWS(WinRule(BetType::OVER, 101, std::make_pair(99U, 101U)));
        REQUIRE_THROWS(WinRule(BetType::UNDER, 101, std::make_pair(5U, 101U)));
        REQUIRE_THROWS(WinRule(BetType::RANGE, 101, std::make_pair(3U, 101U)));
    }

    SECTION("Constructor throws if B is <= 100", "[constructor]")
    {
        REQUIRE_THROWS(WinRule(BetType::AUTO, 101, std::make_pair(100U, 10U)));
        REQUIRE_THROWS(WinRule(BetType::OVER, 101, std::make_pair(100U, 10U)));
        REQUIRE_THROWS(WinRule(BetType::UNDER, 101, std::make_pair(100U, 100U)));
        REQUIRE_THROWS(WinRule(BetType::RANGE, 101, std::make_pair(100U, 100U)));
    }
};

TEST_CASE("Conditions on AUTO bet type", "[winrule, constructor, auto]")
{
    ;
    SECTION("Auto throws an exception if A != B", "[auto]")
    {
        REQUIRE_THROWS(WinRule(BetType::AUTO, 1, std::make_pair(200U, 201U)));
    }

    SECTION("Constructor throws if A is = 100", "[validrange]")
    {
        REQUIRE_THROWS(WinRule(BetType::AUTO, 2, std::make_pair(100U, 102U)));
    }
};

TEST_CASE("Conditions on OVER bet type", "[winrule, constructor, over]")
{
    // There may be no cap to the multiplier so B is left unspecified
    ;
    SECTION("Constructor throws if A is = 100", "[validrange]")
    {
        REQUIRE_THROWS(WinRule(BetType::OVER, 5, std::make_pair(100U, 102U)));
    }

};

TEST_CASE("Conditions on RANGE bet type", "[winrule, constructor, range]")
{
    ;
    SECTION("Constructor throws if A is = 100", "[validrange]")
    {
        REQUIRE_THROWS(WinRule(BetType::RANGE, 101, std::make_pair(100U, 102U)));
    }

    SECTION("Constructor throws if A = B", "[validrange]")
    {
        REQUIRE_THROWS(WinRule(BetType::RANGE, 101, std::make_pair(152U, 152U)));
    }
};

TEST_CASE("Copy", "[winrule, constructor, copy]")
{
    ;
    SECTION("Copy constructor copies others", "[copy]")
    {
        WinRule a = WinRule(BetType::AUTO, 101, std::make_pair(152U, 152U));
        WinRule b = WinRule(a);

        REQUIRE(a.type == b.type);
        REQUIRE(a.multi == b.multi);
        REQUIRE(a.ab == b.ab);
    }

    SECTION("Copy assignment", "[copy]")
    {
        WinRule a = WinRule(BetType::AUTO, 101, std::make_pair(152U, 152U));
        WinRule b = a;

        REQUIRE(a.type == b.type);
        REQUIRE(a.multi == b.multi);
        REQUIRE(a.ab == b.ab);
    }
};

TEST_CASE("Move", "[winrule, constructor, copy]")
{
    ;
    SECTION("Move constructor moves others", "[copy]")
    {
        WinRule a = WinRule(BetType::AUTO, 101, std::make_pair(152U, 152U));
        WinRule b = WinRule(std::move(a));

        REQUIRE(b.type == BetType::AUTO);
        REQUIRE(b.multi == 101);
        REQUIRE(b.ab == std::make_pair(152U, 152U));
    }

    SECTION("Move assignment", "[copy]")
    {
        WinRule a = WinRule(BetType::AUTO, 101, std::make_pair(152U, 152U));
        WinRule b = std::move(a);

        REQUIRE(b.type == BetType::AUTO);
        REQUIRE(b.multi == 101);
        REQUIRE(b.ab == std::make_pair(152U, 152U));
    }
};

TEST_CASE("RTP", "[winrule, RTP]")
{
    ;
    SECTION("RTP of empty WinDist is an exception", "[RTP]")
    {
        WinRule a = WinRule(BetType::AUTO, 101, std::make_pair(152U, 152U));
        WinDist wd;
        size_t N = 123;
        REQUIRE_THROWS(a.computeRTP(wd, N));
    }

    SECTION("RTP of singular value not matching the rule is zero", "[RTP]")
    {
        WinRule a = WinRule(BetType::AUTO, 101, std::make_pair(152U, 152U));
        WinDist wd;
        size_t N = 1;
        wd.insert({101, 1});
        a.computeRTP(wd, N);
        REQUIRE(a.RTP == 0);
    }

    SECTION("RTP of singular value in WinDist is multiplier", "[RTP]")
    {
        WinRule a = WinRule(BetType::AUTO, 101, std::make_pair(152U, 152U));
        WinDist wd;
        size_t N = 1;
        wd.insert({152, 1});
        a.computeRTP(wd, N);
        REQUIRE(a.RTP == 101);
    }

    SECTION("RTP of singular value matching the winrule is formula", "[RTP]")
    {
        WinRule a = WinRule(BetType::AUTO, 1.01, std::make_pair(152U, 152U));
        WinDist wd;
        unsigned N = 11;
        wd.insert({152, 1});
        wd.insert({101, 10});
        a.computeRTP(wd, N);
        auto match = Catch::Matchers::WithinRel(1.01 / static_cast<double>(11), 0.01);
        REQUIRE_THAT(a.RTP, match);
    }
}

TEST_CASE("RTPstd", "[winrule, RTPstd]")
{
    ;
    SECTION("RTPstd of empty WinDist is an exception", "[RTP]")
    {
        WinRule a = WinRule(BetType::AUTO, 101, std::make_pair(152U, 152U));
        WinDist wd;
        double N = 1.;
        REQUIRE_THROWS(a.computeRTP(wd, N));
    }

    SECTION("RTP of singular value not matching the rule is zero", "[RTP]")
    {
        WinRule a = WinRule(BetType::AUTO, 101, std::make_pair(152U, 152U));
        WinDist wd;
        unsigned N = 1;
        wd.insert({101, 1});
        a.computeRTP(wd, N);
        REQUIRE(a.RTPstd == 0);
    }

    SECTION("RTP of singular value matching is zero", "[RTP]")
    {
        WinRule a = WinRule(BetType::AUTO, 101, std::make_pair(152U, 152U));
        WinDist wd;
        unsigned N = 1;
        wd.insert({152, 1});
        a.computeRTP(wd, N);
        REQUIRE(a.RTPstd == 0);
    }

    SECTION("RTP of singular value matching the winrule with others non-matching is formula", "[RTP]")
    {
        WinRule a = WinRule(BetType::AUTO, 1.01, std::make_pair(152U, 152U));
        WinDist wd;
        unsigned N = 11;
        wd.insert({152, 1});
        wd.insert({101, 10});
        a.computeRTP(wd, N);
        auto match = Catch::Matchers::WithinRel(std::sqrt(1.01 * 1.01 * 10 / static_cast<double>(11 * 11)), 0.01);
        REQUIRE_THAT(a.RTPstd, match);
    }
};

TEST_CASE("Bet UNDER", "[under]")
{
    ;
    SECTION("UNDER throws if a > 100")
    {
        REQUIRE_THROWS(WinRule(BetType::UNDER, 1.01, std::make_pair(101U, 152U)));
    }

    SECTION("If nothing matches UNDER RTP is zero")
    {
        WinRule a = WinRule(BetType::UNDER, 101, std::make_pair(100U, 152U));
        WinDist wd;
        unsigned N = 1;
        wd.insert({170, 1});
        a.computeRTP(wd, N);
        REQUIRE(a.RTP == 0);
    }

    SECTION("Under of everything has RTP of multiplier")
    {
        WinRule a = WinRule(BetType::UNDER, 1.01, std::make_pair(100U, 152U));
        WinDist wd;
        unsigned N = 1;
        wd.insert({122, 1});
        a.computeRTP(wd, N);
        REQUIRE(a.RTP == 1.01);
    }
}

TEST_CASE("Bet OVER", "[under]")
{
    SECTION("OVER throws if a = 100")
    {
        REQUIRE_THROWS(WinRule(BetType::OVER, 1.01, std::make_pair(100U, 152U)));
    }

    SECTION("If nothing matches OVER, RTP is zero")
    {
        WinRule a = WinRule(BetType::OVER, 101, std::make_pair(152U, 152U));
        WinDist wd;
        unsigned N = 1;
        wd.insert({120, 1});
        a.computeRTP(wd, N);
        REQUIRE(a.RTP == 0);
    }

    SECTION("OVER of everything has RTP of multiplier")
    {
        WinRule a = WinRule(BetType::OVER, 1.01, std::make_pair(112U, 112U));
        WinDist wd;
        unsigned N = 1;
        wd.insert({122, 1});
        a.computeRTP(wd, N);
        REQUIRE(a.RTP == 1.01);
    }
}
TEST_CASE("Bet RANGE", "[under]")
{

    SECTION("RANGE throws if a = 100")
    {
        REQUIRE_THROWS(WinRule(BetType::RANGE, 1.01, std::make_pair(100U, 152U)));
    }

    SECTION("RANGE of everything has RTP of multiplier")
    {
        WinRule a = WinRule(BetType::RANGE, 1.01, std::make_pair(112U, 200U));
        WinDist wd;
        unsigned N = 1;
        wd.insert({122, 1});
        a.computeRTP(wd, N);
        REQUIRE(a.RTP == 1.01);
    }

    SECTION("RANGE of everything has RTP of multiplier")
    {
        WinRule a = WinRule(BetType::RANGE, 1.01, std::make_pair(112U, 200U));
        WinDist wd;
        unsigned N = 1;
        wd.insert({122, 1});
        a.computeRTP(wd, N);
        REQUIRE(a.RTP == 1.01);
    }
}
