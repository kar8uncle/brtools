#ifndef BRTOOLS_UTIL_INTEGRITY_EXPECT_H
#define BRTOOLS_UTIL_INTEGRITY_EXPECT_H
#pragma once

#include <brtools/error/integrity_error.h>
#include <sstream>  // for ostringstream

namespace brtools
{
namespace util
{
    template<typename ExpectedType, typename ActualType>
    void integrity_expect(const std::string& quantity_name, const ExpectedType& expected, const ActualType& actual)
    {
        if (!error::integrity_error::suppressed)
        if (expected != actual)
        {
            throw error::integrity_error(static_cast<const std::ostringstream&>(std::ostringstream()
                      << "mismatching " << quantity_name << "," << std::endl
                      << "expected: "   << expected      << ";" << std::endl
                      << "actual: "     << actual        << "."
                  ).str());
        }
    }
}
}

#endif
