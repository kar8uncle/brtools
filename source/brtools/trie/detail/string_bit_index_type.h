#ifndef BRTOOLS_TRIE_DETAIL_STRING_BIT_INDEX_TYPE_H
#define BRTOOLS_TRIE_DETAIL_STRING_BIT_INDEX_TYPE_H

#include <limits>
#include <cstdint>  // uint16_t
#include <string>

namespace brtools
{
namespace trie
{
namespace detail
{
    /**
     * Similar to bit fields provided in the language, but behavior is more
     * portable and well-suited to br usecase.
     *
     * @tparam _BitCount The number of bits this bit field contains.
     * @tparam _BitShift The number of bits to right shift away before the
     *                   value of this bit field starts in raw value.
     * @tparam _RawValueType The type of the underlying value that holds
     *                       the bits. Expects an unsigned integral type.
     */
    template<size_t _BitCount, size_t _BitShift, typename _RawValueType>
    class bit_field
    {
    public: // aliasi of the template parameters
        static constexpr size_t value_size  = _BitCount;
        static constexpr size_t value_start = _BitShift;
        static constexpr size_t value_end   = value_start + value_size;

    public: // masks
        static constexpr size_t bits_in_raw_value = std::numeric_limits<_RawValueType>::digits;

        /**
         * Mask to keep the least significant first _BitCount-bits of a value.
         * For example, if _BitCount is 3, then truncate_mask is 0b0...00111.
         */
        static constexpr _RawValueType truncate_mask = static_cast<_RawValueType>(~0u) >> (bits_in_raw_value - _BitCount);

        /**
         * Mask to clear the previously set bits in the raw value, in order to
         * update the bit field.
         * For example, if _BitCount is 3, _BitShift is 5, then clear_mask is
         * 0b1...1'000'11111.
         */
        static constexpr _RawValueType clear_mask = ~(truncate_mask << _BitShift);

    public:
        explicit bit_field(_RawValueType& raw) : _m_raw(raw) {}

        /**
         * Assigns a new value to this bit field.
         */
        bit_field& operator=(const _RawValueType& val)
        {
            _m_raw = (_m_raw & clear_mask) | ((val & truncate_mask) << _BitShift);
            return *this;
        }

        /**
         * Contextually converts to _RawValueType. The actual value of this 
         * bit field. 
         */
        operator _RawValueType() const
        {   return (_m_raw >> _BitShift) & truncate_mask;   }

    private:
        _RawValueType& _m_raw;
    };

    /**
     * Addresses a bit in a string. Following the string indexing conventions,
     * the leftmost bit is defined to be index 0.
     *
     * @tparam _IndexType The underlying type that stores the index.
     */
    template<typename _IndexType>
    struct string_bit_index_type
    {
    public:
        using index_type = _IndexType;

    private:
        /**
         * 3 bits of space used to address bits in an 8-bit character.
         */
        static constexpr size_t bit_index_type_size  = 3;

        /**
         * The remaining bits available in _IndexType to address characters
         * in a string.
         */
        static constexpr size_t char_index_type_size = std::numeric_limits<index_type>::digits - bit_index_type_size;
        
        using bit_index_type  = bit_field< bit_index_type_size, 0,                         index_type>;
        using char_index_type = bit_field<char_index_type_size, bit_index_type::value_end, index_type>;

    public:
        enum : index_type
        { max = static_cast<index_type>(~0u) };

        /**
         * The bit index in the string.
         */
        index_type      index;

        /**
         * The bit index in the character specified by char_index. Value
         * range is 0-7.
         */
        bit_index_type  bit_index;

        /**
         * The character index in the string.
         */
        char_index_type char_index;

    public: // constructors
        string_bit_index_type(const index_type idx = max)
        : index(idx)
        , bit_index(index)
        , char_index(index)
        {}

        string_bit_index_type(const string_bit_index_type& other)
        : index(other.index)
        , bit_index(index)
        , char_index(index)
        {}

    public: // size_t-like operators
        /* Comparison */
        bool operator<(const string_bit_index_type& other) const
        {   return index <  other.index;    }

        bool operator<=(const string_bit_index_type& other) const
        {   return index <= other.index;    }

        bool operator>(const string_bit_index_type& other) const
        {   return index >  other.index;    }

        bool operator>=(const string_bit_index_type& other) const
        {   return index >= other.index;    }

        /* Equality */
        bool operator==(const string_bit_index_type& other) const
        {   return index == other.index;   }

        bool operator!=(const string_bit_index_type& other) const
        {   return index != other.index;   }

        /* Assignment */
        string_bit_index_type& operator=(const string_bit_index_type& other)
        {   return index = other.index, *this;   }

        /* Max value is max. Remains as max if overflows. */
        string_bit_index_type operator+(const string_bit_index_type& other) const
        {   return string_bit_index_type(*this) += other;   }

        string_bit_index_type& operator+=(const string_bit_index_type& other)
        {   return index = max - index <= other.index ? max : index + other.index, *this;   }

        string_bit_index_type& operator++()
        {   return index = max - index <= 1 ? max : index + 1, *this;   }

        string_bit_index_type operator++(int)
        {   return string_bit_index_type(index++);   }

        /* Min value is 0. Remains as 0 if underflows. */
        string_bit_index_type operator-(const string_bit_index_type& other) const
        {   return string_bit_index_type(*this) -= other;   }

        string_bit_index_type& operator-=(const string_bit_index_type& other)
        {   return index = index < other.index ? 0 : index - other.index, *this;   }

        string_bit_index_type& operator--()
        {   return index = index < 1 ? 0 : index - 1, *this;   }

        string_bit_index_type operator--(int)
        {   return string_bit_index_type(index--);   }

        /**
         * Note: Due to limitations of the language, the syntax is weird.
         *       The current usage is index[string].
         *
         * @returns The bit of str at the specified index, or 0 if index is
         *          past the end of the given string.
         */
        bool operator[](const std::string& str) const
        {
            if (char_index >= str.length())
            {
                // treat the str as if there were a stream of NUL past the end
                return 0;
            }
            else
            {
                return (str[char_index] >> ( 7 - bit_index )) & 1;
            }
        }
    };
}
}
}
#endif
