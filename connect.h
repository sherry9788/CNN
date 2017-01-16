#ifndef CONNECT_H_INCLUDED
#define CONNECT_H_INCLUDED

#include "base.h"

#include "layer_common.h"
#include "input_layer.h"
#include "conv_layer.h"
#include "pooling_layer.h"
#include "fully_connected_layer.h"
#include "output_layer.h"

void connect(input_layer prev_layer, layer next_layer);

void connect(conv_layer prev_layer, layer next_layer);

void connect(pooling_layer prev_layer, layer next_layer);

void connect(fully_connected_layer prev_layer, layer next_layer);

void connect(output_layer prev_layer, layer next_layer);

void connect(input_layer prev_layer, layer next_layer)
{
    std::vector<node_ptr_t> &next_layer_nodes = next_layer.get_nodes();
    std::vector<edge_ptr_t> &connecting_edges = prev_layer.get_edges();

    int Count = prev_layer.get_acceptance();

    for(int i = 0; i < Count; ++i)
    {
        node_ptr_t node_ptr(new node());
        edge_ptr_t edge_ptr(new edge({prev_layer.get_nodes()[i]}, node_ptr, {1}));
        next_layer_nodes.push_back(node_ptr);
        connecting_edges.push_back(edge_ptr);
        node_ptr->add_prev_edge(edge_ptr);
    }
}

#endif // CONNECT_H_INCLUDED
