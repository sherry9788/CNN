#ifndef POOLING_LAYER_H_INCLUDED
#define POOLING_LAYER_H_INCLUDED

#include "base.h"

#include "layer_common.h"
#include "edge.h"
#include "pooling_operator.h"

class pooling_layer : public layer
{
public:

    pooling_layer() = delete;
    pooling_layer(shape2d_t n_window);

    void backward_propagate();
    void forward_propagate();

    void reset_op(pooling::op *new_op);

    shape2d_t get_area() const;

private:

    shape2d_t m_area;
    pooling::op *op = nullptr;

};

pooling_layer::pooling_layer(shape2d_t n_window):
    layer(layer_type::able_to_backpropagate),
    m_area(n_window)
{
    int window_size = get<0>(m_area) * get<1>(m_area);
    if(window_size == 4)
        op = new pooling::get_max<4>;
    else if (window_size == 9)
        op = new pooling::get_max<9>;
    else
        throw call_error("wrong pooling function!", "pooling_layer");
}

void pooling_layer::backward_propagate()
{
    for(auto &curr_node : m_node)
    {
        curr_node->get_delta() = 0;
    }

    for(auto &curr_edge : m_edge)
    {
        curr_edge->get_prev_node()[0]->get_delta() =
                curr_edge->get_next_node()->get_delta();
    }

    m_prev_layer->backward_propagate();
}

void pooling_layer::forward_propagate()
{
    m_edge.clear();
    vec_node_t &next_nodes = m_next_layer->get_nodes();
    if(get<0>(m_area) == 2)
    {
        for(int depth_index = 0, depth = get<0>(m_size); depth_index < depth; ++ depth_index)
        {
            for(int height_index = 0, height = get<1>(m_size); height_index < height/2; ++ height_index)
            {
                for(int width_index = 0, width = get<2>(m_size); width_index < width/2; ++ width_index)
                {
                    vec_node_t nodes_in_window;
                    for(int window_x = 0; window_x < 2; ++ window_x)
                    {
                        for(int window_y = 0; window_y < 2; ++ window_y)
                        {
                            nodes_in_window.push_back(
                                m_node[depth_index * height * width + (height_index * 2 + window_x) * width
                                + width_index * 2 + window_y]
                                );
                        } // for window_y
                    } // for window_x
                    vector<double> activations;
                    for(auto &c : nodes_in_window)
                    {
                        activations.push_back(c->get_activation());
                    }
                    int position;
                    (*op)(activations, position);
                    ptr_edge_t new_edge(new edge());
                    new_edge->init({nodes_in_window[position]},
                                   next_nodes[depth_index * height * width / 4 + height_index * width / 2 + width_index],
                                   {ptr_double_t(new double(1))},
                                   new_edge,
                                   false
                                  );
                    m_edge.push_back(new_edge);
                } // for width_index
            } // for height_index
        } // for depth_index
    }
    else
    {
        for(int depth_index = 0, depth = get<0>(m_size); depth_index < depth; ++ depth_index)
        {
            for(int height_index = 0, height = get<1>(m_size); height_index < height/3; ++ height_index)
            {
                for(int width_index = 0, width = get<2>(m_size); width_index < width/3; ++ width_index)
                {
                    vec_node_t nodes_in_window;
                    for(int window_x = 0; window_x < 3; ++ window_x)
                    {
                        for(int window_y = 0; window_y < 3; ++ window_y)
                        {
                            nodes_in_window.push_back(
                                m_node[depth_index * height * width + (height_index * 3 + window_x) * width
                                + width_index * 3 + window_y]
                                );
                        } // for window_y
                    } // for window_x
                    vector<double> activations;
                    for(auto &c : nodes_in_window)
                    {
                        activations.push_back(c->get_activation());
                    }
                    int position;
                    (*op)(activations, position);
                    ptr_edge_t new_edge(new edge());
                    new_edge->init({nodes_in_window[position]},
                                   next_nodes[depth_index * height * width / 9 + height_index * width / 3 + width_index],
                                   {ptr_double_t(new double(1))},
                                   new_edge,
                                   false
                                  );
                } // for width_index
            } // for height_index
        } // for depth_index
    }
    for(auto &c : m_edge)
    {
        c->activate();
    }
    m_next_layer->forward_propagate();
}

void pooling_layer::reset_op(pooling::op *new_op)
{
    op = new_op;
}

shape2d_t pooling_layer::get_area() const
{
    return m_area;
}

#endif // POOLING_LAYER_H_INCLUDED
