#ifndef INPUT_LAYER_H_INCLUDED
#define INPUT_LAYER_H_INCLUDED

#include "layer_common.h"
#include "edge.h"

class input_layer : public layer
{
public:

    void forward_propagade();
    void backward_propagade();

    void input_data(const std::vector<double> &n_input);
    int get_acceptance() const;

    input_layer(int n_acceptance);

private:

    int m_acceptance;

};

void input_layer::input_data(const std::vector<double> &n_input)
{
    // the number of input elements is wrong
    assert(get_node_size() == (int)n_input.size());

    for(int i = 0, Count = n_input.size(); i < Count; ++i)
    {
        m_node[i]->activate(n_input[i]);
    }
}

void input_layer::forward_propagade()
{
    for(auto &c : m_edge)
    {
        c->activate();
    }
}

void input_layer::backward_propagade()
{
    // do nothing
}

input_layer::input_layer(int n_acceptance):
    layer(layer_type::not_able_to_backpropagade),
    m_acceptance(n_acceptance)
    {
        m_node.resize(n_acceptance);
    }

int input_layer::get_acceptance() const
{
    return m_acceptance;
}

#endif // INPUT_LAYER_H_INCLUDED
