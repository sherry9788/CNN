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

private:

    std::vector<std::vector<double> > m_data;
    shape2d_t m_area;

};

#endif // KERNEL_H_INCLUDED
