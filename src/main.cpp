#include <iostream>
#include <functional>
#include <random>
#include <time.h>
#include <thread>
#include <cmath>
#include <vector>
#include <crashgame/header.h>
#include <crashgameutils/header.h>

std::vector<unsigned> getMulipliers();
std::vector<double> getCrashProb();
std::vector<double> getCrashAt();

const unsigned kMaxTick = 4095;
const double kAlpha = 0.00208;
const double kRTP = 0.961;
const double kProbThres = 0.5;
const double kProb1 = 0.001;
const double kProb2 = 0.01;

int main()
{
    constexpr size_t n_sims = static_cast<unsigned>(1e8);
    const size_t n_threads = std::thread::hardware_concurrency();

    /*
        Boilerplate

        unsigned M = 20;
    */

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution d(0., 1.);

    std::vector<unsigned> mults = getMulipliers();
    std::vector<double> crash_prob = getCrashProb();
    std::vector<double> crash_at = getCrashAt();
    std::vector<double> F;

    double cumulative = 0;
    for (int i = 0; i <= kMaxTick; i++)
    {
        cumulative += crash_at.at(i);
        // cumulative += 1 / static_cast<double>(kMaxTick + 1);
        F.emplace_back(cumulative);
    }
    F.at(kMaxTick) = 1;

    const auto func = std::function<const unsigned()>(
        [&]() -> const unsigned
        {
            unsigned index = cg::binarySearch(F, d(gen));
            unsigned ending_mult = 100;

            if (index)
            {
                double diff = mults.at(index) - mults.at(index - 1);
                double prob_test = diff < kProbThres ? kProb2 : kProb1;

                if (diff && d(gen) < prob_test)
                {
                    int r = 1 + 100 * diff * d(gen);
                    ending_mult = mults.at(index - 1) + r;
                }
                else
                    ending_mult = mults.at(index);
            }

            return ending_mult;

            /*
                Boilerplate

                double u = d(gen);
                if (M * u < 1)
                    return 0;
                else
                    return std::ceil(100 * M * u);
            */
        });

    auto t1 = std::clock();
    CrashGame cg(n_threads, n_sims, func);
    auto t2 = std::clock();

    cg.win_rules.emplace_back(WinRule(BetType::OVER, 1.01, std::make_pair<unsigned, unsigned>(102U, 500001U)));

    /*
        Boilerplate

        cg.win_rules.emplace_back(WinRule(BetType::OVER, 1.0, std::make_pair<unsigned, unsigned>(101U, 3000U)));
    */

    cg.evaluateWins();
    cg::printRules(cg);
    cg::writeWinDist(cg.getWinDist(), "win_dist");

    int tot_ms = (t2 - t1) * 1000 / CLOCKS_PER_SEC;
    int ms = tot_ms % 1000;
    int tot_sec = (tot_ms - ms) / 1000;
    int sec = tot_sec % 60;
    int min = (tot_sec - sec) / 60;

    // sec -= 60 * min;
    // ms -= 60 * 1000 * min + 1000 * sec;

    std::cout << "Time taken to simulate: " << min << " min " << sec << " sec " << ms << " ms" << std::endl;

    return 0;
}

std::vector<unsigned> getMulipliers()
{
    std::vector<unsigned> out;
    for (int i = 0; i <= kMaxTick; i++)
    {
        unsigned mult = std::round(std::exp(i * kAlpha) * 100);
        out.emplace_back(std::min(mult, static_cast<unsigned>(500000)));
    }

    return out;
}

std::vector<double> getCrashProb()
{
    std::vector<double> out;
    std::vector<unsigned> mults = getMulipliers();
    out.emplace_back(100 * (1 - kRTP) / mults.at(1));

    for (int i = 1; i < kMaxTick; i++)
    {
        double prob = (mults.at(i + 1) - mults.at(i)) / static_cast<double>(mults.at(i + 1));
        out.emplace_back(std::round(prob * 1e8) / 1e8);
    }

    out.emplace_back(1);
    return out;
}

std::vector<double> getCrashAt()
{
    std::vector<double> crash_prob = getCrashProb();
    std::vector<double> cumProdOneMinus;
    std::vector<double> out;

    double dp = 1 - crash_prob.at(0);
    cumProdOneMinus.emplace_back(dp);
    for (int i = 1; i <= kMaxTick; i++)
    {
        dp *= 1 - crash_prob.at(i);
        cumProdOneMinus.emplace_back(dp);
    }

    out.emplace_back(crash_prob.at(0));
    for (int i = 1; i <= kMaxTick; i++)
        out.emplace_back(cumProdOneMinus.at(i - 1) * crash_prob.at(i));
    return out;
}