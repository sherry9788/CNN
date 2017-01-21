#ifndef NETWORK_H
#define NETWORK_H

#include "util/util.h"

#include "layer/conv_layer.h"
#include "layer/fully_connected_layer.h"
#include "layer/input_layer.h"
#include "layer/output_layer.h"
#include "layer/pooling_layer.h"

#include "file_op/loader.h"
#include "file_op/saver.h"

#include "math_tools/loss_function.h"

class network
{
public:

    network(string file_path);
    network();

    void input_data(vector<double> input);
    double output_data(vector<double> &output, const vector<double> &answer);
    vector<double> output_data();
    void epoch();

private:

    input_layer *m_input_layer;
    output_layer *m_output_layer;

    vector<double> m_diff;

};

network::network(string file_path)
{
    load_from_file((void **)&m_input_layer, (void **)&m_output_layer, file_path);
    m_diff.resize(10);
}

network::network()
{
    m_input_layer = new input_layer(shape2d_t(28, 28));
    conv_layer *l2 = new conv_layer(shape2d_t(3, 3));
    l2->get_connecting_type() = connecting_type::one_to_all;
    for(int Count = 0; Count < 6; ++ Count)
    {
        l2->add_kernel(kernel(3, 3, random_clamp));
    }
    pooling_layer *l3 = new pooling_layer(shape2d_t(2, 2));
    conv_layer *l4 = new conv_layer(shape2d_t(3, 3));
    //l4->get_connecting_type() = connecting_type::one_by_one;
    for(int Count = 0; Count < 2; ++ Count)
    {
        l4->add_kernel(kernel(3, 3, random_clamp));
    }
    pooling_layer *l5 = new pooling_layer(shape2d_t(2, 2));
    fully_connected_layer *l6 = new fully_connected_layer;
    fully_connected_layer *l7 = new fully_connected_layer(100);
    fully_connected_layer *l8 = new fully_connected_layer(30);
    m_output_layer = new output_layer(10);

    connect(*m_input_layer, *l2);
    connect(*l2, *l3);
    connect(*l3, *l4);
    connect(*l4, *l5);
    connect(*l5, *l6);
    connect(*l6, *l7);
    connect(*l7, *l8);
    connect(*l8, *m_output_layer);

    m_diff.resize(10);
}

void network::input_data(vector<double> input)
{
    m_input_layer->input_data(input);
}

double network::output_data(vector<double> &output, const vector<double> &answer)
{
    output = m_output_layer->output();
    for(int i = 0; i < 10; ++i)
    {
        m_diff[i] = output[i] - answer[i];
    }
    return square_error(output, answer);
}

vector<double> network::output_data()
{
    return m_output_layer->output();
}

void network::epoch()
{
    m_output_layer->set_diff(m_diff);
    m_output_layer->backward_propagate();
}

#endif // NETWORK_H
