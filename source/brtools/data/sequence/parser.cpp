#include "parser.h"
#include <brtools/util/integrity_expect.h>
#include <algorithm>    // generate

using brtools::util::integrity_expect;
using namespace brtools::data::sequence;
using namespace brtools::io;
using namespace std;

parser::parser(stream_parser& sp)
: file_parser(sp)
{
    integrity_expect("file magic", "RSEQ", file_magic());
    integrity_expect("header size", 0x20, file_header_size());
    integrity_expect("section count", 2, sp.read<uint16_t>());
    sp >> _m_data_section_ref
       >> _m_labl_section_ref;
}

parser::data_section_parser parser::data()
{
    _m_sp.seek_by_offset_from_base(_m_data_section_ref.offset());
    return data_section_parser(_m_sp, _m_data_section_ref.length());
}

parser::labl_section_parser parser::labl()
{
    _m_sp.seek_by_offset_from_base(_m_labl_section_ref.offset());
    return labl_section_parser(_m_sp, _m_labl_section_ref.length());
}

parser::data_section_parser::data_section_parser(stream_parser& sp, const uint32_t expected_section_length)
: section_parser(sp)
{
    integrity_expect("section magic", "DATA", section_magic());
    integrity_expect("section length", expected_section_length, section_length());
    integrity_expect("header length", 0x0C, sp.read<uint32_t>());

    // event offsets are relative to the first event, which is after the header length
    _m_scope = sp.push_offset_base(sp.tell());
}

parser::labl_section_parser::labl_section_parser(stream_parser& sp, const uint32_t expected_section_length)
: section_parser(sp)
{
    integrity_expect("section magic", "LABL", section_magic());
    integrity_expect("section length", expected_section_length, section_length());
}

vector<pair<streamoff, string>> parser::labl_section_parser::labels()
{
    stream_parser::read_scope scope(_m_sp);
    vector<pair<streamoff, string>> result(_m_sp.read<uint32_t>());

    for (auto& pair : result)
    {
        stream_parser::read_scope scope(_m_sp, streamoff(_m_sp.read<uint32_t>()));

        pair.first = _m_sp.read<uint32_t>();
        for (size_t length = _m_sp.read<uint32_t>(); length > 0; --length)
        {
            pair.second.push_back(_m_sp.read<char>());
        }
    }
    return result;
}
