#ifndef BRTOOLS_DATA_TYPES_VARIABLE_H
#define BRTOOLS_DATA_TYPES_VARIABLE_H
#pragma once

#include <cstdint>

namespace brtools
{
namespace io
{
    class stream_parser;
}

namespace data
{
namespace types
{
    struct variable
    {
        // wait for C++20 and use std::span
        static int16_t* memory;

        uint8_t slot() const;
        
        operator int16_t() const;
        
        const variable& operator=  (int16_t new_value) const;

        const variable& operator+= (int16_t operand) const;
        const variable& operator-= (int16_t operand) const;
        const variable& operator*= (int16_t operand) const;
        const variable& operator/= (int16_t operand) const;
        const variable& operator%= (int16_t operand) const;
        const variable& operator|= (int16_t operand) const;
        const variable& operator&= (int16_t operand) const;
        const variable& operator^= (int16_t operand) const;
        const variable& operator<<=(int16_t operand) const;
        const variable& operator~  (               ) const;

    private:
        uint8_t _m_slot;

        friend 
        io::stream_parser& operator>>(io::stream_parser& parser, variable& value);
    };
}
}
}

#endif
