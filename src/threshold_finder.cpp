/* Copyright 2020, Sergey Popov (me@sergobot.me) */

#include <future>
#include "threshold_finder.h"
#include "lattice.h"

namespace lattice {

/* static */ double ThresholdFinder::run(size_t iterations, size_t threads, Mode mode, Lattice& (*generator)()) {
    auto results = std::vector<std::future<double>>();
    for (size_t i = 0; i < threads; ++i) {
        results.push_back(std::async(
                &ThresholdFinder::find_threshold,
                generator,
                iterations / threads + (i == 0) * (iterations % threads),
                mode
                ));
    }

    double average = 0;
    for (auto &r : results) {
        average += r.get();
    }

    return average / iterations;
}

/* static */ double ThresholdFinder::find_threshold(Lattice &(*generator)(), size_t iterations, Mode mode) {
    std::random_device dev;
    std::mt19937 rng(dev());

    double total = 0;

    for (size_t i = 0; i < iterations; ++i) {
        Lattice &lat = generator();

        auto nodes = lat.nodes();
        size_t nodes_count = lat.nodes_count();
        std::uniform_int_distribution<std::mt19937::result_type> node_dist(0, nodes_count);

        size_t dropped_count = 0;
        do {
            size_t node_id;
            do {
                node_id = node_dist(rng);
            } while (nodes.at(node_id)->nodes.empty());

            if (mode == EDGES) {
                std::uniform_int_distribution<std::mt19937::result_type> edge_dist(0, nodes.at(node_id)->nodes.size());
                size_t another_node_id = nodes.at(node_id)->nodes.at(edge_dist(rng));

                lat.drop_edge_between(node_id, another_node_id);
            } else if (mode == NODES) {
                lat.drop_node(node_id);
            } else {
                return 0;
            }

            ++dropped_count;
        } while (is_permeable(lat));

        total += dropped_count / double(mode == EDGES ? lat.edges_count() : lat.nodes_count());
    }
    return 0;
}

/* static */ bool ThresholdFinder::is_permeable(Lattice &lat) {
    auto source_nodes = lat.source_idx();
    auto nodes = lat.nodes();
    std::deque<bool> visited(lat.nodes_count(), false);

    for (size_t node: source_nodes) {
        if (path_exists(lat, node, visited)) {
            return true;
        }
    }

    return false;
}

/* static */ bool ThresholdFinder::path_exists(Lattice &lat, size_t &from, std::deque<bool> &visited) {
    auto node = lat.nodes().at(from);
    visited[from] = true;

    if (node->type == Node::Type::TARGET) {
        return true;
    }

    for (size_t &another_node : node->nodes) {
        if (!visited.at(another_node) && path_exists(lat, another_node, visited)) {
            return true;
        }
    }

    return false;
}

}
