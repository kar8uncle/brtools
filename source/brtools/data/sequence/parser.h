#ifndef BRTOOLS_DATA_SEQUENCE_PARSER_H
#define BRTOOLS_DATA_SEQUENCE_PARSER_H
#pragma once

#include <memory>       // std::unique_ptr
#include <iosfwd>       // std::streamoff
#include <utility>      // std::pair
#include <cstdint>      // uint32_t
#include <vector>
#include <string>

#include <brtools/data/types/sized_ref.h>

#include <brtools/io/section_parser.h>
#include <brtools/io/file_parser.h>

namespace brtools
{
namespace data
{
namespace sequence
{
    class parser final : io::file_parser
    {
    public:
        parser(io::stream_parser&);
    
    private:
        struct data_section_parser final : io::section_parser
        {
            data_section_parser(io::stream_parser&, uint32_t expected_section_length);

        private:
            std::unique_ptr<io::stream_parser::offset_scope> _m_scope;
        };

        struct labl_section_parser final : io::section_parser
        {
            labl_section_parser(io::stream_parser&, uint32_t expected_section_length);

            std::vector<std::pair<std::streamoff, std::string>> labels();
        };

    private:
        types::sized_ref _m_data_section_ref;
        types::sized_ref _m_labl_section_ref;

    public:
        data_section_parser data();
        labl_section_parser labl();
    };
}
}
}

#endif
