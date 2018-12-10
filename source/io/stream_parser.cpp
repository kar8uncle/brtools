#include "stream_parser.h"
#include <stdexcept>        // for invalid_argument, logic_error
#include <algorithm>        // for reverse

using namespace brtools::io;
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

stream_parser::offset_scope::offset_scope(stack<streampos>& offset_bases)
: _m_offset_bases(offset_bases)
{}

stream_parser::offset_scope::~offset_scope()
{
    _m_offset_bases.pop();
}

unique_ptr<stream_parser::offset_scope> stream_parser::push_offset_base(const streampos absolute_pos)
{
    _m_offset_bases.push(absolute_pos);
    return make_unique<offset_scope>(_m_offset_bases);
}

unique_ptr<stream_parser::offset_scope> stream_parser::push_offset_base(const streamoff relative_pos)
{
    return push_offset_base(_m_offset_bases.top() + relative_pos);
}

void stream_parser::seek_by_offset_from_base(const streamoff offset)
{
    _m_stream_in.seekg(_m_offset_bases.top() + offset);
}
