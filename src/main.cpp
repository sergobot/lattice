#include <iostream>
#include <fstream>

#include "square_lattice.h"

using namespace lattice;

int main(int argc, char *argv[]) {
    std::random_device dev;
    std::mt19937 rng(dev());

    std::ofstream fout("percolation_thresholds.csv");
    fout << "size,threshold" << std::endl;

    const size_t sizes[] = {10, 20, 50, 100, 250, 500, 1000};
    const size_t total_tries = 1000;

    for (size_t size: sizes) {
        std::cout << "Computing percolation threshold for " << size << "x" << size << " square lattice" << std::endl
                  << "Will do " << total_tries << " iterations" << std::endl;

        for (size_t i = 0; i < total_tries; ++i) {
            if (i % (total_tries / 10) == 0)
                std::cout << i << " iterations completed" << std::endl;

            SquareLattice lattice(size, rng);
            double threshold = lattice.find_threshold();

            fout << size << "," << threshold << std::endl;
        }

        std::cout << "Done!" << std::endl;
    }

    std::cout << std::endl << "No more lattices! ;-)" << std::endl;
    fout.close();
    return 0;
}