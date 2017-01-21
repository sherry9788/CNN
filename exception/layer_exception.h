#ifndef LAYER_EXCEPTION
#define LAYER_EXCEPTION

#include "util/dependence.h"

class kernel;

class call_error : public std::logic_error
{
public:

    call_error(std::string info, std::string class_name):
        std::logic_error(info)
    {
        m_class_name = class_name;
        std::cerr << "You are trying to call a wrong function" << std::endl;
    }

    std::string get_class_name() const
    {
        return m_class_name;
    }

private:

    std::string m_class_name;

};

class kernel_error : public std::logic_error
{
public:

    kernel_error(std::string info, kernel *wrong_kernel):
        std::logic_error(info)
    {
        m_kernel = wrong_kernel;
    }

private:

    kernel *m_kernel;
};

class size_error : public std::logic_error
{
public:

    size_error(std::string info) :
        logic_error(info)
    {
        // do nothing
    }

};

class edge_error : public std::logic_error
{
public:

    edge_error(std::string info):
        std::logic_error(info)
    {
        // do nothing
    }
};

#endif // LAYER_EXCEPTION
