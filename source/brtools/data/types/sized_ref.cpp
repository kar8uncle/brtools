#include "sized_ref.h"
#include <brtools/io/stream_parser.h>

using namespace brtools::io;
using namespace brtools::data::types;
using namespace std;

stream_parser& brtools::data::types::operator>>(stream_parser& sp, sized_ref& ref)
{
    ref._m_offset = sp.read<uint32_t>();
    return sp >> ref._m_length;
}

uint32_t sized_ref::length() const
{
    return _m_length;
}

streamoff sized_ref::offset() const
{
    return _m_offset;
}
