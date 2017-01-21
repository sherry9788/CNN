#ifndef SAVER
#define SAVER

#include "util/util.h"

#include "layer/layer_common.h"
#include "layer/input_layer.h"
#include "layer/conv_layer.h"
#include "layer/pooling_layer.h"
#include "layer/fully_connected_layer.h"
#include "layer/output_layer.h"

void *save_(input_layer *ptr_layer, fstream &file_out);
void *save_(conv_layer *ptr_layer, fstream &file_out);
void *save_(pooling_layer *ptr_layer, fstream &file_out);
void *save_(fully_connected_layer *ptr_layer, fstream &file_out);
void *save_(output_layer *ptr_layer, fstream &file_out);

void save_to_file(void *begin, void *end, string file_name = "neuron_data")
{
    std::fstream file_out(file_name, std::ios::out | std::ios::binary);
    bool is_end = false;
    while(!is_end)
    {
        if(begin == end)
        {
            is_end = true;
        }

        layer *ptr_layer = (layer *)begin;

        switch (ptr_layer->m_tag)
        {
        case INPUT_LAYER_TAG:
            begin = save_((input_layer *)begin, file_out);
            break;
        case CONV_LAYER_TAG:
            begin = save_((conv_layer *)begin, file_out);
            break;
        case POOLING_LAYER_TAG:
            begin = save_((pooling_layer *)begin, file_out);
            break;
        case FULLY_CONNECTED_LAYER_TAG:
            begin = save_((fully_connected_layer *)begin, file_out);
            break;
        case OUTPUT_LAYER_TAG:
            begin = save_((output_layer *)begin, file_out);
            break;
        }
    }
}

void *save_(input_layer *ptr_layer, fstream &file_out)
{
    shape3d_t size = ptr_layer->get_size();

    UINT32 *window_size = new UINT32[3];

    window_size[0] = INPUT_LAYER_TAG;
    window_size[1] = get<1>(size);
    window_size[2] = get<2>(size);

    file_out.write((char *)window_size, sizeof(UINT32) * 3);
    delete []window_size;

    return ptr_layer->m_next_layer;
}

void *save_(conv_layer *ptr_layer, fstream &file_out)
{
    UINT32 *kernel_size = new UINT32[4];

    kernel_size[0] = CONV_LAYER_TAG;
    kernel_size[1] = get<0>(ptr_layer->get_kernel_size());
    kernel_size[2] = get<1>(ptr_layer->get_kernel_size());
    kernel_size[3] = get<2>(ptr_layer->get_kernel_size());

    double *kernel_data = new double[kernel_size[1] *
            kernel_size[2] * kernel_size[3]];

    connecting_type *n_connecting_type =
            new connecting_type(ptr_layer->get_connecting_type());

    for(UINT32 kernel_index = 0; kernel_index < kernel_size[1]; ++ kernel_index)
    {
        for(UINT32 kernel_x = 0; kernel_x < kernel_size[2]; ++ kernel_x)
        {
            for(UINT32 kernel_y = 0; kernel_y < kernel_size[3]; ++ kernel_y)
            {
                kernel_data[kernel_index * kernel_size[2] * kernel_size[3] +
                        kernel_x * kernel_size[3] + kernel_y] =
                        *ptr_layer->get_kernel()[kernel_index].at(kernel_x, kernel_y);
            } // for kernel_y
        } // for kernel_x
    } // for kernel_size

    file_out.write((char *)kernel_size, sizeof(UINT32) * 4);
    file_out.write((char *)n_connecting_type, sizeof(connecting_type));
    file_out.write((char *)kernel_data, sizeof(double) *
                   kernel_size[1] * kernel_size[2] * kernel_size[3]);

    delete []kernel_size;
    delete []kernel_data;
    delete n_connecting_type;

    return ptr_layer->m_next_layer;
}

void *save_(pooling_layer *ptr_layer, fstream &file_out)
{
    UINT32 *window_size  = new UINT32[3];
    window_size[0] = POOLING_LAYER_TAG;
    window_size[1] = get<0>(ptr_layer->get_area());
    window_size[2] = get<1>(ptr_layer->get_area());

    file_out.write((char *)window_size, sizeof(UINT32) * 3);
    delete []window_size;

    return ptr_layer->m_next_layer;
}

void *save_(fully_connected_layer *ptr_layer, fstream &file_out)
{
    UINT32 *node_size = new UINT32[3];

    node_size[0] = FULLY_CONNECTED_LAYER_TAG;
    node_size[1] = get<2>(ptr_layer->get_size()) + 1;
    node_size[2] = get<2>(ptr_layer->m_next_layer->get_size());

    int Count_x = node_size[1];
    int Count_y = node_size[2];

    double *weight_data = new double[Count_x * Count_y];

    for(int x = 0; x < Count_x; ++ x)
    {
        for(int y = 0; y < Count_y; ++y)
        {
            weight_data[x * Count_y + y] =
                    *ptr_layer->get_edges()[y]->get_weights()[x];
        } // for y
    } // for x

    file_out.write((char *)node_size, sizeof(UINT32) * 3);
    file_out.write((char *)weight_data, sizeof(double) * Count_x * Count_y);
    delete []weight_data;
    delete []node_size;
    return ptr_layer->m_next_layer;
}

void *save_(output_layer *ptr_layer, fstream &file_out)
{
    UINT32 *acceptance = new UINT32[2];
    acceptance[0] = OUTPUT_LAYER_TAG;
    acceptance[1] = get<2>(ptr_layer->get_size());

    file_out.write((char *)acceptance, sizeof(UINT32) * 2);
    delete []acceptance;

    return ptr_layer->m_next_layer;
}

#endif // SAVER

