/* Copyright 2020, Sergey Popov (me@sergobot.me) */

#include <iostream>
#include <thread>
#include <map>
#include <fstream>
#include <threshold_finder.h>
#include <hexagonal_lattice.h>
#include <triangular_lattice.h>
#include <square_lattice.h>

using namespace lattice;

ThresholdFinder::Result
run_threshold_finder_with_default_parameters(size_t iterations,
                                             const std::function<std::unique_ptr<Lattice>()> &generator) {
    const auto processor_count = std::thread::hardware_concurrency();
    return ThresholdFinder::run(
            iterations,
            processor_count > 0 ? processor_count : 4,
            ThresholdFinder::Mode::EDGES,
            generator
    );
}

int main() {
    enum LatticeType {
        Hexagonal, Triangular, Square
    };
    std::map<LatticeType, std::map<size_t, size_t>> settings = {
            {Hexagonal,  {{10, 1000}, {50, 1000}, {100, 250}, {250, 50}, {500, 10}, {1000, 10}}},
            {Triangular, {{10, 1000}, {50, 1000}, {100, 250}, {250, 50}, {500, 10}, {1000, 10}}},
            {Square,     {{10, 1000}, {50, 1000}, {100, 250}, {250, 50}, {500, 10}, {1000, 10}}}
    };

    std::map<LatticeType, std::string> type_to_name = {
            {Hexagonal,  "hexagonal"},
            {Triangular, "triangular"},
            {Square,     "square"}
    };

    for (const auto &type_to_sizes : settings) {
        LatticeType type = type_to_sizes.first;
        std::string name = type_to_name[type];

        std::cout << "Starting to compute thresholds of " << name << " lattices!" << std::endl;
        for (const auto &size_to_count : type_to_sizes.second) {
            size_t size = size_to_count.first;
            size_t count = size_to_count.second;

            ThresholdFinder::Result result;
            if (type == Hexagonal) {
                result = run_threshold_finder_with_default_parameters(
                        count,
                        [size]() -> std::unique_ptr<Lattice> { return std::make_unique<HexagonalLattice>(size); }
                );
            } else if (type == Triangular) {
                result = run_threshold_finder_with_default_parameters(
                        count,
                        [size]() -> std::unique_ptr<Lattice> { return std::make_unique<TriangularLattice>(size); }
                );
            } else if (type == Square) {
                result = run_threshold_finder_with_default_parameters(
                        count,
                        [size]() -> std::unique_ptr<Lattice> { return std::make_unique<SquareLattice>(size); }
                );
            }
            auto filename = name + "_" + std::to_string(size) + "_" + std::to_string(count) + ".csv";
            std::ofstream output(filename);

            for (const auto &threshold : result.thresholds) {
                output << threshold << std::endl;
            }
            output.close();

            std::cout << count << " iterations with " << size << "x" << size << ": " << result.average() << std::endl;
        }
    }
    std::cout << "Done!" << std::endl;
    return 0;
}