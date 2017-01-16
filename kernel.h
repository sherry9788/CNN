#ifndef KERNEL_H_INCLUDED
#define KERNEL_H_INCLUDED

#include "base.h"

class kernel
{
public:

    shape2d_t get_area() const
    {
        return m_area;
    }

    double operator[] (std::pair<int, int> position)
    {
        return m_data[position.first][position.second];
    }

    tensor_t details() const;

    kernel();

    kernel(tensor_t n_data);

private:

    tensor_t m_data;
    shape2d_t m_area;

};

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

#endif // KERNEL_H_INCLUDED
