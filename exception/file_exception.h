#ifndef FILE_ERROR
#define FILE_ERROR

#include "util/dependence.h"

class file_error : public std::logic_error
{
public:

    file_error(std::string info) :
        std::logic_error(info)
    {
        std::cerr << "exception in file operation!" << std::endl;
    }

private:

    // nothing
};

class file_inexist : public std::logic_error
{
public:

    file_inexist(std::string info):
        std::logic_error(info)
    {
        // do nothing
    }
};

#endif // FILE_ERROR

