#include <data/types/variable.h>
#include <io/stream_parser.h>

using namespace brtools::data::types;
using brtools::io::stream_parser;

int16_t* variable::memory = nullptr;

uint8_t variable::slot() const
{
    return _m_slot;
}

variable::operator int16_t() const
{
    return memory ? memory[_m_slot] : 0;
}

const variable& variable::operator=(const int16_t new_value) const
{
    if (memory)
    {
        memory[_m_slot] = new_value;
    }

    return *this;
}

const variable& variable::operator+=(const int16_t operand) const
{
    return *this = *this + operand;
}

const variable& variable::operator-=(const int16_t operand) const
{
    return *this = *this - operand;
}

const variable& variable::operator*=(const int16_t operand) const
{
    return *this = *this * operand;
}

const variable& variable::operator/=(const int16_t operand) const
{
    return *this = *this / operand;
}

const variable& variable::operator%=(const int16_t operand) const
{
    return *this = *this % operand;
}

const variable& variable::operator|=(const int16_t operand) const
{
    return *this = *this | operand;
}

const variable& variable::operator&=(const int16_t operand) const
{
    return *this = *this & operand;
}

const variable& variable::operator^=(const int16_t operand) const
{
    return *this = *this ^ operand;
}

const variable& variable::operator<<=(const int16_t operand) const
{
    return *this = *this << operand;
}

const variable& variable::operator~() const
{
    return *this = ~*this;
}

stream_parser& brtools::data::types::operator>>(stream_parser& parser, variable& value)
{
    return parser >> value._m_slot;
}
