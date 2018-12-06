#ifndef BRTOOLS_DATA_TYPES_VARINT_H
#define BRTOOLS_DATA_TYPES_VARINT_H
#pragma once

#include <cstdint>
#include <io/stream_parser.h>

namespace brtools
{
namespace data
{
namespace types
{
    /**
     * Represents variable-length unsigned integer.
     */
    template<typename StorageType>
    struct basic_varint
    {
        operator StorageType() const 
        {   return _m_storage; }

    private:
        StorageType _m_storage;

        /**
         * Specializes stream_parser's stream operator to parse a variable-length
         * unsigned integer.
         */
        friend io::stream_parser& operator>>(io::stream_parser& parser,
                                             basic_varint<StorageType>& value)
        {
            uint8_t byte;
            do {
                parser >> byte;
                value._m_storage <<= 7;
                value._m_storage |= byte & 0b0111'1111;
            } while (byte >> 7);

            return parser;
        }
    };

    using varint = basic_varint<uintmax_t>;
}
}
}
#endif
