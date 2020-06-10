/* Copyright 2020, Sergey Popov (me@sergobot.me) */

#ifndef LATTICE_NODE_H
#define LATTICE_NODE_H

#include <utility>
#include <vector>
#include <memory>

namespace lattice {

class Node {
public:
    enum Type {
        SOURCE, INTERMEDIATE, TARGET
    };

    explicit Node() : edges(), type(INTERMEDIATE) {};

    explicit Node(std::vector<size_t> edges, Type type) : edges(std::move(edges)), type(type) {}

    std::vector<size_t> edges;
    Type type;
};

}

#endif //LATTICE_NODE_H
