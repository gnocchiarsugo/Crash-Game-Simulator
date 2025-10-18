#include <iostream>
#include <functional>
#include <random>
#include <algorithm>
#include <utility>
#include <string>
#include <thread>
#include <cmath>
#include <vector>
#include <chrono>
#include <crashgame/header.h>

// void print_outcomes(std::vector<std::pair<rtp_t, rtp_std_t>> &outcomes);

int main()
{
    const size_t n_sims = static_cast<size_t>(1e9);
    const size_t n_threads = std::thread::hardware_concurrency();

    unsigned M = 20;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution d(0., 1.);

    const generator_t<unsigned> func = [&]() -> const unsigned
    {
        double u = d(gen);
        if (M * u < 1)
            return 0;
        else
            return std::ceil(100 * M * u);
    };

    CrashGame<unsigned> cg(n_threads, n_sims, func);
    auto t1 = std::chrono::high_resolution_clock::now();
    cg.simulate();
    auto t2 = std::chrono::high_resolution_clock::now();

    const strategy_t<unsigned> over = [&](const win_dist_t<unsigned> &wd) -> std::vector<metric_t>
    {
        unsigned over_mult = 100U;
        double n_wins = std::ranges::fold_left(wd, 0.,
                                               [&over_mult](double acc, const std::pair<unsigned, occ_t> &p)
                                               {
                                                   if (p.first > over_mult - 1)
                                                       return acc + p.second;
                                                   else
                                                       return acc;
                                               });

        metric_t rtp = over_mult * n_wins / (n_sims * 100);
        metric_t rtp_std = std::sqrt((std::pow(over_mult, 2) * n_wins * (n_sims - n_wins)) / static_cast<double>(std::pow(100, 2) * std::pow(n_sims, 2)));
        rtp_std /= std::sqrt(n_sims);
        return {rtp, rtp_std};
    };

    const auto results = over(cg.get_wd());
    std::string div = "===================================";
    double ALPHA = 1.95;
    std::cout << div << std::endl;
    std::cout << "Simulations: " << n_sims << std::endl;
    std::cout << "Threads: " << n_threads << std::endl;
    std::cout << "RTP: " << results.at(0) << std::endl;
    std::cout << "RTP std: " << results.at(1) << std::endl;
    std::cout << "95% CI: [" << results.at(0) - ALPHA * results.at(1) << ", " << results.at(0) + ALPHA * results.at(1) << "]" << std::endl;
    std::cout << div << std::endl;
    {
        using namespace std::chrono;
        auto duration = duration_cast<milliseconds>(t2 - t1);
        auto mins = duration_cast<minutes>(duration);
        duration -= mins;

        auto secs = duration_cast<seconds>(duration);
        duration -= secs;

        auto millis = duration_cast<milliseconds>(duration);
        std::cout << "Time taken to simulate: " << mins << " min " << secs << " sec " << millis << " ms" << std::endl;
    }

    return 0;
}

// void print_outcomes(std::vector<std::pair<rtp_t, rtp_std_t>> &outcomes)
// {
//     std::string div = "=================================";

//     for (const auto &[i, outcome] : std::views::enumerate(outcomes))
//     {
//         std::cout << div << std::endl;
//         std::cout << "Rule #" << i << std::endl;
//         std::cout << "RTP: " << outcome.first << std::endl;
//         std::cout << "RTP std: " << outcome.second << std::endl;
//     }
//     std::cout << div << std::endl;
// }
