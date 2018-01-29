#ifndef BRTOOLS_IO_DETAIL_INTEGRITY_ERROR
#define BRTOOLS_IO_DETAIL_INTEGRITY_ERROR
#pragma once

#include <stdexcept>        // for runtime_error

namespace brtools
{
namespace io
{
namespace detail
{
    class integrity_error : public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
    };
}
}
}
#endif
