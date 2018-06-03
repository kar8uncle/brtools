#include <gtest/gtest.h>
#include <io/detail/section_parser.h>
#include <io/detail/stream_parser.h>
#include <io/detail/integrity_error.h>

#include <sstream>  // for istringstream
#include <cstdint>  // for uint8_t, uint16_t

using namespace ::testing;
using namespace std;
using namespace brtools::io::detail;

namespace
{
    /**
     * We don't know the endianness of the machine, so it is imperative that we
     * check if we need to reverse the byte order in order to run the tests in
     * a deterministic fashion. For readability, let's use big endian.
     */
    bool needs_reverse_byte_order()
    {
        const auto BYTE_ORDER_MARK = "\xFF\xFE"s;
        istringstream stm(BYTE_ORDER_MARK);
        return stream_parser(stm).read<uint16_t>() != 0xFFFE;
    }
}

/**
 * Tests that a well-formed section won't cause section_parser to throw exceptions.
 *
 * After section_parser construction, the next read should yield data in the section
 * but not the header that contains the magic and section size.
 *
 * After section_parser destruction, the next read should yield data just after the
 * section offset and size.
 */
TEST(section_parser, well_formed_section)
{
    const auto STM_CONTENT = "\x00\x00\x00\x0A" // offset to section from base = 10 bytes
                             "\x00\x00\x00\x10" // size of section = 16 bytes
                             "\xDE\xAD"         // after destruct, next reads should be 0xDEAD
                             "BEEF"             // section magic
                             "\x00\x00\x00\x10" // size of section = 16 bytes
                             "JUNKHERE"         // after construct, next reads should be "JUNKHERE"
                             ""s;
    istringstream stm(STM_CONTENT);
    stream_parser sp(stm);
    if (needs_reverse_byte_order())
    {
        sp.reverse_byte_order();
    }

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

/**
 * Tests that an integrity_error is thrown if section size specified in section is
 * not equal to that specified outside of the section.
 */
TEST(section_parser, size_mismatch_exception)
{
    const auto STM_CONTENT = "\x00\x00\x00\x0A" // offset to section from base = 10 bytes
                             "\x00\x00\x00\x10" // size of section = 16 bytes
                             "\xDE\xAD"         // unused
                             "BEEF"             // section magic
                             "\x00\x00\x00\x0A" // size of section = 10 bytes, mismatches with above
                             "JUNKHERE"         // to make up 16 byte
                             ""s;
    istringstream stm(STM_CONTENT);
    stream_parser sp(stm);
    if (needs_reverse_byte_order())
    {
        sp.reverse_byte_order();
    }

    EXPECT_THROW(section_parser{sp}, integrity_error);
}
