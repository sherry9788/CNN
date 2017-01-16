#ifndef LAYER_COMMON_H_INCLUDED
#define LAYER_COMMON_H_INCLUDED

#include "base.h"

class layer
{
public:

    virtual void forward_propagade();
    virtual void backward_propagade();
    int get_node_size() const;

    layer() = delete;
    template <typename ITER>

    layer(ITER iter_beg, ITER iter_end, layer_type n_layer_type);

    layer(layer_type n_layer_type);

    vec_node_t &get_nodes();
    vec_edge_t &get_edges();

    shape3d_t &get_size();
    shape3d_t get_size() const;

protected:

    vec_node_t m_node;
    vec_edge_t m_edge;
    layer_type m_layer_type;

    shape3d_t m_size;

};

template <typename ITER>
void init_edge(ITER, ITER,
               vec_edge_t &,
               std::output_iterator_tag)
{
    // do nothing
}

template <typename ITER, typename ITER_TYPE>
void init_edge(ITER iter_beg, ITER iter_end,
               vec_edge_t &m_edge,
               ITER_TYPE)
{
    while(iter_beg != iter_end)
    {
        m_edge.push_back(*iter_beg);
        ++iter_beg;
    }
}



template <typename ITER>
layer::layer(ITER iter_beg, ITER iter_end,
             layer_type n_layer_type):
    m_layer_type(n_layer_type)
{
    init_edge(iter_beg, iter_end, std::iterator_traits<ITER>::iterator_category);
}

int layer::get_node_size() const
{
    return m_node.size();
}

vec_node_t &layer::get_nodes()
{
    return m_node;
}

vec_edge_t &layer::get_edges()
{
    return m_edge;
}

layer::layer(layer_type n_layer_type):
    m_layer_type(n_layer_type)
    {
        // do nothing
    }

void layer::backward_propagade()
{
    // do nothing
    throw call_error("this layer doesn't support bp!", "layer");
}

void layer::forward_propagade()
{
    // do nothing
    throw call_error("this layer doesn't support bp!", "layer");
}

shape3d_t &layer::get_size()
{
    return m_size;
}

shape3d_t layer::get_size() const
{
    return m_size;
}

#endif // LAYER_COMMON_H_INCLUDED
