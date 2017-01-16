#ifndef ALIAS_H_INCLUDED
#define ALIAS_H_INCLUDED

#include "lib.h"

class node;
class edge;
class kernel;

using shape2d_t = std::tuple<int, int>;
using shape3d_t = std::tuple<int, int, int>;

using ptr_edge_t = std::shared_ptr<edge>;
using ptr_node_t = std::shared_ptr<node>;

using tensor_t = std::vector<std::vector<double> >;

using vec_edge_t = std::vector<std::shared_ptr<edge> >;
using vec_node_t = std::vector<std::shared_ptr<node> >;

using std::string;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::set;
using std::get;

#endif // ALIAS_H_INCLUDED
