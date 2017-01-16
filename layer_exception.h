#ifndef LAYER_EXCEPTION
#define LAYER_EXCEPTION

#include "base.h"
#include "kernel.h"

class call_error : public std::logic_error
{
public:

    call_error(string info, string class_name):
        std::logic_error(info)
    {
        m_class_name = class_name;
        cerr << "You are trying to call a wrong function" << endl;
    }

    string get_class_name() const
    {
        return m_class_name;
    }

private:

    string m_class_name;

};

class kernel_error : public std::logic_error
{
public:

    kernel_error(string info, kernel wrong_kernel):
        std::logic_error(info)
    {
        m_kernel = wrong_kernel;
    }

private:

    kernel m_kernel;
};

class size_error : public std::logic_error
{
public:

    size_error(string info) :
        logic_error(info)
    {
        // do nothing
    }

};

#endif // LAYER_EXCEPTION
