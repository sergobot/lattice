/* Copyright 2020, Sergey Popov (me@sergobot.me) */

#include <iostream>
#include <thread>
#include <threshold_finder.h>
#include <square_lattice.h>

using namespace lattice;
using namespace std;

unique_ptr<Lattice> generator() {
    return make_unique<SquareLattice>(100);
}

int main() {
    const auto processor_count = std::thread::hardware_concurrency();

    double result = ThresholdFinder::run(
            100,
            processor_count > 0 ? processor_count : 4,
            ThresholdFinder::Mode::EDGES,
            generator
    );
    cout << "The threshold is " << result << endl;
    return 0;
}