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

    explicit Node() : nodes() {};
    explicit Node(std::vector<Node*> nodes) : nodes(std::move(nodes)) {}

    std::vector<Node*> nodes;
    Type type;
};

}

#endif //LATTICE_NODE_H
