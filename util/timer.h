// Keep time of trianing process

#ifndef TIMER
#define TIMER

#include "util/util.h"

struct timer
{
    timer()
    {
        m_time = clock();
    }

    int difference()
    {
        return clock() - m_time;
    }

    void reset()
    {
        m_time = clock();
    }

private:

    int m_time;

};

#endif // TIMER

