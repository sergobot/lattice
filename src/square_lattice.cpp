/* Copyright 2019, Sergey Popov (me@sergobot.me) */

#include "square_lattice.h"

#include <algorithm>
#include <ctime>
#include <iostream>

namespace lattice {
SquareLattice::SquareLattice(size_t size, std::mt19937 & rng)
    : size(size),
      rng(rng) {
    this->lattice = new bool[size * size];
    std::fill_n(this->lattice, size * size, true);

    this->dist = std::uniform_int_distribution<std::mt19937::result_type>(0, size * size - 1);
}

SquareLattice::~SquareLattice() {
    delete this->lattice;
}

double SquareLattice::find_threshold() {
    size_t square_size = size * size;
    for (size_t i = 0; i < square_size; ++i) {
        size_t dropped = this->dist(this->rng);

        while (!this->lattice[dropped])
            dropped = this->dist(this->rng);

        this->drop(dropped);

        if (!this->permeable())
            return (double) (i + 1) / (double) (square_size);
    }
    return 1.;
}

void SquareLattice::drop(size_t position) {
    this->lattice[position] = false;
}

bool SquareLattice::path_exists(bool *visited, size_t position) {
    // Matrix is assumed to be of the same size, as the lattice itself
    if (this->lattice[position] && !visited[position]) {
        visited[position] = true;

        size_t row = position / this->size, col = position % this->size;

        if (row == size - 1)
            return true;

        // Down
        if (row < size - 1 && path_exists(visited, position + size))
            return true;

        // Left
        if (col > 0 && path_exists(visited, position - 1))
            return true;

        // Right
        if (col < size - 1 && path_exists(visited, position + 1))
            return true;

        // Up
        if (row > 0 && path_exists(visited, position - size))
            return true;
    }
    return false;
}

bool SquareLattice::permeable() {
    bool visited[this->size * this->size];
    std::fill_n(visited, this->size * this->size, false);

    for (size_t i = 0; i < this->size; ++i) {
        if (this->lattice[i] && !visited[i] && this->path_exists(visited, i)) {
            return true;
        }
    }
    return false;
}

void SquareLattice::print_lattice(bool *lattice) {
    for (size_t i = 0; i < this->size; ++i) {
        for (size_t j = 0; j < this->size; ++j)
            std::cout << lattice[i * this->size + j] << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
}