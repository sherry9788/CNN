#ifndef KERNEL_H_INCLUDED
#define KERNEL_H_INCLUDED

#include "util/util.h"

class kernel
{
public:

    shape2d_t get_area() const;

    double &operator[] (std::pair<int, int> position);

    double operator[] (std::pair<int, int> position) const;

    ptr_double_t &at(int x, int y);

    ptr_double_t at(int x, int y) const;

    tensor_t details() const;

    kernel();

    kernel(tensor_t n_data);

    kernel(int height, int width, double (*random_engine)());

    vec_double_t align() const;

private:

    tensor_t m_data;
    shape2d_t m_area;

};

shape2d_t kernel::get_area() const
{
    return m_area;
}

double &kernel::operator[] (std::pair<int, int> position)
{
    return *m_data[position.first][position.second];
}

double kernel::operator[] (std::pair<int, int> position) const
{
    return *m_data[position.first][position.second];
}

ptr_double_t &kernel::at(int x, int y)
{
    return m_data[x][y];
}

ptr_double_t kernel::at(int x, int y) const
{
    return m_data[x][y];
}

kernel::kernel()
{
    // do nothing
}

kernel::kernel(tensor_t n_data):
    m_data(n_data)
{
    get<0>(m_area) = m_data.size();
    get<1>(m_area) = m_data[0].size();
}

tensor_t kernel::details() const
{
    return m_data;
}

kernel::kernel(int height, int width, double (*random_engine)())
{
    m_area = shape2d_t(height, width);
    m_data.resize(height);
    for(auto &c : m_data)
    {
        c.resize(width);
        for(auto &d : c)
        {
            d = ptr_double_t(new double(random_engine()));
        }
    }
}

vec_double_t kernel::align() const
{
    vec_double_t ret;
    for(auto &row : m_data)
    {
        for(auto &ele : row)
        {
            ret.push_back(ele);
        }
    }
    return ret;
}

#endif // KERNEL_H_INCLUDED
