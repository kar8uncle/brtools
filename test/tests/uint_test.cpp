#include <gtest/gtest.h>

#include <sstream>      // istringstream

#include <data/types/uint.h>
#include <io/stream_parser.h>

using namespace ::testing;
using namespace brtools::data::types;
using namespace std;

using brtools::io::stream_parser;

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
 * Tests that uint works similarly to the primitive unsigned integral types.
 */
TEST(uint_test, standard_sizes)
{
    {   // Test 1: uint8_t
        const auto expected = 0x42;
        istringstream stm("\x42"s);
        stream_parser sp(stm);

        EXPECT_EQ(expected, sp.read<uint<8>>());
    }

    {   // Test 2: uint16_t
        const auto expected = 0xBEEF;
        istringstream stm("\xBE\xEF"s);
        stream_parser sp(stm);
        if (needs_reverse_byte_order())
        {
            sp.reverse_byte_order();
        }

        EXPECT_EQ(expected, sp.read<uint<16>>());
    }

    {   // Test 3: uint32_t
        const auto expected = 0xDEADBEEF;
        istringstream stm("\xDE\xAD\xBE\xEF"s);
        stream_parser sp(stm);
        if (needs_reverse_byte_order())
        {
            sp.reverse_byte_order();
        }

        EXPECT_EQ(expected, sp.read<uint<32>>());
    }

    {   // Test 4: uint64_t
        const auto expected = 0xCafeBabeDeadBeef;
        istringstream stm("\xCA\xFE\xBA\xBE"
                          "\xDE\xAD\xBE\xEF"s);
        stream_parser sp(stm);
        if (needs_reverse_byte_order())
        {
            sp.reverse_byte_order();
        }

        EXPECT_EQ(expected, (sp.read<uint<64>>()));
    }
}

/**
 * Tests that uints with non standard sizes(e.g. 24-bit) can be extracted from
 * stream correctly.
 */
TEST(uint_test, non_standard_sizes)
{
    { // Test 1: uint<24>
        const auto expected = 0x00ABCDEF;
        istringstream stm("\xAB\xCD\xEF"s);
        stream_parser sp(stm);
        if (needs_reverse_byte_order())
        {
            sp.reverse_byte_order();
        }

        EXPECT_EQ(expected, sp.read<uint<24>>());
    }

    { // Test 2: uint<56>
        const auto expected = 0x00'01dDadC001C0de;
        istringstream stm("\x01\xDD\xAD\xC0\x01\xC0\xDE"s);
        stream_parser sp(stm);
        if (needs_reverse_byte_order())
        {
            sp.reverse_byte_order();
        }

        EXPECT_EQ(expected, sp.read<uint<56>>());
    }
}
