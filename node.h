#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include "base.h"

class node
{
public:

    node() = default;
    node(std::vector<edge_ptr_t> n_prev_edges,
         std::vector<edge_ptr_t> n_next_edges
         );
    template <typename ITER1, typename ITER2>
    node(ITER1 prev_edges_beg, ITER1 prev_edges_end,
            ITER2 next_edges_beg, ITER2 next_edges_end);


    void activate(double n_activate);
    double get_activation() const;

    void add_prev_edge(edge_ptr_t n_prev_edge);
    void add_next_edge(edge_ptr_t n_next_edge);

private:

    std::set<edge_ptr_t> m_prev_edges;
    std::set<edge_ptr_t> m_next_edges;
    double m_activation;
    double m_error;

};

void node::activate(double n_activate)
{
    m_activation = n_activate;
}

double node::get_activation() const
{
    return m_activation;
}

node::node(std::vector<edge_ptr_t> n_prev_edges,
         std::vector<edge_ptr_t> n_next_edges
         ):
             m_prev_edges(n_prev_edges.begin(), n_prev_edges.end()),
             m_next_edges(n_next_edges.begin(), n_next_edges.end())
{
    // do nothing
}

template <typename ITER1, typename ITER2>
node::node(ITER1 prev_edges_beg, ITER1 prev_edges_end,
         ITER2 next_edges_beg, ITER2 next_edges_end
         ):
             m_prev_edges(prev_edges_beg, prev_edges_end),
             m_next_edges(next_edges_beg, prev_edges_end)
{
    // do nothing
}

void node::add_prev_edge(edge_ptr_t n_prev_edge)
{
    m_prev_edges.insert(n_prev_edge);
}

void node::add_next_edge(edge_ptr_t n_next_edge)
{
    m_next_edges.insert(n_next_edge);
}


#endif // NODE_H_INCLUDED
