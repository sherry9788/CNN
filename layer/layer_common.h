#ifndef LAYER_COMMON_H_INCLUDED
#define LAYER_COMMON_H_INCLUDED

#include "util/util.h"

class layer
{
public:
    friend void save_to_file(void *, void *, string);
    friend void load_from_file(void **, void **, string);

    // will be defined by derived classes
    virtual void forward_propagate();
    virtual void backward_propagate();
    // count the number of nodes
    int get_node_size() const;

    layer() = delete;
    template <typename ITER>

    layer(ITER iter_beg, ITER iter_end, layer_type n_layer_type);

    layer(layer_type n_layer_type, UINT32 layer_tag);

    vec_node_t &get_nodes();
    vec_edge_t &get_edges();

    shape3d_t &get_size();
    shape3d_t get_size() const;

    // linearly align all nodes
    void resize_window();

    layer *m_next_layer = nullptr;
    layer *m_prev_layer = nullptr;

protected:

    vec_node_t m_node;
    vec_edge_t m_edge;
    layer_type m_layer_type;

    shape3d_t m_size;

    UINT32 m_tag;
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

layer::layer(layer_type n_layer_type, UINT32 layer_tag):
    m_layer_type(n_layer_type),
    m_tag(layer_tag)
    {
        // do nothing
    }

void layer::backward_propagate()
{
    // do nothing
    throw call_error("this layer doesn't support bp!", "layer");
}

void layer::forward_propagate()
{
    // do nothing
    throw call_error("this layer doesn't support fp!", "layer");
}

shape3d_t &layer::get_size()
{
    return m_size;
}

shape3d_t layer::get_size() const
{
    return m_size;
}

void layer::resize_window()
{
    int window_size = get<0>(m_size) * get<1>(m_size) * get<2>(m_size);
    get<0>(m_size) = get<1>(m_size) = 1;
    get<2>(m_size) = window_size;
}

#endif // LAYER_COMMON_H_INCLUDED
