#ifndef BRTOOLS_DATA_TYPES_RANDOM_H
#define BRTOOLS_DATA_TYPES_RANDOM_H
#pragma once

#include <cstdint>
#include <brtools/io/stream_parser.h>

namespace brtools
{
namespace data
{
namespace types
{
    /**
     * Represents a random number.
     */
    template<typename StorageType>
    struct random
    {
        StorageType lower_bound() const
        {   return _m_lower_bound; }

        StorageType upper_bound() const
        {   return _m_upper_bound; }

        operator StorageType() const
        {   return generate(_m_lower_bound, _m_upper_bound); }

    private:
        StorageType _m_lower_bound;
        StorageType _m_upper_bound;
        
        static StorageType generate(StorageType lower, StorageType upper);

        friend io::stream_parser& operator>>(io::stream_parser& parser,
                                             random<StorageType>& value)
        {
            return parser >> value._m_lower_bound >> value._m_upper_bound;
        }
    };

    using s_rand = random<int16_t>;
    using u_rand = random<uint16_t>;
}
}
}
#endif
