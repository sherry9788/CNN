#ifndef FULLY_CONNECTED_LAYER_H_INCLUDED
#define FULLY_CONNECTED_LAYER_H_INCLUDED

#include "base.h"
#include "layer_common.h"
#include "edge.h"

class fully_connected_layer : public layer
{
public:

    void forward_propagade();
    void backward_propagade();

private:

    // nothing

};


void fully_connected_layer::forward_propagade()
{
    for(auto &c : m_edge)
    {
        c->activate();
    }
}

void fully_connected_layer::backward_propagade()
{
    // do nothing
}

#endif // FULLY_CONNECTED_LAYER_H_INCLUDED
