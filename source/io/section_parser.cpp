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
    const streamoff section_offset = _m_sp.read<uint32_t>();
    const auto section_length_in_header = _m_sp.read<uint32_t>();

    _m_seek_to_on_destruct = _m_sp.tell();

    // jump to start of section
    _m_sp.seek_by_offset_from_base(section_offset);

    // populates the section magic
    generate(_m_magic.begin(), _m_magic.end(), bind(&stream_parser::read<char>, &sp));

    // verify section size
    integrity_expect("section length", _m_length, _m_sp.read<uint32_t>());

    _m_sp.push_offset_base(_m_sp.tell());
}

section_parser::~section_parser()
{
    _m_sp.pop_offset_base();
    _m_sp.seek(_m_seek_to_on_destruct);
}

string section_parser::section_magic() const
{
    return _m_magic;
}
