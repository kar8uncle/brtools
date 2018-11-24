#include "file_parser.h"
#include <cinttypes>    // for uint32_t
#include <algorithm>    // for generate

using namespace brtools::io;
using namespace std;

file_parser::file_parser(stream_parser& sp, const string::size_type magic_length)
: _m_sp(sp)
, _m_magic(magic_length, '\0')
{
    const auto start_of_file = _m_sp.tell();
    _m_sp.push_offset_base(start_of_file);

    // populates the file magic
    generate(_m_magic.begin(), _m_magic.end(), bind(&stream_parser::read<char>, &sp));

    // checks byte order mark, reverses byte order if needed
    if (0xFEFF != _m_sp.read<uint16_t>())
    {
        _m_reverse_bytes_on_destruct = true;
        _m_sp.reverse_byte_order();
    }

    _m_sp >> _m_version_number;
    _m_sp >> _m_size;
    _m_sp >> _m_header_size;
}

file_parser::~file_parser()
{
    _m_sp.seek_by_offset_from_base(_m_size);
    _m_sp.pop_offset_base();
    if (_m_reverse_bytes_on_destruct)
    {
        _m_sp.reverse_byte_order();
    }
}

string file_parser::file_magic() const
{
    return _m_magic;
}

file_parser::version_num_type file_parser::file_version() const
{
    return _m_version_number;
}

file_parser::header_size_type file_parser::file_header_size() const
{
    return _m_header_size;
}

file_parser::file_size_type file_parser::file_size() const
{
    return _m_size;
}
