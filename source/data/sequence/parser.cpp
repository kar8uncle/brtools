#include "parser.h"
#include <util/integrity_expect.h>
#include <algorithm>    // generate

using brtools::util::integrity_expect;
using namespace brtools::data::sequence;
using namespace brtools::io;
using namespace std;

parser::parser(stream_parser& sp)
: file_parser(sp)
{
    integrity_expect("file magic", "RSEQ", file_magic());
    integrity_expect("file size", 0x20, file_header_size());
    integrity_expect("section count", 2, sp.read<uint16_t>());
    sp >> offset_to_data >> data_section_length
       >> offset_to_labl >> labl_section_length;
}

parser::data_section_parser parser::data()
{
    _m_sp.seek_by_offset_from_base(offset_to_data);
    return data_section_parser(_m_sp, data_section_length);
}

parser::labl_section_parser parser::labl()
{
    _m_sp.seek_by_offset_from_base(offset_to_labl);
    return labl_section_parser(_m_sp, labl_section_length);
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

    const auto label_count = sp.read<uint32_t>();
    vector<streamoff> offsets_to_strings;

    for (size_t count = 1; count <= label_count; ++count)
    {
        offsets_to_strings.push_back(sp.read<uint32_t>());    
    }

    for (auto offset_to_string : offsets_to_strings)
    {
        _m_sp.seek_by_offset_from_base(offset_to_string);

        _m_labels.emplace_back(streamoff(sp.read<uint32_t>()), string(sp.read<uint32_t>(), '\0'));
        generate(_m_labels.back().second.begin(),
                 _m_labels.back().second.end(),
                 bind(&stream_parser::read<char>, &sp));
    }
}

const vector<pair<streamoff, string>>& parser::labl_section_parser::labels() const
{
    return _m_labels;
}
