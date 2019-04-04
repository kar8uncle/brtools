#ifndef BRTOOLS_DATA_TYPES_SIZED_REF_H
#define BRTOOLS_DATA_TYPES_SIZED_REF_H
#pragma once

#include <cstdint>  // uint32_t
#include <iosfwd>   // std::streamoff

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
    struct sized_ref
    {
        uint32_t       length() const;
        std::streamoff offset() const;
        
        friend io::stream_parser& operator>>(io::stream_parser&, sized_ref&);

    private:
        std::streamoff _m_offset;
        uint32_t       _m_length;
    };
}
}
}
#endif
