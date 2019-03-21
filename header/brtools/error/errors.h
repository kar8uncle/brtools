#ifndef BRTOOLS_ERROR_ERRORS_H
#define BRTOOLS_ERROR_ERRORS_H
#pragma once

#include <system_error>
#include <type_traits>

namespace brtools
{
namespace error
{
    enum class codes
    {
        // 0 is reserved for no error
        integrity_error = 1,
    };

    const std::error_category& brtools_category();
    std::error_code make_error_code(codes e) noexcept;
    std::error_condition make_error_condition(codes e) noexcept;
}
}

namespace std
{
    template<>
    struct is_error_code_enum<brtools::error::codes>
    : true_type
    {};
}
#endif
