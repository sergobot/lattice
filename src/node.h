/* Copyright 2020, Sergey Popov (me@sergobot.me) */

#ifndef LATTICE_NODE_H
#define LATTICE_NODE_H

#include <utility>
#include <vector>
#include <memory>

namespace lattice {

class Node {
public:
    enum Type {SOURCE, INTERMEDIATE, TARGET};

    explicit Node() : nodes(), type(INTERMEDIATE) {};
    explicit Node(std::vector<size_t> nodes) : nodes(std::move(nodes)), type(INTERMEDIATE) {}

    std::vector<size_t> nodes;
    Type type;
};

}

#endif //LATTICE_NODE_H
