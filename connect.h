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

#include "random_function.h"

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
    next_layer_nodes.clear();
    vec_edge_t &connecting_edges = prev_layer.get_edges();

    int Count = prev_layer.get_acceptance();

    for(int i = 0; i < Count; ++i)
    {
        ptr_node_t node_ptr(new node(nonlinear_function_type::linear));
        ptr_edge_t edge_ptr(new edge());
        edge_ptr->init({prev_layer.get_nodes()[i]}, node_ptr, {ptr_double_t(new double(1))}, edge_ptr);
        next_layer_nodes.push_back(node_ptr);
        connecting_edges.push_back(edge_ptr);
    }

    prev_layer.m_next_layer = &next_layer;
    next_layer.m_prev_layer = &prev_layer;
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

    vector<vector<vector<ptr_node_t> > > &node_map = prev_layer.get_this_node_map();

    int depth = get<0>(prev_layer.get_size());
    int height = get<1>(prev_layer.get_size());
    int width = get<2>(prev_layer.get_size());

    int height_addition = get<1>(prev_layer.get_kernel_size()) / 2;
    int width_addition = get<2>(prev_layer.get_kernel_size()) / 2;

    node_map = expand_map(prev_layer.get_nodes(), depth, height, width, height_addition, width_addition);

    int num_kernel = get<0>(prev_layer.get_kernel_size());

    if(prev_layer.get_connecting_type() == connecting_type::one_to_all)
    {
    //////////////////////// if the connecting type is one to all

    for(int kernel_index = 0; kernel_index < num_kernel; ++ kernel_index)
    {
        for(int depth_index = 0; depth_index < depth; ++ depth_index)
        {
            vec_double_t weights = prev_layer.get_kernel()[kernel_index].align();

            for(int height_index = height_addition; height_index < height + height_addition; ++ height_index)
            {
                for(int width_index = width_addition; width_index < width + width_addition; ++ width_index)
                {
                    ptr_node_t next_node(new node(nonlinear_function_type::ReLU));
                    vec_node_t prev_nodes;
                    for(int displacement_x = -height_addition; displacement_x <= height_addition; ++ displacement_x)
                    {
                        for(int displacement_y = -width_addition; displacement_y <= width_addition; ++ displacement_y)
                        {
                            prev_nodes.push_back(node_map[depth_index][height_index + displacement_x][width_index + displacement_y]);
                        }// for displacement_y
                    } // for displacement_x
                    ptr_edge_t connecting_edge(new edge());
                    connecting_edge->init(prev_nodes, next_node, weights, connecting_edge);
                    prev_layer.get_edges().push_back(connecting_edge);
                    next_layer.get_nodes().push_back(next_node);
                } // for width_index
            } // for height_index
        } // for depth_index
    } // for kernel_index

    next_layer.get_size() = prev_layer.get_size();
    get<0>(next_layer.get_size()) *= get<0>(prev_layer.get_kernel_size());

    }
    else
    {
    //////////////////////////// if the connecting type is one by one

    if(num_kernel != depth)
    {
        throw size_error("the size of kernel is not equal to the number"
                         " of subpictures!");
    }

    for(int depth_index = 0; depth_index < depth; ++ depth_index)
    {
        vec_double_t weights = prev_layer.get_kernel()[depth_index].align();

        for(int height_index = height_addition; height_index < height + height_addition; ++ height_index)
        {
            for(int width_index = width_addition; width_index < width + width_addition; ++ width_index)
            {
                ptr_node_t next_node(new node(nonlinear_function_type::ReLU));
                vec_node_t prev_nodes;
                for(int displacement_x = -height_addition; displacement_x <= height_addition; ++ displacement_x)
                {
                    for(int displacement_y = -width_addition; displacement_y <= width_addition; ++ displacement_y)
                    {
                        prev_nodes.push_back(node_map[depth_index][height_index + displacement_x][width_index + displacement_y]);
                    }// for displacement_y
                } // for displacement_x
                ptr_edge_t connecting_edge(new edge());
                connecting_edge->init(prev_nodes, next_node, weights, connecting_edge);
                prev_layer.get_edges().push_back(connecting_edge);
                next_layer.get_nodes().push_back(next_node);
            } // for width_index

        } // for height_index
    } // for depth_index



    next_layer.get_size() = prev_layer.get_size();
    }

    prev_layer.get_next_node_map() =
            expand_map(next_layer.get_nodes(),
                       get<0>(next_layer.get_size()),
                       get<1>(next_layer.get_size()),
                       get<2>(next_layer.get_size()),
                       height_addition,
                       width_addition);

    next_layer.m_prev_layer = &prev_layer;
    prev_layer.m_next_layer = &next_layer;
}

void connect(pooling_layer &prev_layer, layer &next_layer)
{
    int window_length = get<0>(prev_layer.get_area());
    next_layer.get_size() = shape3d_t(
            get<0>(prev_layer.get_size()),
            get<1>(prev_layer.get_size()) / window_length,
            get<2>(prev_layer.get_size()) / window_length
    );

    vec_node_t &next_nodes = next_layer.get_nodes();

    int node_size = get<0>(prev_layer.get_size()) *
            get<1>(prev_layer.get_size()) * get<2>(prev_layer.get_size()) / 4;

    for(int Count = 0; Count < node_size; ++ Count)
    {
        next_nodes.push_back(ptr_node_t(new node(nonlinear_function_type::linear)));
    }

    next_layer.m_prev_layer = &prev_layer;
    prev_layer.m_next_layer = &next_layer;
}

void connect(fully_connected_layer &prev_layer, layer &next_layer)
{
    prev_layer.resize_window();
    next_layer.resize_window();
    if(get<2>(next_layer.get_size()) == 0)
    {
        throw size_error("the number of nodes of next layer is ZERO!");
    }
    vec_node_t prev_nodes = prev_layer.get_nodes();

    srand(time(nullptr));

    int prev_nodes_size = get<2>(prev_layer.get_size());

    ptr_node_t const_node(new node(BIAS));
    prev_nodes.push_back(const_node);
    ++prev_nodes_size;

    for(auto &c : next_layer.get_nodes())
    {
        c->get_op() = nonlinear_function(nonlinear_function_type::sigmoid);
    }

    for(auto &c : next_layer.get_nodes())
    {
        ptr_edge_t connecting_edge(new edge());
        connecting_edge->init(prev_nodes, c, random_vector(prev_nodes_size, random_clamp), connecting_edge);
        prev_layer.get_edges().push_back(connecting_edge);
    }

    prev_layer.m_next_layer = &next_layer;
    next_layer.m_prev_layer = &prev_layer;
}

#endif // CONNECT_H_INCLUDED
