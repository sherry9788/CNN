#ifndef EDGE_H
#define EDGE_H

#include "base.h"

#include "node.h"

class edge
{
public:

    edge(const vec_node_t &n_prev_nodes,
         ptr_node_t n_next_node,
         std::vector<double> n_weights
         );

    void activate();
    
    ptr_node_t get_next_node() const;
    vec_node_t get_prev_node() const;    

private:

    vec_node_t m_prev_nodes;
    ptr_node_t m_next_node;

    std::vector<double> m_weights;

};

void edge::activate()
{
    double n_activate(0);
    for(int i = 0, Count = m_prev_nodes.size(); i < Count ; ++i )
    {
        n_activate += m_prev_nodes[i]->get_activation() * m_weights[i];
    }

    m_next_node->activate(n_activate);
}

edge::edge(const vec_node_t &n_prev_nodes,
           ptr_node_t n_next_node,
           std::vector<double> n_weights):
               m_prev_nodes(n_prev_nodes),
               m_next_node(n_next_node),
               m_weights(n_weights)
               {
                   assert(n_weights.size() == n_prev_nodes.size());
               }


ptr_node_t edge::get_next_node() const
{
    return this->m_next_node;
}

vec_node_t edge::get_prev_node() const
{
    return this->m_prev_nodes;
}

#endif // EDGE_H
