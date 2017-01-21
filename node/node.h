#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include "util/util.h"

#include "math_tools/nonlinear_function.h"

class node
{
public:

    node(vec_edge_t n_prev_edges,
         vec_edge_t n_next_edges,
         nonlinear_function_type nonlinear_ = nonlinear_function_type::sigmoid
         );

    node(double activation);

    node(nonlinear_function_type n_choice = nonlinear_function_type::sigmoid);

    template <typename ITER1, typename ITER2>
    node(ITER1 prev_edges_beg, ITER1 prev_edges_end,
            ITER2 next_edges_beg, ITER2 next_edges_end);

    // non-linear process the signal it receives
    void activate(double n_input);
    // get the output of activation
    double get_activation() const;

    // add an edge to this node
    void add_prev_edge(ptr_edge_t n_prev_edge);
    // add an edge from this node
    void add_next_edge(ptr_edge_t n_next_edge);

    // get the non-linear function
    nonlinear_function &get_op();
    nonlinear_function get_op() const;

    // get "delta", will be used in backward propagation
    double &get_delta();
    double get_delta() const;

    // get the input signal
    double get_input() const;

    // get the edge to this node
    vec_edge_t &get_prev_edge();
    vec_edge_t get_prev_edge() const;

    // get the edge from this node
    vec_edge_t &get_next_edge();
    vec_edge_t get_next_edge() const;

private:

    vec_edge_t m_prev_edges;
    vec_edge_t m_next_edges;

    double m_last_input;
    double m_activation;
    double m_delta;

    nonlinear_function m_nonlinear_function;
};

node::node(nonlinear_function_type n_choice):
    m_nonlinear_function(n_choice)
{
    m_last_input = m_activation = m_delta = 0;
}

void node::activate(double n_input)
{
    m_last_input = n_input;
    m_activation = m_nonlinear_function(n_input);
}

double node::get_activation() const
{
    return m_activation;
}

node::node(vec_edge_t n_prev_edges,
         vec_edge_t n_next_edges,
         nonlinear_function_type nonlinear_
         ):
             m_prev_edges(n_prev_edges),
             m_next_edges(n_next_edges),
             m_nonlinear_function(nonlinear_)
{
    // do nothing
}

template <typename ITER1, typename ITER2>
node::node(ITER1 prev_edges_beg, ITER1 prev_edges_end,
         ITER2 next_edges_beg, ITER2 next_edges_end
         ):
             m_prev_edges(prev_edges_beg, prev_edges_end),
             m_next_edges(next_edges_beg, next_edges_end)
{
    // do nothing
}

void node::add_prev_edge(ptr_edge_t n_prev_edge)
{
    m_prev_edges.push_back(n_prev_edge);
}

void node::add_next_edge(ptr_edge_t n_next_edge)
{
    m_next_edges.push_back(n_next_edge);
}


nonlinear_function &node::get_op()
{
    return m_nonlinear_function;
}

nonlinear_function node::get_op() const
{
    return m_nonlinear_function;
}

double node::get_delta() const
{
    return m_delta;
}

double &node::get_delta()
{
    return m_delta;
}

node::node(double activation)
{
    m_activation = activation;
    m_last_input = activation;
}

double node::get_input() const
{
    return m_last_input;
}

vec_edge_t &node::get_prev_edge()
{
    return m_prev_edges;
}

vec_edge_t node::get_prev_edge() const
{
    return m_prev_edges;
}

vec_edge_t &node::get_next_edge()
{
    return m_next_edges;
}

vec_edge_t node::get_next_edge() const
{
    return m_next_edges;
}

#endif // NODE_H_INCLUDED
