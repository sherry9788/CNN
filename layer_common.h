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

    std::vector<node_ptr_t> &get_nodes();
    std::vector<edge_ptr_t> &get_edges();

protected:

    std::vector<node_ptr_t> m_node;
    std::vector<edge_ptr_t> m_edge;
    layer_type m_layer_type;

};

template <typename ITER>
void init_edge(ITER iter_beg, ITER iter_end,
               std::vector<edge_ptr_t> &m_edge,
               std::output_iterator_tag)
{
    // do nothing
}

template <typename ITER, typename ITER_TYPE>
void init_edge(ITER iter_beg, ITER iter_end,
               std::vector<edge_ptr_t> &m_edge,
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

std::vector<node_ptr_t> &layer::get_nodes()
{
    return m_node;
}

std::vector<edge_ptr_t> &layer::get_edges()
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
}

void layer::forward_propagade()
{
    // do nothing
}

#endif // LAYER_COMMON_H_INCLUDED
