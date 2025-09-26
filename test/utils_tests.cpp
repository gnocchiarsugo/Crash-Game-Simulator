#include <vector>
#include <fstream>
#include <crashgameutils/header.h>
#include <catch2/catch_all.hpp>

TEST_CASE("Binary search", "[binary search]")
{
    ;
    SECTION("If there are only a < b. If the target is > a then the result should be 1")
    {
        std::vector<double> arr = {0.3, 0.6};
        double t = 0.5;
        REQUIRE(cg::binarySearch(arr, t) == 1);
    }

    SECTION("If there are only a < b. If the target is < a then the result should be 0")
    {
        std::vector<double> arr = {0.3, 0.6};
        double t = 0.2;
        REQUIRE(cg::binarySearch(arr, t) == 0);
    }

    SECTION("If there are only a < b. If the target is > b then the result should be 2")
    {
        std::vector<double> arr = {0.3, 0.6};
        double t = 0.7;
        REQUIRE(cg::binarySearch(arr, t) == 2);
    }

    SECTION("With a valid CDF. If the target is < first element then the result should be 0")
    {
        std::vector<double> arr = {0.3, 0.5, 0.6, 1};
        double t = 0.2;
        REQUIRE(cg::binarySearch(arr, t) == 0);
    }

    SECTION("With a valid CDF. If the target is > first element but < second element then the result should be 1")
    {
        std::vector<double> arr = {0.2, 0.4, 0.66, 0.8, 1};
        double t = 0.3;
        REQUIRE(cg::binarySearch(arr, t) == 1);
    }

    SECTION("With a valid CDF. If the target is > last element<1 then the result should be array.size - 1")
    {
        std::vector<double> arr = {0.2, 0.4, 0.66, 0.8, 1};
        double t = 0.9;
        REQUIRE(cg::binarySearch(arr, t) == arr.size() - 1);
    }
};

TEST_CASE("Writing WinDist to file")
{
    SECTION("the function creates the file when called with empty win_dist and filename")
    {
        WinDist w;
        cg::writeWinDist(w, "test");
        std::ifstream in("./test.txt");
        REQUIRE(in.good());
    }
}
