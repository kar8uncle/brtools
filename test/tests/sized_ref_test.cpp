#include <gtest/gtest.h>

#include <sstream>  // istringstream

#include <brtools/data/types/sized_ref.h>
#include <brtools/io/stream_parser.h>
#include <fixtures/stream_parser_fixture.h>

using namespace ::testing;
using namespace brtools::data::types;
using namespace std;

using brtools::io::stream_parser;
using sized_ref_test = stream_parser_fixture;

/**
 * Tests that the parsing of a sized_ref type is correct.
 */
TEST_F(sized_ref_test, parsing)
{
    {   // Test 1: all 0s
        istringstream stm("\x00\x00\x00\x00"
                          "\x00\x00\x00\x00"s);
        stream_parser sp = parser(stm);

        sized_ref ref; sp >> ref;
        EXPECT_EQ(0x00, ref.offset());
        EXPECT_EQ(0x00, ref.length());
    }

    {   // Test 2: some random data
        istringstream stm("\xDE\xAD\xBE\xEF"
                          "\xCA\xFE\xBA\xBE"s);
        stream_parser sp = parser(stm);

        sized_ref ref; sp >> ref;
        EXPECT_EQ(0xDEAD'BEEF, ref.offset());
        EXPECT_EQ(0xCAFE'BABE, ref.length());
    }
}
