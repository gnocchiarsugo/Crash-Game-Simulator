#include <stdexcept>
#include <string>
#include <utility>
#include <cmath>
#include <numeric>
#include "crashgame/include/data.h"

/*
 *   BetType
 */

std::ostream &operator<<(std::ostream &os, BetType type)
{
    switch (type)
    {
    case BetType::AUTO:
        os << "AUTO";
        break;
    case BetType::OVER:
        os << "OVER";
        break;
    case BetType::UNDER:
        os << "UNDER";
        break;
    case BetType::RANGE:
        os << "RANGE";
        break;
    }
    return os;
}

/*
 *   WinRule
 */

WinRule::WinRule(BetType &&type, double &&multi, std::pair<unsigned, unsigned> &&ab)
    : type(std::move(type)),
      multi(std::move(multi)),
      ab(std::move(ab))
{
    if (multi < 1)
        throw std::invalid_argument("The mutliplier is < 1 ( " + std::to_string(multi) + " < 1 )!");
    else if (ab.first < 100U || ab.second <= 100U || ab.first > ab.second)
        throw std::invalid_argument("The interval [" +
                                    std::to_string(ab.first) + ", " +
                                    std::to_string(ab.second) + "] is invalid!");
    else if (type == BetType::AUTO && ab.first != ab.second)
        throw std::invalid_argument("BetType is AUTO but " +
                                    std::to_string(ab.first) + " != " +
                                    std::to_string(ab.second) + " !");
    else if (type == BetType::OVER && ab.first == 100)
        throw std::invalid_argument("The interval [" +
                                    std::to_string(ab.first) + ", " +
                                    std::to_string(ab.second) + "] is invalid!");
    else if (type == BetType::UNDER && ab.first != 100)
        throw std::invalid_argument("BetType is UNDER but " +
                                    std::to_string(ab.first) + " != 0 !");
    else if (type == BetType::RANGE && (ab.first == 100 || ab.first == ab.second))
        throw std::invalid_argument("The interval [" +
                                    std::to_string(ab.first) + ", " +
                                    std::to_string(ab.second) + "] is invalid!");
}

WinRule::WinRule(const WinRule &other)
    : type(other.type),
      multi(other.multi),
      ab(other.ab) {}

WinRule::WinRule(WinRule &&other)
    : type(std::move(other.type)),
      multi(std::move(other.multi)),
      ab(std::move(other.ab)) {}

void WinRule::computeRTP(const WinDist &win_dist, const size_t &n_sims)
{
    if (win_dist.empty())
        throw std::invalid_argument("win_dist is empty.");

    unsigned n_wins{0};

    switch (type)
    {
    case BetType::AUTO:
        n_wins = win_dist.find(ab.first)->second;
        break;
    case BetType::OVER:
        for (auto it = win_dist.upper_bound(ab.first - 1U); it != win_dist.end(); ++it)
            n_wins += it->second;
        break;
    case BetType::UNDER:
        for (auto it = win_dist.begin(); it != win_dist.lower_bound(ab.second); ++it)
            n_wins += it->second;
        break;
    case BetType::RANGE:
        for (auto it = win_dist.upper_bound(ab.first - 1U); it != win_dist.lower_bound(ab.second); ++it)
            n_wins += it->second;
        break;
    default:
        throw std::invalid_argument("BetType not supported.");
    }
    RTP = multi * n_wins / static_cast<double>(n_sims);
    RTPstd = std::sqrt((std::pow(multi, 2) * n_wins * (n_sims - n_wins)) / std::pow(static_cast<double>(n_sims), 2));
}

/*
 *   DataCollector
 */

DataCollector &DataCollector::operator+=(DataCollector &&other)
{
    WinDist tmp;
    tmp.merge(other._win_dist);
    _win_dist.merge(tmp);
    for (auto &[key, value] : tmp)
        _win_dist.at(key) += std::move(value);
    return *this;
}