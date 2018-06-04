#include "stream_parser.h"
#include <stdexcept>        // for invalid_argument, logic_error
#include <algorithm>        // for reverse

using namespace brtools::io::detail;
using namespace std;

stream_parser::stream_parser(istream& file_in)
: _m_stream_in(file_in)
, _m_should_reverse_bytes(false)
, _m_offset_bases({ 0x0 })  // default offset base is beginning of file
{
}

void stream_parser::reverse_byte_order()
{
    _m_should_reverse_bytes = !_m_should_reverse_bytes;
}

unique_ptr<const char[]> stream_parser::read(const size_t number_of_bytes)
{
    if (number_of_bytes == 0)
    {
        throw invalid_argument(
                "Attempting to read 0 byte from file, which is not allowed."
              );
    }

    auto ret = make_unique<char[]>(number_of_bytes);
    if (!_m_stream_in.read(reinterpret_cast<istream::char_type*>(ret.get()), number_of_bytes))
    {
        throw ios_base::failure("An error occurred while reading from the file.");
    }

    if (_m_should_reverse_bytes)
    {
        reverse(&ret[0], &ret[number_of_bytes]);
    }

    return ret;
}

streampos stream_parser::tell() const
{
    return _m_stream_in.tellg();
}

void stream_parser::seek(const streampos absolute_pos)
{
    _m_stream_in.seekg(absolute_pos);
}

void stream_parser::push_offset_base(const streampos absolute_pos)
{
    _m_offset_bases.push(absolute_pos);
}

void stream_parser::push_offset_base(const streamoff relative_pos)
{
    push_offset_base(_m_offset_bases.top() + relative_pos);
}

void stream_parser::seek_by_offset_from_base(const streamoff offset)
{
    _m_stream_in.seekg(_m_offset_bases.top() + offset);
}

void stream_parser::pop_offset_base()
{
    if (_m_offset_bases.size() == 1)
    {
        // this is the initial 0x00 offset and should not be popped.
        throw logic_error("No more offset to pop.");
    }

    _m_offset_bases.pop();
}

