/* Copyright 2019, Sergey Popov (me@sergobot.me) */

#include "square_lattice.h"

#include <algorithm>
#include <ctime>
#include <iostream>

namespace lattice {
SquareLattice::SquareLattice(size_t size, std::mt19937 &rng)
        : m_size(size),
          m_rng(rng) {
    m_horizontal_edges = new bool[size * (size - 1)];
    m_vertical_edges = new bool[(size - 1) * size];
    std::fill_n(m_horizontal_edges, size * (size - 1), true);
    std::fill_n(m_vertical_edges, (size - 1) * size, true);

    m_dist = std::uniform_int_distribution<std::mt19937::result_type>(0, 2 * size * (size - 1) - 1);
}

SquareLattice::~SquareLattice() {
    delete m_horizontal_edges;
    delete m_vertical_edges;
}

double SquareLattice::find_threshold() {
    for (size_t i = 0; i < 2 * m_size * (m_size - 1); ++i) {
        size_t dropped = m_dist(m_rng);
        auto orientation = EdgeOrientation(dropped % 2);
        bool *edges = orientation == Horizontal ? m_horizontal_edges : m_vertical_edges;
        size_t id = dropped / 2;

        while (!edges[id]) {
            dropped = m_dist(m_rng);
            orientation = EdgeOrientation(dropped % 2);
            edges = orientation == Horizontal ? m_horizontal_edges : m_vertical_edges;
            id = dropped / 2;
        }

        this->drop_edge(orientation, id);

        if (!this->permeable())
            return (double) (2 * m_size * (m_size - 1) - i - 1) / (double) (2 * m_size * (m_size - 1));
    }
    return 0.;
}

size_t SquareLattice::down_edge(size_t x, size_t y) {
    return x + (m_size - 1) * y;
}

size_t SquareLattice::right_edge(size_t x, size_t y) {
    return (m_size - 1) * x + y;
}

void SquareLattice::drop_edge(EdgeOrientation orientation, size_t id) {
    if (orientation == Horizontal)
        m_horizontal_edges[id] = false;
    else
        m_vertical_edges[id] = false;
}

bool SquareLattice::path_exists(bool *horizontal_visited, bool *vertical_visited, size_t x, size_t y) {
    if (x >= m_size || y >= m_size)
        return false;

    if (x == m_size - 1)
        return true;

    size_t down = down_edge(x, y);
    size_t up = down - 1;
    size_t right = right_edge(x, y);
    size_t left = right - 1;

    // There is always a down edge, because we already excluded last line nodes.
    // Down
    if (m_vertical_edges[down] && !vertical_visited[down]) {
        vertical_visited[down] = true;
        if (path_exists(horizontal_visited, vertical_visited, x + 1, y))
            return true;
    }

    // Right
    if (y < m_size - 1 && m_horizontal_edges[right] && !horizontal_visited[right]) {
        horizontal_visited[right] = true;
        if (path_exists(horizontal_visited, vertical_visited, x, y + 1))
            return true;
    }

    // Left
    if (y > 0 && m_horizontal_edges[left] && !horizontal_visited[left]) {
        horizontal_visited[left] = true;
        if (path_exists(horizontal_visited, vertical_visited, x, y - 1))
            return true;
    }

    // Up
    if (x > 0 && m_vertical_edges[up] && !vertical_visited[up]) {
        vertical_visited[up] = true;
        if (path_exists(horizontal_visited, vertical_visited, x - 1, y))
            return true;
    }

    return false;
}

bool SquareLattice::permeable() {
    bool horizontal_visited[(m_size - 1) * m_size];
    bool vertical_visited[m_size * (m_size - 1)];
    std::fill_n(horizontal_visited, (m_size - 1) * m_size, false);
    std::fill_n(vertical_visited, m_size * (m_size - 1), false);

    for (size_t i = 0; i < m_size; ++i) {
        if (this->path_exists(horizontal_visited, vertical_visited, 0, i)) {
            return true;
        }
    }

    return false;
}

void SquareLattice::print(const bool *horizontal_visited, const bool *vertical_visited) {
    const std::string white("\033[0;47m");
    const std::string green("\033[0;42m");
    const std::string blue("\033[0;44m");
    const std::string reset("\033[0m");

    for (size_t i = 0; i < m_size; ++i) {
        for (size_t j = 0; j < m_size - 1; ++j) {
            size_t right = right_edge(i, j);
            if (m_horizontal_edges[right])
                if (horizontal_visited[right])
                    std::cout << blue << "   " << green << "   " << reset;
                else
                    std::cout << blue << "   " << white << "   " << reset;
            else
                std::cout << blue << "   " << reset << "   " << reset;
        }
        std::cout << blue << "   " << reset << std::endl;

        if (i < m_size - 1) {
            for (size_t j = 0; j < m_size; ++j) {
                size_t down = down_edge(i, j);
                if (m_vertical_edges[down])
                    if (vertical_visited[down])
                        std::cout << green << "   " << reset << "   ";
                    else
                        std::cout << white << "   " << reset << "   ";
                else
                    std::cout << reset << "   " << reset << "   ";
            }
        }
        std::cout << std::endl;
    }
}
}