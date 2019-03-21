#ifndef BRTOOLS_IO_SECTION_PARSER_H
#define BRTOOLS_IO_SECTION_PARSER_H
#pragma once

#include <brtools/io/stream_parser.h>
#include <iosfwd>   // std::streampos
#include <memory>   // std::unique_ptr
#include <string>

namespace brtools
{
namespace io
{
    class stream_parser;

    /**
     * Contains common functions for parsing a section. Abstracts out
     * some integrity checks. Intended to be subclassed.
     */
    class section_parser
    {
    public:    
        /**
         * Reads and populates the section magic read from header.
         *
         * Checks if section size specified in the section matches with
         * the size specified in header.
         *
         * @param sp           The stream_parser to read from.
         * @param magic_length The length of section magic. BR magics
         *                     are usually 4 bytes long.
         *
         * @throws integrity_error if size specified in header mismatches
         *                         that specified within the section.
         */
        section_parser(stream_parser& sp, std::string::size_type magic_length = 4);
        section_parser(section_parser&&) = default;

        /**
         * Causes internal input stream to start from after the offset
         * and size.
         */
        virtual ~section_parser();

    public:
        /**
         * Retrieves the section magic read from the section header.
         */
        std::string section_magic() const;

        /**
         * Retrieves the section length as read from the section header.
         */
        uint32_t section_length() const;

    public:
        /**
         * Indicates whether there is still data in the section to read.
         */
        bool has_data_to_read() const;

    protected:
        /**
         * The underlying stream_parser.
         */
        stream_parser& _m_sp;

    private:
        /**
         * The magic read from the header of this section.
         */
        std::string    _m_magic;

        /**
         * The length read from the header of this section.
         */
        uint32_t       _m_length;

        /**
         * Manages lifecycle of offset base pushed during construction.
         */
        std::unique_ptr<stream_parser::offset_scope> _m_offset_scope;

        /**
         * Position of the end of this section. Seeked to on destruction.
         */
        std::streampos _m_end_of_section;
    };
}
}

#endif
