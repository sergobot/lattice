#include <iostream>

#include "square_lattice.h"

using namespace lattice;

int main() {
    std::random_device dev;
    std::mt19937 rng(dev());

    const int total_tries = 1000;
    const int size = 100;

    std::cout << "Computing percolation threshold for a "
              << size << "x" << size << " square lattice." << std::endl
              << "Will do " << total_tries << " iterations" << std::endl;

    double thresholds[total_tries];
    for (size_t i = 0; i < total_tries; ++i) {
        if (i % (total_tries / 10) == 0)
            std::cout << i << " iterations completed" << std::endl;

        SquareLattice lattice(size, rng);
        thresholds[i] = lattice.find_threshold();
    }

    double avg = 0.;
    for (double threshold : thresholds)
        avg += threshold;

    avg /= total_tries;

    std::cout << "Threshold is " << avg << std::endl;
    return 0;
}