#ifndef BRTOOLS_IO_DETAIL_SECTION_PARSER_H
#define BRTOOLS_IO_DETAIL_SECTION_PARSER_H
#pragma once

#include "stream_parser.h"
#include <ios>      // std::streampos
#include <string>

namespace brtools
{
namespace io
{
namespace detail
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
         * Reads offset and size of section, then jumps to the section
         * for user of this parser to perform further parsing.
         *
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

        /**
         * Causes internal input stream to start from after the offset
         * and size.
         */
        ~section_parser();

    public:
        /**
         * Retrieves the section magic read from the section header.
         */
        std::string section_magic() const;

    private:
        /**
         * The underlying stream_parser.
         */
        stream_parser& _m_sp;

        /**
         * The magic read from the header of this section.
         */
        std::string    _m_magic;

        /**
         * Position that should be seeked to when this is destructed.
         */
        std::streampos _m_seek_to_on_destruct;
    };
}
}
}

#endif
