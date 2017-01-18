#ifndef FULLY_CONNECTED_LAYER_H_INCLUDED
#define FULLY_CONNECTED_LAYER_H_INCLUDED

#include "base.h"
#include "layer_common.h"
#include "edge.h"

class fully_connected_layer : public layer
{
public:

    fully_connected_layer(int n_acceptance = 0);

    void forward_propagate();
    void backward_propagate();

    static double study_rate;

private:

};

double fully_connected_layer::study_rate = 0.1;

void fully_connected_layer::forward_propagate()
{
    for(auto &c : m_edge)
    {
        c->activate();
    }
    m_next_layer->forward_propagate();
}

void fully_connected_layer::backward_propagate()
{
    int node_size = m_node.size();
    int edge_size = m_edge.size();
    for(int node_index = 0; node_index < node_size; ++ node_index)
    {
        double partial_E_y = 0;
        for(int edge_index = 0; edge_index < edge_size; ++ edge_index)
        {
            partial_E_y += *m_edge[edge_index]->get_weights()[node_index] *
                    m_edge[edge_index]->get_next_node()->get_delta();
        } // for edge_index
        m_node[node_index]->get_delta() = partial_E_y * m_node[node_index]->get_op().derivative(m_node[node_index]->get_input());
    } // for node_index

    m_prev_layer->backward_propagate();


    for(int edge_index = 0; edge_index < edge_size; ++ edge_index)
    {
        double next_layer_delta = m_edge[edge_index]->get_next_node()->get_delta();
        for(int node_index = 0; node_index < node_size; ++ node_index)
        {
            *m_edge[edge_index]->get_weights()[node_index] -=
                    m_node[node_index]->get_activation() *
                    next_layer_delta *
                    study_rate;
        }

        *m_edge[edge_index]->get_weights()[node_size] -=
            BIAS * next_layer_delta * study_rate;
    }
}

fully_connected_layer::fully_connected_layer(int n_acceptance):
    layer(layer_type::able_to_backpropagate)
{
    get<0>(m_size) = get<1>(m_size) = 1;
    get<2>(m_size) = n_acceptance;
    m_node.resize((n_acceptance));
    for(auto &c : m_node)
        c = ptr_node_t(new node());
}

#endif // FULLY_CONNECTED_LAYER_H_INCLUDED
