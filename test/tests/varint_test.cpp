#include <gtest/gtest.h>

#include <sstream>      // istringstream

#include <data/types/varint.h>
#include <io/stream_parser.h>

using namespace ::testing;
using namespace brtools::data::types;
using namespace std;

using brtools::io::stream_parser;

/**
 * Tests that varint works correctly if the data requires only a one-byte read.
 */
TEST(varint_test, one_byte_read)
{
    {   // Test 1: without extra, unused data
        const auto expected = 0b0'1001011;
        istringstream stm("\x4B"s);
        stream_parser sp(stm);

        EXPECT_EQ(expected, sp.read<varint>());
    }

    {   // Test 2: with extra, unused data
        const auto expected = 0b0'1001011;
        istringstream stm("\x4B"
                          "\xFF\xFF"s);
        stream_parser sp(stm);

        EXPECT_EQ(expected, sp.read<varint>());
    }
}

/**
 * Tests that varint works correctly if the data requires a two-byte read.
 */
TEST(varint_test, two_byte_read)
{
    {   // Test 1: without extra, unused data
        const auto expected = 0b0000001'1001011;
        istringstream stm("\x81\x4B"s); // 1 0000001 | 0 1001011
        stream_parser sp(stm);

        EXPECT_EQ(expected, sp.read<varint>());
    }

    {   // Test 2: with extra, unused data
        const auto expected = 0b1100'1011;
        istringstream stm("\x81\x4B"
                          "\xFF\xFF"s); // unused
        stream_parser sp(stm);

        EXPECT_EQ(expected, sp.read<varint>());
    }
}

/**
 * Tests that varint works correctly if the data requires a three-byte read.
 */
TEST(varint_test, three_byte_read)
{
    {   // Test 1: without extra, unused data
        const auto expected = 0b0010010'1001010'1001011;
        istringstream stm("\x92\xCA\x4B"s); // 1 0010010 | 1 1001010 | 0 1001011
        stream_parser sp(stm);

        EXPECT_EQ(expected, sp.read<varint>());
    }

    {   // Test 2: with extra, unused data
        const auto expected = 0b0010010'1001010'1001011;
        istringstream stm("\x92\xCA\x4B"
                          "\xFA\x34\xBE"s); // unused
        stream_parser sp(stm);

        EXPECT_EQ(expected, sp.read<varint>());
    }
}
