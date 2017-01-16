#ifndef CONNECT_H_INCLUDED
#define CONNECT_H_INCLUDED

#include "base.h"
#include "math_function.h"

#include "layer_common.h"
#include "input_layer.h"
#include "conv_layer.h"
#include "pooling_layer.h"
#include "fully_connected_layer.h"
#include "output_layer.h"

void connect(layer &prev_layer, layer &next_layer);

void connect(input_layer &prev_layer, layer &next_layer);

void connect(conv_layer &prev_layer, layer &next_layer);

void connect(pooling_layer &prev_layer, layer &next_layer);

void connect(fully_connected_layer &prev_layer, layer &next_layer);

void connect(layer &, layer &)
{
    // do nothing
    throw call_error("unknown layer type", "connect function");
}

void connect(input_layer &prev_layer, layer &next_layer)
{
    shape3d_t layer_size = prev_layer.get_size();
    if(get<0>(layer_size) * get<1>(layer_size) * get<2>(layer_size)
            != prev_layer.get_node_size())
    {
        throw call_error("the node size is not fit to the layer_size", "connect function");
    }

    vec_node_t &next_layer_nodes = next_layer.get_nodes();
    vec_edge_t &connecting_edges = prev_layer.get_edges();

    int Count = prev_layer.get_acceptance();

    for(int i = 0; i < Count; ++i)
    {
        ptr_node_t node_ptr(new node());
        ptr_edge_t edge_ptr(new edge({prev_layer.get_nodes()[i]}, node_ptr, {1}));
        next_layer_nodes.push_back(node_ptr);
        connecting_edges.push_back(edge_ptr);
        node_ptr->add_prev_edge(edge_ptr);
    }

    next_layer.get_size() = prev_layer.get_size();
}

void connect(conv_layer &prev_layer, layer &next_layer)
{
    shape3d_t layer_size = prev_layer.get_size();
    if(get<0>(layer_size) * get<1>(layer_size) * get<2>(layer_size)
            != prev_layer.get_node_size())
    {
        throw call_error("the node size is not fit to the layer_size", "connect function");
    }


    if(get<0>(prev_layer.get_kernel_size()) == 0)
    {
        throw kernel_error("connection error! the number of kernel is ZERO!", kernel());
    }

    if(get<1>(prev_layer.get_kernel_size()) % 2 == 0 ||
       get<2>(prev_layer.get_kernel_size()) % 2 == 0)
    {
        throw kernel_error("the lenth of the kernel couldn't be even!", kernel());
    }

    // Expand the picture with zero pixel that we can use the kelnel to scan it

    ptr_node_t zero_node(new node());
    zero_node->activate(0);

    vector<vector<vector<ptr_node_t> > > node_map;

    int depth = get<0>(prev_layer.get_size());
    int height = get<1>(prev_layer.get_size());
    int width = get<2>(prev_layer.get_size());

    int height_addition = get<0>(prev_layer.get_kernel_size()) / 2;
    int width_addition = get<1>(prev_layer.get_kernel_size()) / 2;

    node_map.resize(depth);

    for(int depth_index = 0; depth_index < depth; ++ depth_index)
    {
        node_map[depth_index].resize(height + 2 * height_addition);
        for(int height_index = 0; height_index < height + 2 * height_addition; ++ height_index)
        {
            node_map[depth_index][height_index].resize(width + 2 * width_addition);
            for(int width_index = 0; width_index < width + 2 * width_addition; ++ width_index)
            {
                node_map[depth_index][height_index][width_index] = zero_node;
            } // for width_index
        } // for height_index
    } // for depth_index

    int num_kernel = prev_layer.get_kernel().size();

    for(int depth_index = 0; depth_index < depth; ++ depth_index)
    {
        for(int kernel_index = 0; kernel_index < num_kernel; ++ kernel_index)
        tensor_t weights = prev_layer.get_kernel()[depth].details();

        for(int height_index = 0; height_index < height; ++ height_index)
        {
            for(int width_index = 0; width_index < width; ++ width_index)
            {
                {
                    ptr_node_t next_node(new node());
                    vec_node_t prev_nodes;
                    for(int displacement_x = -height_addition; displacement_x <= height_addition; ++ displacement_x)
                    {
                        for(int displacement_y = -width_addition; displacement_y <= width_addition; ++ displacement_y)
                        {
                            prev_nodes.push_back(node_map[depth][height_index + displacement_x][width_index + displacement_y]);
                    } // for displacement_y
                } // for displacement_x
                ptr_edge_t connecting_edge = new edge(prev_nodes, next_node, weights);
                prev_layer.get_edges().push_back(connecting_edge);
                next_layer.get_nodes().push_back(next_node);
            } // for width_index

        } // for height_index

    } // for depth_index
}

#endif // CONNECT_H_INCLUDED
