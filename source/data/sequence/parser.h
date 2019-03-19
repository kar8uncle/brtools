#ifndef BRTOOLS_DATA_SEQUENCE_PARSER_H
#define BRTOOLS_DATA_SEQUENCE_PARSER_H
#pragma once

#include <memory>       // std::unique_ptr
#include <iosfwd>       // std::streamoff
#include <utility>      // std::pair
#include <vector>
#include <string>
#include <io/section_parser.h>
#include <io/file_parser.h>

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

            const std::vector<std::pair<std::streamoff, std::string>>& labels() const;

        private:
            std::vector<std::pair<std::streamoff, std::string>> _m_labels;
        };

    private:
        uint32_t offset_to_data;
        uint32_t data_section_length;
        uint32_t offset_to_labl;
        uint32_t labl_section_length;

    public:
        data_section_parser data();
        labl_section_parser labl();
    };
}
}
}

#endif
