#ifndef NEURON_LOADER
#define NEURON_LOADER

#include "util/util.h"

#include "layer/layer_common.h"
#include "layer/input_layer.h"
#include "layer/conv_layer.h"
#include "layer/pooling_layer.h"
#include "layer/fully_connected_layer.h"
#include "layer/output_layer.h"

#include "node/connect.h"

void init_conv_layer(conv_layer *n_layer, pair<int, char*> data);

void init_fully_connected_layer(fully_connected_layer *n_layer, pair<int, char*> data);

void load_from_file(void **begin, void **end, string file_path = "neuron_data")
{
    map<int, pair<int, char*> > index_to_data;

    vector<layer *> ptr_layer;

    fstream file_in(file_path, std::ios::in | std::ios::binary);

    UINT32 choice;
    bool is_end = false;
    while(!is_end)
    {
        file_in.read((char *)(&choice), sizeof(UINT32));
        if(choice == INPUT_LAYER_TAG)
        {
            UINT32 window_x, window_y;
            file_in.read((char *)(&window_x), sizeof(UINT32));
            file_in.read((char *)(&window_y), sizeof(UINT32));
            input_layer *new_input_layer = new input_layer(shape2d_t(window_x, window_y));
            ptr_layer.push_back(new_input_layer);
        }
        else if(choice == CONV_LAYER_TAG)
        {
            int kernel_size, kernel_height, kernel_width;
            file_in.read((char *)(&kernel_size), sizeof(UINT32));
            file_in.read((char *)(&kernel_height), sizeof(UINT32));
            file_in.read((char *)(&kernel_width), sizeof(UINT32));
            connecting_type n_connecting_type;
            file_in.read((char *)(&n_connecting_type), sizeof(connecting_type));
            conv_layer *new_conv_layer = new conv_layer(shape2d_t(kernel_height, kernel_width));
            new_conv_layer->get_connecting_type() = n_connecting_type;
            for(int Count = 0; Count < kernel_size; ++ Count)
            {
                new_conv_layer->add_kernel(kernel(kernel_height, kernel_width, all_one));
            }
            ptr_layer.push_back(new_conv_layer);
            int data_length = sizeof(double) * kernel_size *
                    kernel_width * kernel_height;
            char *kernel_data = new char[data_length];
            file_in.read(kernel_data, data_length);
            index_to_data.insert(std::make_pair(ptr_layer.size() - 1,
                                 std::make_pair(data_length, kernel_data)));
        }
        else if(choice == POOLING_LAYER_TAG)
        {
            UINT32 window_height, window_width;
            file_in.read((char *)(&window_height), sizeof(UINT32));
            file_in.read((char *)(&window_width), sizeof(UINT32));
            pooling_layer * new_pooling_layer =
                    new pooling_layer(shape2d_t(window_height, window_width));
            ptr_layer.push_back(new_pooling_layer);
        }
        else if(choice == FULLY_CONNECTED_LAYER_TAG)
        {
            UINT32 acceptance, next_layer_acceptance;
            file_in.read((char *)(&acceptance), sizeof(UINT32));
            --acceptance;
            fully_connected_layer *new_fully_connected_layer =
                    new fully_connected_layer(acceptance);
            ptr_layer.push_back(new_fully_connected_layer);
            file_in.read((char *)(&next_layer_acceptance), sizeof(UINT32));
            UINT32 data_size = (acceptance + 1) * next_layer_acceptance;
            char *weight_data = new char[sizeof(double) * data_size];
            file_in.read(weight_data, data_size * sizeof(double));
            index_to_data.insert(std::make_pair(ptr_layer.size() - 1,
                                 std::make_pair(data_size * sizeof(double), weight_data )));
        }
        else if(choice == OUTPUT_LAYER_TAG)
        {
            UINT32 output_window;
            file_in.read((char *)(&output_window), sizeof(UINT32));
            output_layer *new_output_layer = new output_layer(output_window);
            ptr_layer.push_back(new_output_layer);
            is_end = true;
        }
        else
        {
            throw file_error("exception in loading file!");
        }
    }

    for(UINT32 i = 1; i < ptr_layer.size(); ++i)
    {
        switch(ptr_layer[i - 1]->m_tag)
        {
        case INPUT_LAYER_TAG:
            connect(*(input_layer *)ptr_layer[i -1], *ptr_layer[i]);
            break;
        case CONV_LAYER_TAG:
            connect(*(conv_layer *)ptr_layer[i -1], *ptr_layer[i]);
            break;
        case POOLING_LAYER_TAG:
            connect(*(pooling_layer *)ptr_layer[i -1], *ptr_layer[i]);
            break;
        case FULLY_CONNECTED_LAYER_TAG:
            connect(*(fully_connected_layer *)ptr_layer[i -1], *ptr_layer[i]);
            break;
        default:
            break;
            // do nothing
        }
    }

    for(auto uninit_layer : index_to_data)
    {
        if(ptr_layer[uninit_layer.first]->m_tag == CONV_LAYER_TAG)
        {
            init_conv_layer((conv_layer *)ptr_layer[uninit_layer.first], uninit_layer.second);
        }
        else
        {
            init_fully_connected_layer((fully_connected_layer *)ptr_layer[uninit_layer.first], uninit_layer.second);
        }

    }
    *begin = ptr_layer[0];
    *end = ptr_layer[ptr_layer.size() - 1];
}

void init_conv_layer(conv_layer *n_layer, pair<int, char*> data)
{
    double *kernel_data = (double *)data.second;
    int kernel_size = get<0>(n_layer->get_kernel_size());
    int kernel_height = get<1>(n_layer->get_kernel_size());
    int kernel_width = get<2>(n_layer->get_kernel_size());
    int area = kernel_height * kernel_width;
    for(int kernel_index = 0; kernel_index < kernel_size; ++ kernel_index)
    {
        tensor_t curr_kernel = n_layer->get_kernel()[kernel_index].details();
        for(int height_index = 0; height_index < kernel_height; ++ height_index)
        {
            for(int width_index = 0; width_index < kernel_width; ++ width_index)
            {
                *curr_kernel[height_index][width_index] =
                        kernel_data[area * kernel_index +
                        height_index * kernel_width + width_index];
            } // for width_index
        } // for height_index
    } // for kernel_index
    delete []kernel_data;
}

void init_fully_connected_layer(fully_connected_layer *n_layer, pair<int, char*> data)
{
    double *weight_data = (double*)data.second;
    int Count_x = get<2>(n_layer->get_size());
    int Count_y = get<2>(n_layer->m_next_layer->get_size());
    for(int x = 0; x < Count_x + 1; ++x)
    {
        for(int y = 0; y < Count_y; ++y)
        {
            *n_layer->get_edges()[y]->get_weights()[x] =
                    weight_data[x * Count_y + y];
        } // for y
    } // for x
    delete []weight_data;
}

#endif // NEURON_LOADER

