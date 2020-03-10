/* Copyright 2020, Sergey Popov (me@sergobot.me) */

#ifndef LATTICE_LATTICE_H
#define LATTICE_LATTICE_H

class Lattice {
public:
    virtual void drop_node() = 0;

    virtual void drop_edge() = 0;
};


#endif //LATTICE_LATTICE_H
