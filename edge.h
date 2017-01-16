#ifndef EDGE_H
#define EDGE_H

#include "base.h"

#include "node.h"

class edge
{
public:

    edge(const std::vector<node_ptr_t> &n_prev_nodes,
         node_ptr_t n_next_node,
         std::vector<double> n_weights
         );

    void activate();

private:

    std::vector<node_ptr_t> m_prev_nodes;
    node_ptr_t m_next_node;

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

edge::edge(const std::vector<node_ptr_t> &n_prev_nodes,
           node_ptr_t n_next_node,
           std::vector<double> n_weights):
               m_prev_nodes(n_prev_nodes),
               m_next_node(m_next_node),
               m_weights(n_weights)
               {
                   assert(n_weights.size() == n_prev_nodes.size());
               }

#endif // EDGE_H
