#ifndef INPUT_LAYER_H_INCLUDED
#define INPUT_LAYER_H_INCLUDED

#include "util/util.h"

#include "layer/layer_common.h"
#include "edge/edge.h"

class input_layer : public layer
{
public:

    void forward_propagate();
    void backward_propagate();

    void input_data(const std::vector<double> &n_input);
    int get_acceptance() const;

    input_layer(int n_acceptance);
    input_layer(shape2d_t window);

private:

    int m_acceptance;

};

input_layer::input_layer(shape2d_t window):
    layer(layer_type::not_able_to_backpropagate,
          INPUT_LAYER_TAG)
{
    m_acceptance = get<0>(window) * get<1>(window);
    m_size = shape3d_t(1, get<0>(window), get<1>(window));
    for(int i = 0; i < m_acceptance; ++i)
    {
        m_node.push_back(ptr_node_t(new node(nonlinear_function_type::linear)));
    }
}

void input_layer::input_data(const std::vector<double> &n_input)
{
    // the number of input elements is wrong
    assert(get_acceptance() == (int)n_input.size());

    for(int i = 0, Count = n_input.size(); i < Count; ++i)
    {
        m_node[i]->activate(n_input[i]);
    }

    forward_propagate();
}

void input_layer::forward_propagate()
{
    for(auto &c : m_edge)
    {
        c->activate();
    }

    m_next_layer->forward_propagate();
}

void input_layer::backward_propagate()
{
    // do nothing
}

input_layer::input_layer(int n_acceptance):
    layer(layer_type::not_able_to_backpropagate, INPUT_LAYER_TAG),
    m_acceptance(n_acceptance)
    {
        for(int i = 0; i < n_acceptance; ++i)
        {
            m_node.push_back(ptr_node_t(new node(nonlinear_function_type::linear)));
        }
    }

int input_layer::get_acceptance() const
{
    return m_acceptance;
}

#endif // INPUT_LAYER_H_INCLUDED
