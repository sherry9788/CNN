#ifndef OUTPUT_LAYER_H_INCLUDED
#define OUTPUT_LAYER_H_INCLUDED

#include "util/util.h"

#include "layer/layer_common.h"

class output_layer : public layer
{
public:
    output_layer();
    output_layer(int n_window_size);

    void forward_propagate();
    void backward_propagate();

    vector<double> output();

    void reset_window_size();

    void set_diff(const vector<double> &diff);

private:

    int m_window_size = 0;
    vector<double> m_diff;
};

void output_layer::reset_window_size()
{
    m_window_size = get<0>(m_size) * get<1>(m_size) * get<2>(m_size);
}

output_layer::output_layer():
    layer(layer_type::able_to_backpropagate,
          OUTPUT_LAYER_TAG)
{
    // do nothing
}

void output_layer::forward_propagate()
{
    // do nothing
}

void output_layer::backward_propagate()
{
    if(m_diff.size() != m_node.size())
    {
        throw size_error("bp error");
    }

    for(int index = 0, Count = m_diff.size(); index < Count; ++index)
    {
        ptr_node_t &curr_node = m_node[index];
        curr_node->get_delta() =
                curr_node->get_op().derivative(curr_node->get_input())
                 * m_diff[index];
    }

    m_prev_layer->backward_propagate();
}

std::vector<double> output_layer::output()
{
    std::vector<double> ret(m_window_size);
    for(int i = 0; i < m_window_size; ++i)
    {
        ret[i] = m_node[i]->get_activation();
    }
    return ret;
}

output_layer::output_layer(int n_window_size):
    layer(layer_type::able_to_backpropagate, OUTPUT_LAYER_TAG),
    m_window_size(n_window_size)
    {
        m_node.resize(n_window_size);
        for(auto &c : m_node)
        {
            c = ptr_node_t(new node());
        }
        get<0>(m_size) = get<1>(m_size) = 1;
        get<2>(m_size) = n_window_size;
    }

void output_layer::set_diff(const vector<double> &diff)
{
    m_diff = diff;
}

#endif // OUTPUT_LAYER_H_INCLUDED
