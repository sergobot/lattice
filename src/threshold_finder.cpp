/* Copyright 2020, Sergey Popov (me@sergobot.me) */

#include <future>
#include <numeric>
#include "threshold_finder.h"
#include "lattice.h"

namespace lattice {

ThresholdFinder::Result::Result(std::vector<double> th) : thresholds(std::move(th)) {
}

void ThresholdFinder::Result::append(const ThresholdFinder::Result &another) {
    thresholds.insert(thresholds.end(), another.thresholds.begin(), another.thresholds.end());
}

double ThresholdFinder::Result::average() {
    return std::accumulate(thresholds.begin(), thresholds.end(), 0.0) / thresholds.size();
}

/* static */ ThresholdFinder::Result
ThresholdFinder::run(size_t iterations, size_t threads, Mode mode, const std::function<std::unique_ptr<Lattice>()> &generator) {
    auto results = std::vector<std::future<Result>>();
    for (size_t i = 0; i < threads; ++i) {
        results.push_back(std::async(
                &ThresholdFinder::find_threshold,
                generator,
                iterations / threads + (i < iterations % threads),
                mode
        ));
    }

    Result final;
    for (auto &r : results) {
        final.append(r.get());
    }

    return final;
}

/* static */ ThresholdFinder::Result
ThresholdFinder::find_threshold(const std::function<std::unique_ptr<Lattice>()> &generator, size_t iterations, Mode mode) {
    std::random_device dev;
    std::mt19937 rng(dev());

    std::vector<double> thresholds;
    std::vector<size_t> path;

    for (size_t i = 0; i < iterations; ++i) {
        auto lat = generator();
        auto &nodes = lat->nodes();
        auto &edges = lat->edges();

        const size_t total = mode == EDGES ? edges.size() : nodes.size();
        std::uniform_int_distribution<std::mt19937::result_type> dist(0, total - 1);

        size_t dropped_count = 0;
        do {
            // If it's the first iteration on a given lattice, path will be empty and it's necessary to compute it
            bool path_interrupted = path.size() == 0;

            if (mode == EDGES) {
                size_t edge_id, node_a, node_b;
                bool settled = false;
                do {
                    edge_id = dist(rng);
                    node_a = edges[edge_id].node_a;
                    node_b = edges[edge_id].node_b;

                    for (const size_t &edge : nodes[node_a].edges) {
                        if (edge == edge_id) {
                            settled = true;
                            break;
                        }
                    }
                } while (!settled);

                lat->drop_edge_between(node_a, node_b);
                if (path.size() > 0) {
                    for (size_t k = 0; k < path.size() - 1; ++k) {
                        if ((path[k] == node_a && path[k + 1] == node_b) || (path[k] == node_b && path[k + 1] == node_a)) {
                            path_interrupted = true;
                            break;
                        }
                    }
                }
            } else if (mode == NODES) {
                size_t node_id;
                do {
                    node_id = dist(rng);
                } while (nodes[node_id].edges.empty());

                lat->drop_node(node_id);
                if (path.size() > 0) {
                    for (size_t k = 0; k < path.size(); ++k) {
                        if (path[k] == node_id) {
                            path_interrupted = true;
                            break;
                        }
                    }
                }
            } else {
                return Result();
            }
            ++dropped_count;

            if (path_interrupted)
                path = is_permeable(*lat);
        } while (path.size() > 0);
        lat.reset(nullptr);
        thresholds.push_back(1 - dropped_count / double(total));
    }
    return Result(thresholds);
}

/* static */ std::vector<size_t> ThresholdFinder::is_permeable(const Lattice &lat) {
    auto source_nodes = lat.source_idx();
    std::deque<bool> visited(lat.nodes().size(), false);
    std::vector<size_t> path;

    for (size_t node: source_nodes) {
        if (!visited[node] && path_exists(lat, node, visited, path) > 0) {
            break;
        }
    }

    return path;
}

/* static */ bool ThresholdFinder::path_exists(const Lattice &lat, const size_t &from, std::deque<bool> &visited, std::vector<size_t> &path) {
    auto node = lat.nodes()[from];
    visited[from] = true;

    if (node.type == Node::Type::TARGET) {
        path.push_back(from);
        return true;
    }

    for (size_t &edge : node.edges) {
        size_t another_node = from == lat.edges()[edge].node_b ? lat.edges()[edge].node_a : lat.edges()[edge].node_b;
        if (!visited[another_node] && path_exists(lat, another_node, visited, path)) {
            path.push_back(from);
            return true;
        }
    }

    return false;
}

}
