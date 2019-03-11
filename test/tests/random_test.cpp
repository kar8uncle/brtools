#include <gtest/gtest.h>

#include <sstream>      // istringstream

#include <data/types/random.h>
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

    /**
     * Helper to interpret the bits of an unsigned 16-bit integer as a signed
     * 16-bit integer.
     */
    int16_t reinterpret(const uint16_t& v)
    {
        return reinterpret_cast<const int16_t&>(v);
    }
}

/**
 * Tests that u_rand boundaries are parsed correctly.
 */
TEST(random_test, unsigned_boundary_parsing)
{
    {   // Test 1: 1 byte
        const uint16_t expected[] = { 0x0000, 0x0012 };
        istringstream stm("\x00\x00\x00\x12"s);
        stream_parser sp(stm);
        if (needs_reverse_byte_order())
        {
            sp.reverse_byte_order();
        }

        u_rand ur; sp >> ur;
        EXPECT_EQ(expected[0], ur.lower_bound());
        EXPECT_EQ(expected[1], ur.upper_bound());
    }

    {   // Test 2: 1 byte and 2 bytes
        const uint16_t expected[] = { 0x0042, 0xABCD };
        istringstream stm("\x00\x42\xAB\xCD"s);
        stream_parser sp(stm);
        if (needs_reverse_byte_order())
        {
            sp.reverse_byte_order();
        }

        u_rand ur; sp >> ur;
        EXPECT_EQ(expected[0], ur.lower_bound());
        EXPECT_EQ(expected[1], ur.upper_bound());
    }

    {   // Test 3: 2 bytes
        const uint16_t expected[] = { 0xBABE, 0xBEEF };
        istringstream stm("\xBA\xBE\xBE\xEF"s);
        stream_parser sp(stm);
        if (needs_reverse_byte_order())
        {
            sp.reverse_byte_order();
        }

        u_rand ur; sp >> ur;
        EXPECT_EQ(expected[0], ur.lower_bound());
        EXPECT_EQ(expected[1], ur.upper_bound());
    }
}

/**
 * Tests that s_rand boundaries are parsed correctly.
 */
TEST(random_test, signed_boundary_parsing)
{
    {   // Test 1: 1 byte
        const int16_t expected[] = { reinterpret(0x0000), reinterpret(0x0012) };
        istringstream stm("\x00\x00\x00\x12"s);
        stream_parser sp(stm);
        if (needs_reverse_byte_order())
        {
            sp.reverse_byte_order();
        }

        s_rand sr; sp >> sr;
        EXPECT_EQ(expected[0], sr.lower_bound());
        EXPECT_EQ(expected[1], sr.upper_bound());
    }

    {   // Test 2: 1 byte and 2 bytes
        const int16_t expected[] = { reinterpret(0xBABE), reinterpret(0x001D) };
        istringstream stm("\xBA\xBE\x00\x1D"s);
        stream_parser sp(stm);
        if (needs_reverse_byte_order())
        {
            sp.reverse_byte_order();
        }

        s_rand sr; sp >> sr;
        EXPECT_EQ(expected[0], sr.lower_bound());
        EXPECT_EQ(expected[1], sr.upper_bound());
    }

    {   // Test 3: 2 bytes
        const int16_t expected[] = { reinterpret(0xFFFF), reinterpret(0x7FFF) };
        istringstream stm("\xFF\xFF\x7F\xFF"s);
        stream_parser sp(stm);
        if (needs_reverse_byte_order())
        {
            sp.reverse_byte_order();
        }

        s_rand sr; sp >> sr;
        EXPECT_EQ(expected[0], sr.lower_bound());
        EXPECT_EQ(expected[1], sr.upper_bound());
    }
}

// since we are talking about random numbers, one try may not be reliable;
// let's try a relatively large number of times.
constexpr size_t try_times = 100;
/**
 * Tests that the generated unsigned random value falls between the specified range.
 */
TEST(random_test, generated_unsigned_value_in_bounds)
{
    {   // Test 1: 1 byte
        istringstream stm("\x00\x00\x00\x12"s);
        stream_parser sp(stm);
        if (needs_reverse_byte_order())
        {
            sp.reverse_byte_order();
        }

        u_rand ur; sp >> ur;
        for (size_t t = 0; t < try_times; ++t)
        {
            const uint16_t value = ur;
            EXPECT_LE(ur.lower_bound(), value); EXPECT_GT(ur.upper_bound(), value);
        }
    }

    {   // Test 2: 1 byte and 2 bytes
        istringstream stm("\x00\x42\xAB\xCD"s);
        stream_parser sp(stm);
        if (needs_reverse_byte_order())
        {
            sp.reverse_byte_order();
        }

        u_rand ur; sp >> ur;
        for (size_t t = 0; t < try_times; ++t)
        {
            const uint16_t value = ur;
            EXPECT_LE(ur.lower_bound(), value); EXPECT_GT(ur.upper_bound(), value);
        }
    }

    {   // Test 3: 2 bytes
        istringstream stm("\xBA\xBE\xBE\xEF"s);
        stream_parser sp(stm);
        if (needs_reverse_byte_order())
        {
            sp.reverse_byte_order();
        }

        u_rand ur; sp >> ur;
        for (size_t t = 0; t < try_times; ++t)
        {
            const uint16_t value = ur;
            EXPECT_LE(ur.lower_bound(), value); EXPECT_GT(ur.upper_bound(), value);
        }
    }
}

/**
 * Tests that the generated signed random value falls between the specified range.
 */
TEST(random_test, generated_signed_value_in_bounds)
{
    {   // Test 1: 1 byte
        istringstream stm("\x00\x00\x00\x12"s);
        stream_parser sp(stm);
        if (needs_reverse_byte_order())
        {
            sp.reverse_byte_order();
        }

        s_rand sr; sp >> sr;
        for (size_t t = 0; t < try_times; ++t)
        {
            const int16_t value = sr;
            EXPECT_LE(sr.lower_bound(), value); EXPECT_GT(sr.upper_bound(), value);
        }
    }

    {   // Test 2: 1 byte and 2 bytes
        istringstream stm("\xBA\xBE\x00\x1D"s);
        stream_parser sp(stm);
        if (needs_reverse_byte_order())
        {
            sp.reverse_byte_order();
        }

        s_rand sr; sp >> sr;
        for (size_t t = 0; t < try_times; ++t)
        {
            const int16_t value = sr;
            EXPECT_LE(sr.lower_bound(), value); EXPECT_GT(sr.upper_bound(), value);
        }
    }

    {   // Test 3: 2 bytes
        istringstream stm("\xFF\xFF\x7F\xFF"s);
        stream_parser sp(stm);
        if (needs_reverse_byte_order())
        {
            sp.reverse_byte_order();
        }

        s_rand sr; sp >> sr;
        for (size_t t = 0; t < try_times; ++t)
        {
            const int16_t value = sr;
            EXPECT_LE(sr.lower_bound(), value); EXPECT_GT(sr.upper_bound(), value);
        }
    }
}
