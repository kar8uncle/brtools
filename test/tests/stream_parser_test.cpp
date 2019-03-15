#include <gtest/gtest.h>
#include <io/stream_parser.h>
#include <fixtures/stream_parser_fixture.h>

#include <sstream>      // istringstream
#include <cstdint>      // uint8_t, uint16_t, uint32_t
#include <stdexcept>    // logic_error
#include <string>

using namespace ::testing;
using namespace std;
using namespace brtools::io;

/**
 * Tests reading one byte at a time using the stream_parser::read. This is the
 * only test that uses the lower-level read method.
 */
TEST_F(stream_parser_fixture, read_method_1_byte)
{
    const auto STM_CONTENT = "abcdefg"s;
    istringstream stm(STM_CONTENT);
    stream_parser sp = parser(stm);

    for (const auto byte : STM_CONTENT)
    {
        EXPECT_EQ(byte, *sp.read().get());
    }
}

/**
 * Tests reading one byte at a time using the templated stream_parser::read.
 */
TEST_F(stream_parser_fixture, templated_read_method_1_byte)
{
    const auto STM_CONTENT = "abcdefg"s;
    istringstream stm(STM_CONTENT);
    stream_parser sp = parser(stm);

    for (const auto byte : STM_CONTENT)
    {
        EXPECT_EQ(byte, sp.read<uint8_t>());
    }
}

/**
 * Tests reading one byte at a time using the stream operator.
 */
TEST_F(stream_parser_fixture, read_1_byte)
{
    {   // Test 1: ASCII characters
        const auto STM_CONTENT = "abcdefg"s;
        istringstream stm(STM_CONTENT);
        stream_parser sp = parser(stm);

        for (const auto byte : STM_CONTENT)
        {
            char c; sp >> c;
            EXPECT_EQ(byte, c);
        }
    }

    {   // Test 2: negative int8_t
        const auto STM_CONTENT = "\xFF\xFE\xFD\xFC"s;
        istringstream stm(STM_CONTENT);
        stream_parser sp = parser(stm);

        for (int8_t expected = -1; expected >= -4; --expected)
        {
            int8_t v; sp >> v;
            EXPECT_EQ(expected, v);
        }
    }
}

/**
 * Tests byte reversal for 8-byte integer type. Doesn't care about the initial
 * ordering, as long as the value is correctly reversed after stream_parser::
 * reverse_byte_order is called.
 */
TEST_F(stream_parser_fixture, reverse_8_byte)
{ 
    const auto STM_CONTENT = "\x01\x02\x03\x04\x05\x06\x07\x08"
                             "\x08\x07\x06\x05\x04\x03\x02\x01"
                             ""s;
    istringstream stm(STM_CONTENT);
    stream_parser sp = parser(stm);

    uint64_t original; sp >> original;
    sp.reverse_byte_order();
    uint64_t reversed; sp >> reversed;

    EXPECT_EQ(original, reversed);
}

/**
 * Tests byte reversal for 8-byte integer type. Uses templated read. Doesn't 
 * care about the initial ordering, as long as the value is correctly reversed
 * after stream_parser::reverse_byte_order is called.
 */
TEST_F(stream_parser_fixture, reverse_8_byte_templated_read)
{ 
    const auto STM_CONTENT = "\x01\x02\x03\x04\x05\x06\x07\x08"
                             "\x08\x07\x06\x05\x04\x03\x02\x01"
                             ""s;
    istringstream stm(STM_CONTENT);
    stream_parser sp = parser(stm);

    const auto original = sp.read<uint64_t>();
    sp.reverse_byte_order();
    const auto reversed = sp.read<uint64_t>();

    EXPECT_EQ(original, reversed);
}

/**
 * Tests reading 8 bytes. Checks the actual value that was read. Since the byte
 * order depends on the machine, to ensure this test is deterministic, reverses
 * the byte order to emulate a big-endian machine.
 */
TEST_F(stream_parser_fixture, read_8_bytes)
{
    const auto STM_CONTENT = "\xDE\xAD\xBE\xEF\x0D\x15\xEA\x5E"s;
    istringstream stm(STM_CONTENT);
    stream_parser sp = parser(stm);

    EXPECT_EQ(0xDeadBeef0D15EA5E, sp.read<uint64_t>());
}

/**
 * Tests that a call to tell immediately after seek yields the same position.
 */
TEST_F(stream_parser_fixture, seek_tell_integrity)
{
    const auto STM_CONTENT = "ABCD"s;
    istringstream stm(STM_CONTENT);
    stream_parser sp = parser(stm);

    {   // Test 1: change to read at position 0x1
        const streampos p = 0x1;
        sp.seek(p);
        EXPECT_EQ(p, sp.tell());
    }

    {   // Test 2: change to read at position 0x3
        const streampos p = 0x3;
        sp.seek(p);
        EXPECT_EQ(p, sp.tell());
    }

    {   // Test 3: change back to read at position 0x0
        const streampos p = 0x0;
        sp.seek(p);
        EXPECT_EQ(p, sp.tell());
    }
}

/**
 * Tests that tell returns the correct stream position after reads of different
 * sizes.
 */
TEST_F(stream_parser_fixture, tell_with_reads)
{
    const auto STM_CONTENT = "ABCDEFGH"s;
    istringstream stm(STM_CONTENT);
    stream_parser sp = parser(stm);

    streampos cur_pos = 0x0;

    // Test 0: before any read operator, sp should be reading position 0x0
    EXPECT_EQ(cur_pos, sp.tell());

    {   // Test 1: read 1 byte
        constexpr size_t read_count = 1;
        sp.read(read_count);
        cur_pos += streamoff(read_count);
        EXPECT_EQ(cur_pos, sp.tell());
    }

    {   // Test 2: read 3 bytes
        constexpr size_t read_count = 3;
        sp.read(read_count);
        cur_pos += streamoff(read_count);
        EXPECT_EQ(cur_pos, sp.tell());
    }

    {   // Test 3: read 2 bytes
        constexpr size_t read_count = sizeof(uint16_t);
        sp.read<uint16_t>();
        cur_pos += streamoff(read_count);
        EXPECT_EQ(cur_pos, sp.tell());
    }
}

/**
 * Tests that seek changes the stream to the correct stream position.
 */
TEST_F(stream_parser_fixture, read_with_seeks)
{
    const auto STM_CONTENT = "01234567"s;
    istringstream stm(STM_CONTENT);
    stream_parser sp = parser(stm);

    {   // Test 0: read 1 byte before seek
        EXPECT_EQ('0', sp.read<char>());
    }

    {   // Test 1: read 1 byte after seek to position 2
        sp.seek(streampos(0x2));
        EXPECT_EQ('2', sp.read<char>());
    }

    {   // Test 2: read 1 byte after seek to position 6
        sp.seek(streampos(0x6));
        EXPECT_EQ('6', sp.read<char>());
    }

    {   // Test 3: read 1 byte after seek back to position 0
        sp.seek(streampos(0x0));
        EXPECT_EQ('0', sp.read<char>());
    }
}

/**
 * Tests the offset scoping related methods.
 */
TEST_F(stream_parser_fixture, offset_scoping)
{
    const auto STM_CONTENT = "01234567"s;
    istringstream stm(STM_CONTENT);
    stream_parser sp = parser(stm);
    
    {   // Test 0: seek by offset before pushing a base
        sp.seek_by_offset_from_base(0x1);
        EXPECT_EQ('1', sp.read<char>());
    }
    
    {   // Test 1: pushing and popping(discarding return value) an absolute
        //         base is a no-op
        sp.push_offset_base(streampos(0x5));
        sp.seek_by_offset_from_base(0x1);
        EXPECT_EQ('1', sp.read<char>());
    }
    
    {   // Test 2: pushing and popping(discarding return value) a relative
        //         base is a no-op
        sp.push_offset_base(streamoff(0x5));
        sp.seek_by_offset_from_base(0x1);
        EXPECT_EQ('1', sp.read<char>());
    }

    {   // Test 3: pushing an absolute base, seek by offset then read
        const auto scope = sp.push_offset_base(streampos(0x1));
        sp.seek_by_offset_from_base(0x0);
        EXPECT_EQ('1', sp.read<char>());
    }

    {   // Test 4: pushing an absolute base, a former seek by offset doesn't affect the latter seek
        const auto scope = sp.push_offset_base(streampos(0x1));
        sp.seek_by_offset_from_base(0x5);
        sp.seek_by_offset_from_base(0x0);
        EXPECT_EQ('1', sp.read<char>());
    }

    {   // Test 5: pushing an absolute base, pushing a positive relative base, seek by offset then read
        const auto outer_scope = sp.push_offset_base(streampos(0x1));
        {
            const auto inner_scope = sp.push_offset_base(streamoff(0x1));
            sp.seek_by_offset_from_base(0x1);
            EXPECT_EQ('3', sp.read<char>());
        }
        sp.seek_by_offset_from_base(0x1);
        EXPECT_EQ('2', sp.read<char>());
    }

    {   // Test 6: pushing an absolute base, pushing a negative relative base, seek by offset then read
        const auto outer_scope = sp.push_offset_base(streampos(0x1));
        {
            const auto inner_scope = sp.push_offset_base(streamoff(-0x1));
            sp.seek_by_offset_from_base(0x1);
            EXPECT_EQ('1', sp.read<char>());
        }
    }
}

/**
 * Tests that reading an empty stream raises an ios_base::failure exception.
 */
TEST_F(stream_parser_fixture, overreading_empty_stream_exception)
{
    const string STM_CONTENT;
    istringstream stm(STM_CONTENT);
    stream_parser sp = parser(stm);

    uint32_t read;
    EXPECT_THROW(sp >> read, ios_base::failure);
    EXPECT_THROW(sp.read<uint32_t>(), ios_base::failure);
}

/**
 * Tests that reading more than what the stream contains raises an ios_base::
 * failure exception.
 */
TEST_F(stream_parser_fixture, overreading_nonempty_stream_exception)
{
    const auto STM_CONTENT = "abc"s;
    istringstream stm(STM_CONTENT);
    stream_parser sp = parser(stm);

    uint32_t read;
    EXPECT_THROW(sp >> read, ios_base::failure);
    EXPECT_THROW(sp.read<uint32_t>(), ios_base::failure);
}

/**
 * Tests that requesting to read 0 byte raises an invalid_argument exception.
 */
TEST_F(stream_parser_fixture, read_0_byte_exception)
{
    const string STM_CONTENT;
    istringstream stm(STM_CONTENT);
    stream_parser sp = parser(stm);

    EXPECT_THROW(sp.read(0), invalid_argument);
}
