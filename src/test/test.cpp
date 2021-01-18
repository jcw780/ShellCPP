#include <chrono>
#include <memory>
#include <utility>

#include "../shellCPP.hpp"

// Sample Test / Benchmark Function
void runtime() {
    double total = 0.0;
    std::unique_ptr<wows_shell::shell> test, test1;
    wows_shell::shellCalc sc(1);
    sc.set_max(90.0);
    unsigned int runs = 1;
    wows_shell::shellParams sp = {.460, 780, .292, 1460, 2574, 6,
                                  .033, 76,  45,   60,   0};
    wows_shell::dispersionParams dp = {10,  2.8, 1000, 5000,  0.5,
                                       0.2, 0.6, 0.8,  26630, 2.1};
    // test = std::make_unique<wows_shell::shell>(sp, dp, "Yamato");

    wows_shell::shellParams sp1 = {.457, 800, .256, 1373, 2500, 6,
                                   .033, 76,  45,   60,   0};
    wows_shell::dispersionParams dp1 = {13,   1.1, 1000, 5000,  0.6,
                                        0.25, 0.4, 0.75, 20680, 1.8};
    test = std::make_unique<wows_shell::shell>(sp1, dp1, "Kremlin");

    std::cout << wows_shell::generateHash(*test) << "\n";
    std::cout << "Started Impact\n";
    for (unsigned int i = 0; i < runs; i++) {
        auto t1 = std::chrono::high_resolution_clock::now();
        sc.calculateImpact<wows_shell::numerical::adamsBashforth5, false>(
            *test, false);
        auto t2 = std::chrono::high_resolution_clock::now();
        total += (double)std::chrono::duration_cast<std::chrono::nanoseconds>(
                     t2 - t1)
                     .count();
    }
    std::cout << "completed" << std::endl;

    std::vector<double> angle = {0, 5, 10};
    // std::cout << "Started Post\n";
    auto t1 = std::chrono::high_resolution_clock::now();
    sc.calculatePostPen(70, 0, *test, angle, true, true);
    auto t2 = std::chrono::high_resolution_clock::now();
    // std::cout << "Started Angle\n";
    sc.calculateAngles(76, 0, *test);
    // std::cout << "Started Dispersion\n";
    test->printImpactData();
    auto maxDist = test->maxDist();
    if (std::get<0>(maxDist) != std::numeric_limits<std::size_t>::max()) {
        std::cout << std::get<0>(maxDist) << " " << std::get<1>(maxDist)
                  << "\n";
    } else {
        std::cout << std::get<0>(maxDist) << " Error\n";
    }
    std::cout << test->interpolateDistanceImpact(
                     30000, wows_shell::impact::impactIndices::rawPenetration)
              << "\n";
    // test->printTrajectory(0);
    // test->printPostPenData();
    // test->printAngleData();

    sc.calculateDispersion(*test);
    test->printDispersionData();

    std::cout << std::fixed << std::setprecision(10)
              << total / runs / 1000000000 << std::endl;
    std::cout << std::fixed << std::setprecision(10)
              << (double)std::chrono::duration_cast<std::chrono::nanoseconds>(
                     t2 - t1)
                         .count() /
                     1000000000
              << "\n";
    // std::cout<<"Ended\n";

    std::cout << wows_shell::utility::base85Encode(std::string("Hello"))
              << "\n";
}

int main() {
    for (int i = 0; i < 1; ++i) {
        runtime();
        std::cout << "Stage: " << i << " Finished \n";
    }
    return 0;
}