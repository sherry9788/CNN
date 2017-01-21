#ifndef CONV_LAYER_H_INCLUDED
#define CONV_LAYER_H_INCLUDED

#include "util/util.h"

#include "edge/kernel.h"
#include "edge/edge.h"

#include "math_tools/math_function.h"

#include "layer/layer_common.h"

#include "math_tools/math_function.h"

vector<vector<vector<ptr_node_t> >  >
expand_map(const vec_node_t &prev_vec_node, int depth,
           int height, int width, int height_addition, int width_addition);

class conv_layer : public layer
{
public:

    conv_layer() = delete;

    conv_layer(shape2d_t window_size);

    void forward_propagate();
    void backward_propagate();

    void add_kernel(kernel n_kernel);

    shape3d_t get_kernel_size() const;

    vector<kernel> get_kernel() const;

    connecting_type get_connecting_type() const;
    connecting_type &get_connecting_type();

    vector<vector<vector<ptr_node_t> > > &get_this_node_map();
    vector<vector<vector<ptr_node_t> > > get_this_node_map() const;

    vector<vector<vector<ptr_node_t> > > &get_next_node_map();
    vector<vector<vector<ptr_node_t> > > get_next_node_map() const;

    static double m_study_rate;

private:

    // Kernel is the scan window and will be used in the initiation
    // of the network
    vector<kernel> m_kernel;
    shape3d_t m_kernel_size;

    vector<vector<vector<ptr_node_t> > > m_this_node_map;
    vector<vector<vector<ptr_node_t> > > m_next_node_map;

    connecting_type m_connecting_type = connecting_type::one_to_all;

};

double conv_layer::m_study_rate = 0.01;

void conv_layer::add_kernel(kernel n_kernel)
{
    if(get<0>(n_kernel.get_area()) != get<1>(m_kernel_size) ||
       get<1>(n_kernel.get_area()) != get<2>(m_kernel_size))
    {
        throw kernel_error("the size is not fit to this layer", &n_kernel);
    }
    ++get<0>(m_kernel_size);
    m_kernel.push_back(n_kernel);
}

void conv_layer::forward_propagate()
{
    for(auto &c : m_edge)
    {
        c->activate();
    }
    m_next_layer->forward_propagate();
}

void conv_layer::backward_propagate()
{
    int depth = get<0>(m_size);
    int height = get<1>(m_size);
    int width = get<2>(m_size);

    int kernel_size = get<0>(m_kernel_size);
    int kernel_height = get<1>(m_kernel_size);
    int kernel_width = get<2>(m_kernel_size);

    int center_x = kernel_height / 2;
    int center_y = kernel_width / 2;

    if(m_connecting_type == connecting_type::one_by_one)
    {
    ///////////////////////// if the connecting type is one by one

    for(int depth_index = 0; depth_index < depth; ++ depth_index)
    {
        tensor_t curr_kernel = m_kernel[depth_index].details();
        for(int height_index = 0; height_index < height; ++ height_index)
        {
            for(int width_index = 0; width_index < width; ++ width_index)
            {
                double partial_E_y = 0;
                for(int kernel_x = 0; kernel_x < kernel_height; ++ kernel_x)
                {
                    for(int kernel_y = 0; kernel_y < kernel_width; ++ kernel_y)
                    {
                        partial_E_y +=
                                *curr_kernel[kernel_x][kernel_y] *
                                m_next_node_map[depth_index]
                                [center_x - kernel_x + kernel_height/2 + height_index]
                                [center_y - kernel_y + kernel_width/2 + width_index]
                                ->get_delta();
                    } // for kernel_x
                } // for_kernel_y

                ptr_node_t curr_node = m_this_node_map[depth_index][height_index + kernel_height/2][width_index + kernel_width/2];
                curr_node->get_delta() = curr_node->get_op().derivative(curr_node->get_input()) * partial_E_y;
            } // for width_index
        } // for height_index
    } // for_depth_index

    m_prev_layer->backward_propagate();

    for(int depth_index = 0; depth_index < depth; ++ depth_index)
    {
        tensor_t curr_kernel = m_kernel[depth_index].details();
        for(int kernel_x = 0; kernel_x < kernel_height; ++ kernel_x)
        {
            for(int kernel_y = 0; kernel_y < kernel_width; ++ kernel_y)
            {
                double weight_change = 0;
                for(int height_index = 0; height_index < height; ++ height_index)
                {
                    for(int width_index = 0; width_index < width; ++ width_index)
                    {
                        weight_change += m_next_node_map[depth_index][height_index][width_index]->get_delta() *
                                m_this_node_map[depth_index][height_index + kernel_x][width_index + kernel_x]->get_activation();
                    } // for width_index
                } // for height_index
                *curr_kernel[kernel_x][kernel_y] -= m_study_rate * weight_change;
            } // for kernel_y
        } // for kernel_x
    } // for depth_index

    }
    else
    {
    /////////////////////////////////////// if the connecting type is one to all

    for(int depth_index = 0; depth_index < depth; ++ depth_index)
    {
        for(int height_index = 0; height_index < height; ++ height_index)
        {
            for(int width_index = 0; width_index < width; ++ width_index)
            {
                double partial_E_y = 0;
                for(int kernel_index = 0; kernel_index < kernel_size; ++ kernel_index)
                {
                    tensor_t curr_kernel = m_kernel[kernel_index].details();
                    for(int kernel_x = 0; kernel_x < kernel_height; ++ kernel_x)
                    {
                        for(int kernel_y = 0; kernel_y < kernel_width; ++ kernel_y)
                        {
                            partial_E_y +=
                                    *curr_kernel[kernel_x][kernel_y] *
                                    m_next_node_map[kernel_index * depth + depth_index]
                                    [center_x - kernel_x + kernel_height/2 + height_index]
                                    [center_y - kernel_y + kernel_width/2 + width_index]
                                    ->get_delta();
                        } // for kernel_x
                    } // for kernel_y
                } // for kernel_index

                ptr_node_t curr_node = m_this_node_map[depth_index][height_index + kernel_height/2][width_index + kernel_width/2];
                curr_node->get_delta() = curr_node->get_op().derivative(curr_node->get_input()) * partial_E_y;
            } // for width_index
        } // for height_index
    } // for_depth_index

    m_prev_layer->backward_propagate();

    for(int kernel_index = 0; kernel_index < kernel_size; ++ kernel_index)
    {
        tensor_t curr_kernel = m_kernel[kernel_index].details();
        {
            for(int kernel_x = 0; kernel_x < kernel_height; ++ kernel_x)
            {
                for(int kernel_y = 0; kernel_y < kernel_width; ++ kernel_y)
                {
                    double weight_change = 0;
                    for(int depth_index = 0; depth_index < depth; ++ depth_index)
                    {
                        for(int height_index = 0; height_index < height; ++ height_index)
                        {
                            for(int width_index = 0; width_index < width; ++ width_index)
                            {
                                weight_change += m_next_node_map[depth_index + kernel_index * depth]
                                        [height_index]
                                        [width_index]->get_delta() *
                                        m_this_node_map[depth_index]
                                        [height_index + kernel_x]
                                        [width_index + kernel_x]->get_activation();
                            } // for width_index
                        } // for height_index

                    } // for depth_index
                    *curr_kernel[kernel_x][kernel_y] -= m_study_rate * weight_change;
                } // for kernel_y
            } // for kernel_x
        } // for kernel_index
    } // for kernel_index

    }

}

conv_layer::conv_layer(shape2d_t window_size):
    layer(layer_type::able_to_backpropagate,
          CONV_LAYER_TAG)
{
    get<0>(m_kernel_size) = 0;
    get<1>(m_kernel_size) = get<0>(window_size);
    get<2>(m_kernel_size) = get<1>(window_size);
}

shape3d_t conv_layer::get_kernel_size() const
{
    return m_kernel_size;
}

vector<kernel> conv_layer::get_kernel() const
{
    return m_kernel;
}

connecting_type conv_layer::get_connecting_type() const
{
    return m_connecting_type;
}

connecting_type &conv_layer::get_connecting_type()
{
    return m_connecting_type;
}

vector<vector<vector<ptr_node_t> > > &conv_layer::get_this_node_map()
{
    return m_this_node_map;
}

vector<vector<vector<ptr_node_t> > > conv_layer::get_this_node_map() const
{
    return m_this_node_map;
}

vector<vector<vector<ptr_node_t> > > &conv_layer::get_next_node_map()
{
    return m_next_node_map;
}

vector<vector<vector<ptr_node_t> > > conv_layer::get_next_node_map() const
{
    return m_next_node_map;
}

vector<vector<vector<ptr_node_t> >  >
expand_map(const vec_node_t &prev_vec_node, int depth,
           int height, int width, int height_addition, int width_addition)
{
    ptr_node_t zero_node(new node(nonlinear_function_type::linear));
    vector<vector<vector<ptr_node_t> > > ret;
    ret.resize(depth);
    for(int depth_index = 0; depth_index < depth; ++ depth_index)
    {
        ret[depth_index].resize(height + 2 * height_addition);
        for(int height_index = 0; height_index < height_addition * 2 + height; ++ height_index)
        {
            ret[depth_index][height_index] = vec_node_t(width + 2 * width_addition, zero_node);
        }
    }

    for(int depth_index = 0; depth_index < depth; ++ depth_index)
    {
        for(int height_index = 0; height_index < height; ++ height_index)
        {
            for(int width_index = 0; width_index < width; ++ width_index)
            {
                ret[depth_index][height_index + height_addition][width_addition + width_index] =
                        prev_vec_node[depth_index * height * width + height_index * width + width_index];
            } // for width_index
        } // for height_index
    } // for depth_index

    return ret;
}


#endif // CONV_LAYER_H_INCLUDED
