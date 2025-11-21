#include <iostream>
#include <slot/header.h>
#include <utility>
#include <chrono>
#include <thread>
#include "scenarios.h"

int main()
{
    const unsigned n_threads = std::thread::hardware_concurrency();
    const size_t n_sims = 1e8;

    Platform<decltype(slot)> plt(n_threads, n_sims);
    scenarios_t<decltype(slot)> scenes;
    scenes[1] = base_scene;

    using clock = std::chrono::high_resolution_clock;
    auto t1 = clock::now();
    plt.simulate(slot, scenes);
    auto t2 = clock::now();

    std::vector<double> stats = statistic::compute(plt.get_dist(), bet);

    auto div = std::string(40, '=');
    constexpr double ALPHA95 = 1.959964;
    std::cout << div << '\n';
    std::cout << "Simulation #: " << n_sims << '\n';
    std::cout << "Threads: " << n_threads << '\n';
    std::cout << "RTP: " << stats[0] << '\n';
    std::cout << "RTP std: " << stats[1] << '\n';
    std::cout << "95% CI: [" << stats[0] - ALPHA95 * stats[2] << ", " << stats[0] + ALPHA95 * stats[2] << "]" << '\n';
    std::cout << div << '\n';

    std::chrono::duration<double, std::milli> tot_ms = t2 - t1;
    auto min = std::chrono::duration_cast<std::chrono::minutes>(tot_ms);
    auto sec = std::chrono::duration_cast<std::chrono::seconds>(tot_ms - min);
    auto ms = tot_ms - min - sec;
    std::cout << "Time taken to simulate: " << min << " " << sec << " " << ms << std::endl;

    return 0;
}
