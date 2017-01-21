// define aliases

#ifndef BASE_H_INCLUDED
#define BASE_H_INCLUDED

#include "util/headers.h"

#include "exception/layer_exception.h"
#include "exception/math_exception.h"
#include "exception/file_exception.h"

class node;
class edge;
class kernel;

using shape2d_t = std::tuple<int, int>;
using shape3d_t = std::tuple<int, int, int>;

using ptr_edge_t = std::shared_ptr<edge>;
using ptr_node_t = std::shared_ptr<node>;
using ptr_double_t = std::shared_ptr<double>;

using tensor_t = std::vector<std::vector<std::shared_ptr<double> > >;

using vec_edge_t = std::vector<std::shared_ptr<edge> >;
using vec_node_t = std::vector<std::shared_ptr<node> >;
using vec_double_t = std::vector<std::shared_ptr<double> >;

using std::string;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::set;
using std::get;
using std::fstream;
using std::map;
using std::pair;

#define BIAS -1

#define INPUT_LAYER_TAG     			0x00000100
#define CONV_LAYER_TAG 					0x00000200
#define POOLING_LAYER_TAG   			0x00000300
#define FULLY_CONNECTED_LAYER_TAG       0x00000400
#define OUTPUT_LAYER_TAG 				0x00000500

#define IMAGE_MAGIC_NUMBER 0x00000803
#define LABEL_MAGIC_NUMBER 0x00000801

#define LENGTH 28
#define PIXEL 784

using UINT8 = unsigned char;
using UINT16 = unsigned short;
using UINT32 = unsigned;

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

// will be used in convolutional layers
enum class connecting_type
{
    one_by_one,
    one_to_all
};

#endif // BASE_H_INCLUDED
