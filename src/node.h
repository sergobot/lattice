/* Copyright 2020, Sergey Popov (me@sergobot.me) */

#ifndef LATTICE_NODE_H
#define LATTICE_NODE_H

#include <utility>
#include <vector>
#include <memory>

namespace lattice {

class Node {
public:
    explicit Node() : nodes() {};
    explicit Node(std::vector<Node*> nodes) : nodes(std::move(nodes)) {}

    std::vector<Node*> nodes;
};

}

#endif //LATTICE_NODE_H
