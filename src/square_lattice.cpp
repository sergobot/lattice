/* Copyright 2019, Sergey Popov (me@sergobot.me) */

#include "square_lattice.h"

#include <algorithm>
#include <ctime>
#include <iostream>

namespace lattice {
SquareLattice::SquareLattice(size_t size, std::mt19937 &rng)
        : m_size(size),
          m_rng(rng) {
    m_lattice = new bool[size * size];
    std::fill_n(m_lattice, size * size, true);

    m_dist = std::uniform_int_distribution<std::mt19937::result_type>(0, size * size - 1);
}

SquareLattice::~SquareLattice() {
    delete m_lattice;
}

double SquareLattice::find_threshold() {
    size_t square_size = m_size * m_size;
    for (size_t i = 0; i < square_size; ++i) {
        size_t dropped = m_dist(m_rng);

        while (!m_lattice[dropped])
            dropped = m_dist(m_rng);

        this->drop(dropped);

        if (!this->permeable())
            return (double) (i + 1) / (double) (square_size);
    }
    return 1.;
}

void SquareLattice::drop(size_t position) {
    m_lattice[position] = false;
}

bool SquareLattice::path_exists(bool *visited, size_t position) {
    // Matrix is assumed to be of the same size, as the lattice itself
    if (m_lattice[position] && !visited[position]) {
        visited[position] = true;

        size_t row = position / m_size, col = position % m_size;

        // Reached the last line successfully
        if (row == m_size - 1)
            return true;

        // Down
        if (row < m_size - 1 && path_exists(visited, position + m_size))
            return true;

        // Left
        if (col > 0 && path_exists(visited, position - 1))
            return true;

        // Right
        if (col < m_size - 1 && path_exists(visited, position + 1))
            return true;

        // Up
        if (row > 0 && path_exists(visited, position - m_size))
            return true;
    }
    return false;
}

bool SquareLattice::permeable() {
    bool visited[m_size * m_size];
    std::fill_n(visited, m_size * m_size, false);

    for (size_t i = 0; i < m_size; ++i) {
        if (m_lattice[i] && !visited[i] && this->path_exists(visited, i)) {
            return true;
        }
    }
    return false;
}

void SquareLattice::print(const bool *visited) {
    const std::string green("\033[0;42m");
    const std::string white("\033[0;47m");
    const std::string red("\033[0;41m");
    const std::string reset("\033[0m");

    if (visited == nullptr) {
        for (size_t i = 0; i < m_size; ++i) {
            for (size_t j = 0; j < m_size; ++j)
                if (m_lattice[i * m_size + j])
                    std::cout << white << "  " << reset;
                else
                    std::cout << red << "  " << reset;
            std::cout << std::endl;
        }
        std::cout << std::endl;
    } else {
        for (size_t i = 0; i < m_size; ++i) {
            for (size_t j = 0; j < m_size; ++j)
                if (visited[i * m_size + j])
                    std::cout << green << "  " << reset;
                else if (m_lattice[i * m_size + j])
                    std::cout << white << "  " << reset;
                else
                    std::cout << red << "  " << reset;
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}
}