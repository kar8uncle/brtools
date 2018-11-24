#ifndef BRTOOLS_IO_FILE_PARSER_H
#define BRTOOLS_IO_FILE_PARSER_H
#pragma once

#include "stream_parser.h"
#include <ios>      // std::streampos
#include <cstdint>  // uint16_t
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
    class file_parser
    {
    public:
        using version_num_type = uint16_t;
        using header_size_type = uint16_t;
        using file_size_type   = uint32_t;

    public:    
        /**
         * Reads header of file and extracts magic. Determine if byte
         * needs to be reversed. User of this class can perform further 
         * parsing after construction.
         *
         * @param sp           The stream_parser to read from.
         * @param magic_length The length of section magic. BR magics
         *                     are usually 4 bytes long.
         */
        file_parser(stream_parser& sp, std::string::size_type magic_length = 4);

        /**
         * Causes internal input stream to start from after the file;
         * that is, streampos at construction + file size specified in
         * the file header.
         *
         * Re-reverse the bytes if bytes are set to reverse during con-
         * struction of this instance.
         */
        ~file_parser();

    public: // file info in header
        /**
         * Retrieves the file magic read from the file header.
         */
        std::string file_magic() const;

        /**
         * Retrieves the file version read from the file header.
         */
        version_num_type file_version() const;

        /**
         * Retrieves the file header size read from the file header.
         */
        header_size_type file_header_size() const;

        /**
         * Retrieves the file size read from the file header.
         */
        file_size_type file_size() const;

    private:
        /**
         * The underlying stream_parser.
         */
        stream_parser&    _m_sp;

        /**
         * The magic read from the header of this section.
         */
        std::string       _m_magic;

        /**
         * If true, indicates that bytes are set to reverse in construc-
         * tion time, and needs to be set again when destructed.
         */ 
        bool              _m_reverse_bytes_on_destruct;

        /**
         * Version number read from file header.
         */
        version_num_type  _m_version_number;

        /**
         * Header size read from file header.
         */
        header_size_type  _m_header_size;
        
        /**
         * File size read from file header.
         */
        file_size_type    _m_size;
    };
}
}

#endif
