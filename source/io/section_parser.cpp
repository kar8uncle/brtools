#include "section_parser.h"
#include "../util/integrity_expect.h"
#include <cstdint>      // for uint32_t
#include <algorithm>    // for generate

using brtools::util::integrity_expect;
using namespace brtools::io;
using namespace std;

section_parser::section_parser(stream_parser& sp, const string::size_type magic_length)
: _m_sp(sp)
, _m_magic(magic_length, '\0')
{
    _m_end_of_section = _m_sp.tell();

    // populates the section magic
    generate(_m_magic.begin(), _m_magic.end(), bind(&stream_parser::read<char>, &sp));

    sp >> _m_length;
    _m_end_of_section += _m_length;

    _m_offset_scope = _m_sp.push_offset_base(_m_sp.tell());
}

section_parser::~section_parser()
{
    _m_sp.seek(_m_end_of_section);
}

string section_parser::section_magic() const
{
    return _m_magic;
}

uint32_t section_parser::section_length() const
{
    return _m_length;
}

bool section_parser::has_data_to_read() const
{
    return _m_sp.tell() < _m_end_of_section;
}
