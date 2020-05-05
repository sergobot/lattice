/* Copyright 2020, Sergey Popov (me@sergobot.me) */

#ifndef LATTICE_LATTICE_H
#define LATTICE_LATTICE_H

#include "node.h"

namespace lattice {

class Lattice {
public:
    virtual size_t nodes_count() = 0;

    virtual std::vector<Node*> source_nodes() = 0;

    virtual std::vector<Node*> target_nodes() = 0;

    virtual void drop_node(Node* n) = 0;

    virtual void drop_edge_between(Node* n1, Node* n2) = 0;
};

}
#endif //LATTICE_LATTICE_H
