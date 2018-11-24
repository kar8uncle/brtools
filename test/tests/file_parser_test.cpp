#include <gtest/gtest.h>
#include <io/stream_parser.h>
#include <io/file_parser.h>

#include <sstream>  // for istringstream

using namespace ::testing;
using namespace std;
using namespace brtools::io;

/**
 * Tests that a well-formed big endian file is read correctly.
 * Information in the file should be extracted correctly.
 */
TEST(file_parser, big_endian_file)
{
    const auto STM_CONTENT = "MAGC"             // file magic
                             "\xFE\xFF"         // byte order mark, file is big endian
                             "\xDE\xAD"          // file version
                             "\x00\x00\x00\x14" // size of file = 20 bytes
                             "\x00\x0E"         // size of header = 14 bytes, header ends here
                             "HELLO!"           // actual file content
                             "B"                // content after file, should be read after file_parser destructs
                             ""s;
    istringstream stm(STM_CONTENT);
    stream_parser sp(stm);

    {
        file_parser fp(sp);
        EXPECT_EQ("MAGC"s, fp.file_magic());
        EXPECT_EQ(0xDEAD, fp.file_version());
        EXPECT_EQ(0x000E, fp.file_header_size());
        EXPECT_EQ(0x00000014, fp.file_size());
        EXPECT_EQ('H', sp.read<char>());
    }

    // content after file
    EXPECT_EQ('B', sp.read<char>());
}

/**
 * Tests that a well-formed little endian file is read correctly.
 * Information in the file should be extracted correctly.
 */
TEST(file_parser, 5_byte_magic_little_endian_file)
{
    const auto STM_CONTENT = "MAGIC"            // file magic
                             "\xFF\xFE"         // byte order mark, file is little endian
                             "\xAD\xDE"         // file version
                             "\x14\x00\x00\x00" // size of file = 20 bytes
                             "\x0F\x00"         // size of header = 15 bytes, header ends here
                             "HELLO"            // actual file content
                             "B"                // content after file, should be read after file_parser destructs
                             ""s;
    istringstream stm(STM_CONTENT);
    stream_parser sp(stm);

    {
        file_parser fp(sp, 5);
        EXPECT_EQ("MAGIC"s, fp.file_magic());
        EXPECT_EQ(0xDEAD, fp.file_version());
        EXPECT_EQ(0x000F, fp.file_header_size());
        EXPECT_EQ(0x00000014, fp.file_size());
        EXPECT_EQ('H', sp.read<char>());
    }

    // content after file
    EXPECT_EQ('B', sp.read<char>());
}

/**
 * Tests byte order setting is restored after file_parser destructed.
 */
TEST(file_parser, byte_order_restored_after_completed_read)
{
    const auto STM_CONTENT = "\xBE\xEF"         // for setting expectation

                             "MAGC"             // file magic
                             "\xFF\xFE"         // byte order mark, file is little endian
                             "\xAD\xDE"         // file version
                             "\x0E\x00\x00\x00" // size of file = 14 bytes
                             "\x0E\x00"         // size of header = 14 bytes, header and file ends here

                             "\xBE\xEF"         // same 2 bytes as beginning of stream

                             "MAGC"             // file magic
                             "\xFE\xFF"         // byte order mark, file is big endian
                             "\xDE\xAD"         // file version
                             "\x00\x00\x00\x0E" // size of file = 14 bytes
                             "\x00\x0E"         // size of header = 14 bytes, header and file ends here

                             "\xBE\xEF"         // same 2 bytes as beginning of stream
                             ""s;
    istringstream stm(STM_CONTENT);
    stream_parser sp(stm);

    const auto expected = sp.read<uint16_t>();
    file_parser{sp};
    EXPECT_EQ(expected, sp.read<uint16_t>());
    file_parser{sp};
    EXPECT_EQ(expected, sp.read<uint16_t>());
}
