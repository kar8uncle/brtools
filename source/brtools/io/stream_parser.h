#ifndef BRTOOLS_IO_STREAM_PARSER_H
#define BRTOOLS_IO_STREAM_PARSER_H
#pragma once

#include <istream>
#include <ios>              // std::streampos, std::streamoff
#include <memory>           // std::unique_ptr
#include <type_traits>      // std::is_integral
#include <stack>

namespace brtools
{
namespace io
{
    class stream_parser
    {
    public:
        stream_parser(std::istream&);

        /**
         * Byte ordering is dependent on endianness of the machine. By default,
         * when reading multiple bytes, the bytes are not reversed, so the ma-
         * chine endianness difference can cause the program behavior to be dif-
         * ferent on different machines.
         * 
         * You should take advantage of the file's byte order mark to determine 
         * if the bytes need to be in reverse order.
         * 
         * When this is called, subsequent multi-byte reads will yield reversed 
         * results.
         */
        void reverse_byte_order();

    public: // read methods
        /**
         * Reads specified number of bytes from stream. Reverses bytes if number_
         * of_bytes is greater than 1, and there was a call to stream_parser::
         * reverse_byte_order before.
         *
         * @throws std::ios_base::failure
         *         if the good bit is not set after this read.
         *
         * @throws std::invalid_argument
         *         if number_of_bytes == 0
         */
        std::unique_ptr<const char[]> read(size_t number_of_bytes = 1);

        /**
         * Reads a value of type Tp from the parser, but doesn't change the reading
         * position of the stream.
         */
        template<typename Tp>
        Tp peek() const
        {
            const read_scope scope(const_cast<stream_parser&>(*this));
            return const_cast<stream_parser*>(this)->read<Tp>();
        }

        /**
         * Reads a value of type Tp from the parser. Alias of the stream operator.
         * Typically Tp is an integral type, but since the stream operator can be
         * specialized for other types, Tp can be any specialized stream_parser sup-
         * ported type.
         */
        template<typename Tp>
        Tp read()
        {
            Tp v{}; *this >> v; return v;
        }

    public: // position related
        /**
         * RAII-style object to seek to the position pointed to when this read_scope
         * is created, on destruction.
         */
        struct read_scope
        {
            read_scope(stream_parser&);

            /**
             * @param seek_afterwards After the current position is recorded,
             *                        seek to the specified position.
             */
            read_scope(stream_parser&, std::streampos);

            /**
             * @param seek_afterwards After the current position is recorded,
             *                        seek to current offset base + specified offset.
             */
            read_scope(stream_parser&, std::streamoff seek_afterwards);
            ~read_scope();

        private:
            stream_parser& _m_parser;
            std::streampos _m_original_pos;
        };

        /**
         * Retrieves the current stream position being read.
         */
        std::streampos tell() const;

        /**
         * Retrieves the offset of the current stream position relative to the
         * current offset base.
         */
        std::streamoff tell_offset_from_base() const;

        /**
         * Changes the current stream position being read to the given absolute
         * stream position.
         */
        void seek(std::streampos);

    public:
        /**
         * RAII-style object to ensure pushed offset bases are properly popped
         * when the offset_scope goes out of scope.
         */
        struct offset_scope
        {
            offset_scope(std::stack<std::streampos>&);
            ~offset_scope();

        private:
            std::stack<std::streampos>& _m_offset_bases;
        };

    public: // offset related
        /**
         * Pushes a scope where offsets specified in subsequent calls to 
         * stream_parser::seek_by_offset_from_base are relative to. Does not change 
         * current read stream position.
         *
         * @param absolute_pos The absolute position in the stream to use as the
         *                     offset base.
         */
        std::unique_ptr<offset_scope> push_offset_base(std::streampos absolute_pos);

        /**
         * Pushes a scope where offsets specified in subsequent calls to 
         * stream_parser::seek_by_offset_from_base are relative to. Does not change
         * current read stream position.
         *
         * @param relative_pos The relative position in the stream to use as the
         *                     offset base; that is, relative_pos + current off-
         *                     set base is the new offset base.
         */
        std::unique_ptr<offset_scope> push_offset_base(std::streamoff relative_pos);

        /**
         * Seeks to the position in the stream offset by the given offset from
         * the current offset base. An offset base can be pushed by calling
         * stream_parser::push_offset_base.
         * 
         * Before any scope is pushed, the base is the beginning of file, or
         * std::streampos(0x0).
         */
        void seek_by_offset_from_base(std::streamoff);

    private:
        std::istream&              _m_stream_in;
        bool                       _m_should_reverse_bytes;
        std::stack<std::streampos> _m_offset_bases;
    };

    /**
     * Enables use of stream operator like standard istreams. LHS is an instance
     * of stream_parser, and RHS is typically a modifiable integral lvalue. Note 
     * that this operator may be specialized, so RHS is not required to be an
     * integral type.
     */
    template<typename Tp>
    stream_parser& operator>>(stream_parser& parser, Tp& value)
    {
        static_assert(std::is_integral<Tp>::value,
                      "Stream operator use for non-integral types "
                      "must be specialized.");
        value = *reinterpret_cast<const Tp*>(parser.read(sizeof(value)).get());
        return parser;
    }
}
}
#endif
