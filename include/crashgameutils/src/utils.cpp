#include <iostream>
#include <iomanip>
#include <ranges>
#include <string>
#include <stdexcept>
#include <fstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include "crashgameutils/include/utils.h"

const unsigned cg::binarySearch(const std::vector<double> &arr, const double &target)
{
    return std::distance(arr.begin(), std::upper_bound(arr.begin(), arr.end(), target));
}

void cg::printRules(const CrashGame &cg)
{

    for (const auto &[i, win_rule] : std::views::enumerate(cg.win_rules))
    {
        const double ALPHA = 1.95996;
        const double sample_std = win_rule.RTPstd / std::sqrt(cg.getSims());
        std::cout << std::setprecision(std::ceil(std::log10(abs(cg.getSims()))));
        std::cout << "====================================================" << std::endl;
        std::cout << "Rule #" << i << ":" << std::endl;
        std::cout << "Type: " << win_rule.type << std::endl;
        switch (win_rule.type)
        {
        case BetType::AUTO:
            std::cout << "Cashout at: " << win_rule.ab.first / static_cast<double>(100) << std::endl;
            break;
        case BetType::OVER:
            std::cout << "Cashout from: " << win_rule.ab.first / static_cast<double>(100) << std::endl;
            break;
        case BetType::UNDER:
            std::cout << "Cashout under: " << win_rule.ab.second / static_cast<double>(100) << std::endl;
            break;
        case BetType::RANGE:
            std::cout << "Cashout in the interval: [" << win_rule.ab.first / static_cast<double>(100) << ", " << win_rule.ab.second / static_cast<double>(100) << "]" << std::endl;
            break;
        }
        std::cout << "Number of simulations: " << cg.getSims() << std::endl;
        std::cout << "RTP: " << win_rule.RTP << std::endl;
        std::cout << "RTP std: " << win_rule.RTPstd << std::endl;
        std::cout << "95% CI: [" << win_rule.RTP - sample_std * ALPHA << ", " << win_rule.RTP + sample_std * ALPHA << "]" << std::endl;
    }
    std::cout << "====================================================" << std::endl;
}

void cg::writeWinDist(const WinDist &win_dist, std::string&& filename)
{
    filename = "./" + filename + ".txt";
    std::ofstream out(filename);

    if (!out.is_open())
        throw std::domain_error("Error opening win_dist.txt");

    out << "Multiplier\tOccurrence" << std::endl;
    for (const auto &item : win_dist)
        out << item.first / static_cast<double>(100) << " \t" << item.second << std::endl;

    out.close();
}
