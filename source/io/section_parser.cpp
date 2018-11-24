#include "section_parser.h"
#include <error/integrity_error.h>
#include <cinttypes>    // for uint32_t
#include <algorithm>    // for generate

using namespace brtools::io;
using namespace brtools::error;
using namespace std;

namespace
{
    void check_section_length(stream_parser& fp, const uint32_t expected_length)
    {
        const auto section_length_in_section = fp.read<uint32_t>();
        if (!integrity_error::suppressed)
        if (expected_length != section_length_in_section)
        {
            throw integrity_error(
                     "Section length specified in file header is " + to_string(expected_length) + ", "
                     "but section length specified in section is " + to_string(section_length_in_section) + "."
                  );
        }
    }
}

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
    check_section_length(_m_sp, section_length_in_header);

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
