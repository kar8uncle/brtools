#ifndef BRTOOLS_DATA_TYPES_UINT_H
#define BRTOOLS_DATA_TYPES_UINT_H
#pragma once

#include <cstdint>
#include <iterator>     // std::rbegin, std::rend
#include <climits>      // CHAR_BIT
#include <type_traits>  // std::enable_if_t
#include "../../io/stream_parser.h"

namespace brtools
{
namespace data
{
namespace types
{
    /**
     * Represents an integral type containing BitCount-many bits. This struct
     * is only defined if BitCount is multiples of CHAR_BIT.
     *
     * @tparam BitCount 
     *         The number of bits this integral type has. Must be multiples of
     *         CHAR_BIT, which is typically 8.
     *
     * @tparam StorageType
     *         The primitive type that is able to store this integral type
     *         without data loss.
     *
     * @tparam ByteCount
     *         The number of bytes this integral type has. Equal to
     *         BitCount / CHAR_BIT, if BitCount is divisible by CHAR_BIT.
     *
     * @tparam (unnamed)
     *         SFINAE to ensure StorageType has enough memory to hold a value
     *         of this integral type.
     */
    template<size_t BitCount, 
             typename StorageType = uintmax_t,
             std::enable_if_t<BitCount % CHAR_BIT == 0, size_t> ByteCount = BitCount / CHAR_BIT,
             typename = std::enable_if_t<sizeof(StorageType) >= ByteCount>>
    struct uint
    {
        /**
         * Converts this uint to StorageType.
         */
        operator StorageType() const
        {
            StorageType result = 0;
            for (auto it = std::rbegin(_m_storage); it != std::rend(_m_storage); ++it)
            {
                result <<= CHAR_BIT;
                result |= *it;
            }
            return result;
        }

    private:
        uint8_t _m_storage[ByteCount];

    private:
        /**
         * Specializes stream_parser's stream operator for this uint.
         */
        friend io::stream_parser& operator>>(io::stream_parser& parser, uint<BitCount, StorageType>& value)
        {
            memcpy(value._m_storage, parser.read(ByteCount).get(), ByteCount);
            return parser;
        }
    };
}
}
}
#endif
