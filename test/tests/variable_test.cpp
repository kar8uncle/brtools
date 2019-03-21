#include <gtest/gtest.h>

#include <sstream>
#include <brtools/data/types/variable.h>
#include <brtools/io/stream_parser.h>
#include <fixtures/stream_parser_fixture.h>

using namespace ::testing;
using namespace brtools::data::types;
using namespace std;

using brtools::io::stream_parser;
using variable_test = stream_parser_fixture;

namespace
{
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
 * Tests that the parsed variable slot numbers are correct.
 * Ensures that accessing memory now yields all 0x00.
 */
TEST_F(variable_test, variable_slot)
{
    const uint8_t expected[] = {0x08, 0x05, 0x00, 0x0A, 0x1F, 0x2F};
    istringstream stm("\x08\x05\x00\x0A\x1F\x2F"s);
    stream_parser sp = parser(stm);

    for (const auto expected_slot : expected)
    {
        variable v; sp >> v;
        EXPECT_EQ(expected_slot, v.slot());
        EXPECT_EQ(0x00, v);
    }
}

/**
 * Tests that the parsed variable accesses the correct memory.
 */
TEST_F(variable_test, variable_value)
{
    const uint8_t expected[] = {0x08, 0x05, 0x00, 0x09, 0x07, 0x03, 0x01};
    int16_t memory[] = 
    {
        /* 00 */  reinterpret(0xDEAD),
        /* 01 */  reinterpret(0xBEEF),
        /* 02 */  reinterpret(0xCAFE),
        /* 03 */  reinterpret(0xBABE),
        /* 04 */  reinterpret(0xf001),
        /* 05 */  reinterpret(0xce11),
        /* 06 */  reinterpret(0xfee1),
        /* 07 */  reinterpret(0xdea1),
        /* 08 */  reinterpret(0xba11),
        /* 09 */  reinterpret(0x1eaf),
    };
    istringstream stm("\x08\x05\x00\x09\x07\x03\x01"s);
    stream_parser sp = parser(stm);

    variable::memory = memory;
    for (const auto expected_slot : expected)
    {
        variable v; sp >> v;
        EXPECT_EQ(memory[expected_slot], v);
    }
}

/**
 * Tests that the parsed variable modifies the memory correctly.
 */
TEST_F(variable_test, operators)
{
    int16_t memory = 0x7000;
    istringstream stm("\x00"s);
    stream_parser sp = parser(stm);

    variable::memory = &memory;
    variable v; sp >> v;

    {   // Test 0: sanity check
        EXPECT_EQ(0x7000, memory);
        EXPECT_EQ(memory, v);
    }

    {   // Test 1: assign
        v = 0x0FFF;
        EXPECT_EQ(memory, 0x0FFF);
    }

    {   // Test 2: plus equals
        v += 0x0001;
        EXPECT_EQ(memory, 0x1000);
    }

    {   // Test 3: minus equals
        v -= 0x0FFE;
        EXPECT_EQ(memory, 0x0002);
    }

    {   // Test 4: multiply equals
        v *= 0x000A;
        EXPECT_EQ(memory, 0x0014);
    }

    {   // Test 5: divide equals
        v /= 0x0005;
        EXPECT_EQ(memory, 0x0004);
    }

    {   // Test 6: mod equals
        v %= 0x0003;
        EXPECT_EQ(memory, 0x0001);
    }

    {   // Test 7: bitwise-OR equals
        v |= 0x00F0;
        EXPECT_EQ(memory, 0x00F1);
    }

    {   // Test 8: bitwise-AND equals
        v &= 0x7F82;
        EXPECT_EQ(memory, 0x0080);
    }

    {   // Test 9: bitwise-XOR equals
        v ^= 0x00FF;
        EXPECT_EQ(memory, 0x007F);
    }

    // TODO: the other operations are not tested as the behaviors are not yet clear
}
