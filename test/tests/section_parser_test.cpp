#include <gtest/gtest.h>
#include <io/section_parser.h>
#include <io/stream_parser.h>
#include <fixtures/stream_parser_fixture.h>

#include <sstream>  // for istringstream
#include <cstdint>  // for uint8_t, uint16_t

using namespace ::testing;
using namespace std;
using namespace brtools::io;
using section_parser_test = stream_parser_fixture;

/**
 * After section_parser construction, the next read should yield data in the section
 * but not the header that contains the magic and section size.
 * 
 * After section_parser destruction, the next read should yield data just after the
 * section start + size.
 */
TEST_F(section_parser_test, section_info_population)
{
    const auto STM_CONTENT = "BEEF"             // section magic
                             "\x00\x00\x00\x14" // size of section = 20 bytes
                             "JUNKHERE"         // after construct, next reads should be "JUNKHERE"
                             "\xCA\xFE\xBA\xBE" // more data in section as indicated by the size
                             "\xDE\xAD"         // after destruct, next read should be 0xDEAD
                             ""s;
    istringstream stm(STM_CONTENT);
    stream_parser sp = parser(stm);

    {
        section_parser secp(sp);
        EXPECT_EQ("BEEF"s, secp.section_magic());
        EXPECT_EQ(0x0014, secp.section_length());
        for (const auto ec : "JUNKHERE"s)
        {
            EXPECT_TRUE(secp.has_data_to_read());
            EXPECT_EQ(ec, sp.read<uint8_t>());
        }

        // seek to after the section
        sp.seek_by_offset_from_base(0x0E);
        EXPECT_FALSE(secp.has_data_to_read());
    }

    EXPECT_EQ(0xDEAD, sp.read<uint16_t>());
}
