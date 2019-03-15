#include <gtest/gtest.h>
#include <io/section_parser.h>
#include <io/stream_parser.h>
#include <error/integrity_error.h>
#include <fixtures/stream_parser_fixture.h>

#include <sstream>  // for istringstream
#include <cstdint>  // for uint8_t, uint16_t

using namespace ::testing;
using namespace std;
using namespace brtools::io;
using section_parser_test = stream_parser_fixture;

/**
 * Tests that a well-formed section won't cause section_parser to throw exceptions.
 *
 * After section_parser construction, the next read should yield data in the section
 * but not the header that contains the magic and section size.
 *
 * After section_parser destruction, the next read should yield data just after the
 * section offset and size.
 */
TEST_F(section_parser_test, well_formed_section)
{
    const auto STM_CONTENT = "\x00\x00\x00\x0A" // offset to section from base = 10 bytes
                             "\x00\x00\x00\x10" // size of section = 16 bytes
                             "\xDE\xAD"         // after destruct, next reads should be 0xDEAD
                             "BEEF"             // section magic
                             "\x00\x00\x00\x10" // size of section = 16 bytes
                             "JUNKHERE"         // after construct, next reads should be "JUNKHERE"
                             ""s;
    istringstream stm(STM_CONTENT);
    stream_parser sp = parser(stm);

    {
        section_parser secp(sp);
        EXPECT_EQ("BEEF"s, secp.section_magic());
        for (const auto ec : "JUNKHERE"s)
        {
            EXPECT_EQ(ec, sp.read<uint8_t>());
        }
    }

    EXPECT_EQ(0xDEAD, sp.read<uint16_t>());
}

using brtools::error::integrity_error;

/**
 * Tests that an integrity_error is thrown if section size specified in section is
 * not equal to that specified outside of the section.
 */
TEST_F(section_parser_test, size_mismatch_exception)
{
    const auto STM_CONTENT = "\x00\x00\x00\x0A" // offset to section from base = 10 bytes
                             "\x00\x00\x00\x10" // size of section = 16 bytes
                             "\xDE\xAD"         // unused
                             "BEEF"             // section magic
                             "\x00\x00\x00\x0A" // size of section = 10 bytes, mismatches with above
                             "JUNKHERE"         // to make up 16 byte
                             ""s;
    istringstream stm(STM_CONTENT);
    stream_parser sp = parser(stm);

    EXPECT_THROW(section_parser{sp}, integrity_error);
}
