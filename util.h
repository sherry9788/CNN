#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <iostream>
      
#include "alias.h"
#include "node.h"

enum class layer_type
{
    able_to_backpropagate,
    not_able_to_backpropagate
};

template <typename T>
void debug(T x)
{
    std::cout << "---------------"
        << x
        <<       "---------------"
        << std::endl;
}

enum class connecting_type
{
    one_by_one,
    one_to_all
};

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

#endif // UTIL_H_INCLUDED
