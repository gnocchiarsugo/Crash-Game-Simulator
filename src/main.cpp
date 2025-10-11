#include <iostream>
#include <functional>
#include <random>
#include <ranges>
#include <string>
#include <time.h>
#include <thread>
#include <cmath>
#include <vector>
#include <crashgame/header.h>

void print_outcomes(std::vector<std::pair<rtp_t, rtp_std_t>> &outcomes);

int main()
{
    const size_t n_sims = static_cast<size_t>(1e5);
    const size_t n_threads = std::thread::hardware_concurrency();

    unsigned M = 20;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution d(0., 1.);

    const auto func = std::function<const unsigned()>([&]() -> const unsigned
                                                      {
            double u = d(gen);
            if (M * u < 1)
                return 0;
            else
                return std::ceil(100 * M * u); });

    CrashGame cg(n_threads, n_sims, func);
    auto t1 = std::clock();
    cg.play();
    auto t2 = std::clock();

    const strategy_t over([&](const WinDist &wd) -> const std::pair<rtp_t, rtp_std_t>
                          {
                              mult_t over_mult = 100U;
                              size_t n_sims = std::accumulate(wd.win_dist.begin(),
                                                                wd.win_dist.end(),
                                                                0U, [&](unsigned acc, const std::pair<mult_t, occ_t> &p)
                                                                { return acc += p.second; });

                              size_t n_wins = std::accumulate(wd.win_dist.begin(),
                                                                wd.win_dist.end(),
                                                                0U, [&](unsigned acc, const std::pair<mult_t, occ_t> &p)
                                                                {
                                                                    if (p.first > over_mult-1)
                                                                        return acc + p.second;
                                                                    else
                                                                        return acc; });
                              rtp_t rtp = over_mult * n_wins / static_cast<double>(n_sims*100);
                              rtp_std_t rtp_std = std::sqrt((std::pow(over_mult,2) * n_wins * (n_sims-n_wins)) / static_cast<double>(std::pow(100,2) * std::pow(n_sims,2)));
                              rtp_std /= std::sqrt(n_sims);
                              return std::make_pair(rtp, rtp_std); });

    auto values = cg.evaluate(std::vector<strategy_t>{over});

    int tot_ms = (t2 - t1) * 1000 / CLOCKS_PER_SEC;
    int ms = tot_ms % 1000;
    int tot_sec = (tot_ms - ms) / 1000;
    int sec = tot_sec % 60;
    int min = (tot_sec - sec) / 60;

   print_outcomes(values);

    std::cout << "Time taken to simulate: " << min << " min " << sec << " sec " << ms << " ms" << std::endl;

    return 0;
}

void print_outcomes(std::vector<std::pair<rtp_t, rtp_std_t>> &outcomes)
{
    std::string div = "=================================";

    for (const auto &[i, outcome] : std::views::enumerate(outcomes))
    {
        std::cout << div << std::endl;
        std::cout << "Rule #" << i << std::endl;
        std::cout << "RTP: " << outcome.first << std::endl;
        std::cout << "RTP std: " << outcome.second << std::endl;
    }
    std::cout << div << std::endl;
}
