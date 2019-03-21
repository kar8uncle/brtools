#ifndef BRTOOLS_ERROR_INTEGRITY_ERROR_H
#define BRTOOLS_ERROR_INTEGRITY_ERROR_H
#pragma once

#include <system_error> // for system_error

namespace brtools
{
namespace error
{
    /**
     * A system error that indicates that some file contains unexpected data.
     *
     * For example, unexpected section header size, magic, section count etc.
     */
    class integrity_error : public std::system_error
    {
    public:
        /**
         * Indicates whether the API should suppress throwing this error.
         */
        static bool suppressed;

    public:
        integrity_error(const std::string& what);
    };
}
}
#endif
