#ifndef EDGE_H
#define EDGE_H

#include "base.h"

#include "node.h"

class edge
{
public:

    edge() = default;

    void init(const vec_node_t &n_prev_nodes,
         ptr_node_t n_next_node,
         vec_double_t n_weights,
         ptr_edge_t ptr_this,
         bool add_info_to_node = true
         );

    void activate();
    
    ptr_node_t get_next_node() const;
    vec_node_t get_prev_node() const;    

    vec_double_t &get_weights();

private:

    vec_node_t m_prev_nodes;
    ptr_node_t m_next_node;

    vec_double_t m_weights;

};

void edge::activate()
{
    double n_output(0);
    for(int i = 0, Count = m_prev_nodes.size(); i < Count ; ++i )
    {
        n_output += m_prev_nodes[i]->get_activation() * *m_weights[i];
    }

    m_next_node->activate(n_output);
}

void edge::init(const vec_node_t &n_prev_nodes,
           ptr_node_t n_next_node,
           vec_double_t n_weights,
           ptr_edge_t ptr_this,
           bool add_info_to_node)
           {
               m_prev_nodes = n_prev_nodes;
               m_next_node = n_next_node;
               m_weights = n_weights;
               if(n_weights.size() != n_prev_nodes.size())
               {
                   throw edge_error("the weights size is not fit to nodes size!");
               }

               if(!add_info_to_node)
               {
                   return;
               }

               for(auto &c : n_prev_nodes)
               {
                   c->add_next_edge(ptr_this);
               }

               n_next_node->add_prev_edge(ptr_this);
           }


ptr_node_t edge::get_next_node() const
{
    return this->m_next_node;
}

vec_node_t edge::get_prev_node() const
{
    return this->m_prev_nodes;
}

vec_double_t &edge::get_weights()
{
    return m_weights;
}

#endif // EDGE_H
