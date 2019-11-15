#include <iostream>

#include "square_lattice.h"

using namespace lattice;

int main() {
    std::random_device dev;
    std::mt19937 rng(dev());

    const int total_tries = 100;
    const int size = 100;

    std::cout << "Computing percolation threshold for a "
              << size << "x" << size << " square lattice." << std::endl
              << "Will do " << total_tries << " iterations" << std::endl;

    double thresholds[total_tries];
    for (double & threshold : thresholds) {
        SquareLattice lattice(size, rng);
        threshold = lattice.find_threshold();
    }

    double avg = 0.;
    for (double threshold : thresholds)
        avg += threshold;

    avg /= total_tries;

    std::cout << "Threshold is " << avg << std::endl;
    return 0;
}