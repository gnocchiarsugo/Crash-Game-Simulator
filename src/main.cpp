#include <iostream>
#include <functional>
#include <random>
#include <time.h>
#include <thread>
#include <cmath>
#include <vector>
#include <crashgame/header.h>
#include <crashgameutils/header.h>

int main()
{
    const size_t n_sims = static_cast<unsigned>(1e7);
    const size_t n_threads = std::thread::hardware_concurrency();
    unsigned M = 20;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution d(0., 1.);

    const auto func = std::function<const unsigned()>(
        [&]() -> const unsigned
        {
            double u = d(gen);
            if (M * u < 1)
                return 0;
            else
                return std::ceil(100 * M * u);
        });

    auto t1 = std::clock();
    CrashGame cg(n_threads, n_sims, func);
    auto t2 = std::clock();

    cg.win_rules.emplace_back(WinRule(BetType::OVER, 1.0, std::make_pair<unsigned, unsigned>(101U, 3000U)));

    cg.evaluateWins();
    cg::printRules(cg);
    cg::writeWinDist(cg.getWinDist(), "win_dist");

    int tot_ms = (t2 - t1) * 1000 / CLOCKS_PER_SEC;
    int ms = tot_ms % 1000;
    int tot_sec = (tot_ms - ms) / 1000;
    int sec = tot_sec % 60;
    int min = (tot_sec - sec) / 60;

    std::cout << "Time taken to simulate: " << min << " min " << sec << " sec " << ms << " ms" << std::endl;

    return 0;
}