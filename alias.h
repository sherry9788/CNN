#ifndef ALIAS_H_INCLUDED
#define ALIAS_H_INCLUDED

#include "stdlib.h"

class node;
class edge;

using shape2d_t = std::pair<int, int>;
using shape3d_t = std::tuple<int, int, int>;

using edge_ptr_t = std::shared_ptr<edge>;
using node_ptr_t = std::shared_ptr<node>;

using tensor_t = std::vector<std::vector<double> >;

#endif // ALIAS_H_INCLUDED
