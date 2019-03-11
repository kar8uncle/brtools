#include <gtest/gtest.h>
#include <data/sequence/sequence.h>
#include <data/types/variable.h>
#include <error/integrity_error.h>

#include <mocks/mock_sequence_visitor.h>

#include <sstream>
#include <string>

using namespace ::testing;
using namespace std;
using namespace brtools::data::sequence;
using brtools::error::integrity_error;

/**
 * Tests that when file magic is not RSEQ, integrity_error is thrown.
 */
TEST(sequence, file_magic_integrity_error)
{
    const auto STM_CONTENT =
/* 00 */ "SSEQ"             // error - file magic
/* 04 */ "\xFE\xFF"         // big endian BOM
/* 06 */ "\x01\x00"         // file version
/* 08 */ "\x00\x00\x00\x38" // file length
/* 0C */ "\x00\x20"         // file header length
/* 0E */ "\x00\x02"         // number of file sections
/* 10 */ "\x00\x00\x00\x20" // offset to DATA section
/* 14 */ "\x00\x00\x00\x0C" // length of DATA section
/* 18 */ "\x00\x00\x00\x2C" // offset to LABL section
/* 1C */ "\x00\x00\x00\x0C" // length of LABL section

/* 20 */ "DATA"             // section magic
/* 24 */ "\x00\x00\x00\x0C" // section length
/* 28 */ "\x00\x00\x00\x0C" // section header size

/* 2C */ "LABL"             // section magic
/* 30 */ "\x00\x00\x00\x0C" // section length
/* 34 */ "\x00\x00\x00\x00" // number of labels: 0

/* 38 */ ""s;

    istringstream stm(STM_CONTENT);
    EXPECT_THROW(sequence::make_sequence(stm), integrity_error);
}

/**
 * Tests that when file header length isn't 0x20, integrity_error is thrown.
 */
TEST(sequence, file_header_length_integrity_error)
{
    const auto STM_CONTENT =
/* 00 */ "RSEQ"             // file magic
/* 04 */ "\xFE\xFF"         // big endian BOM
/* 06 */ "\x01\x00"         // file version
/* 08 */ "\x00\x00\x00\x38" // file length
/* 0C */ "\x00\xFF"         // error - file header length
/* 0E */ "\x00\x02"         // number of file sections
/* 10 */ "\x00\x00\x00\x20" // offset to DATA section
/* 14 */ "\x00\x00\x00\x0C" // length of DATA section
/* 18 */ "\x00\x00\x00\x2C" // offset to LABL section
/* 1C */ "\x00\x00\x00\x0C" // length of LABL section

/* 20 */ "DATA"             // section magic
/* 24 */ "\x00\x00\x00\x0C" // section length
/* 28 */ "\x00\x00\x00\x0C" // section header size

/* 2C */ "LABL"             // section magic
/* 30 */ "\x00\x00\x00\x0C" // section length
/* 34 */ "\x00\x00\x00\x00" // number of labels: 0

/* 38 */ ""s;

    istringstream stm(STM_CONTENT);
    EXPECT_THROW(sequence::make_sequence(stm), integrity_error);
}

/**
 * Tests that when file header doesn't indicate file having exactly 2 sections,
 * integrity_error is thrown.
 */
TEST(sequence, file_section_count_integrity_error)
{
    const auto STM_CONTENT =
/* 00 */ "RSEQ"             // error - file magic
/* 04 */ "\xFE\xFF"         // big endian BOM
/* 06 */ "\x01\x00"         // file version
/* 08 */ "\x00\x00\x00\x38" // file length
/* 0C */ "\x00\x20"         // file header length
/* 0E */ "\x00\x00"         // error - number of file sections
/* 10 */ "\x00\x00\x00\x20" // offset to DATA section
/* 14 */ "\x00\x00\x00\x0C" // length of DATA section
/* 18 */ "\x00\x00\x00\x2C" // offset to LABL section
/* 1C */ "\x00\x00\x00\x0C" // length of LABL section

/* 20 */ "DATA"             // section magic
/* 24 */ "\x00\x00\x00\x0C" // section length
/* 28 */ "\x00\x00\x00\x0C" // section header size

/* 2C */ "LABL"             // section magic
/* 30 */ "\x00\x00\x00\x0C" // section length
/* 34 */ "\x00\x00\x00\x00" // number of labels: 0

/* 38 */ ""s;

    istringstream stm(STM_CONTENT);
    EXPECT_THROW(sequence::make_sequence(stm), integrity_error);
}

/**
 * Tests that when the first section's magic is not DATA, integrity_error is thrown.
 */
TEST(sequence, DATA_section_magic_integrity_error)
{
    const auto STM_CONTENT =
/* 00 */ "RSEQ"             // file magic
/* 04 */ "\xFE\xFF"         // big endian BOM
/* 06 */ "\x01\x00"         // file version
/* 08 */ "\x00\x00\x00\x38" // file length
/* 0C */ "\x00\x20"         // file header length
/* 0E */ "\x00\x02"         // number of file sections
/* 10 */ "\x00\x00\x00\x20" // offset to DATA section
/* 14 */ "\x00\x00\x00\x0C" // length of DATA section
/* 18 */ "\x00\x00\x00\x2C" // offset to LABL section
/* 1C */ "\x00\x00\x00\x0C" // length of LABL section

/* 20 */ "BAKA"             // error - section magic
/* 24 */ "\x00\x00\x00\x0C" // section length
/* 28 */ "\x00\x00\x00\x0C" // section header size

/* 2C */ "LABL"             // section magic
/* 30 */ "\x00\x00\x00\x0C" // section length
/* 34 */ "\x00\x00\x00\x00" // number of labels: 0

/* 38 */ ""s;

    istringstream stm(STM_CONTENT);
    EXPECT_THROW(sequence::make_sequence(stm), integrity_error);
}

/**
 * Tests that when the first section's length does not match that specified in
 * the file header, integrity_error is thrown.
 */
TEST(sequence, DATA_section_length_integrity_error)
{
    const auto STM_CONTENT =
/* 00 */ "RSEQ"             // file magic
/* 04 */ "\xFE\xFF"         // big endian BOM
/* 06 */ "\x01\x00"         // file version
/* 08 */ "\x00\x00\x00\x38" // file length
/* 0C */ "\x00\x20"         // file header length
/* 0E */ "\x00\x02"         // number of file sections
/* 10 */ "\x00\x00\x00\x20" // offset to DATA section
/* 14 */ "\x00\x00\x00\x0C" // length of DATA section
/* 18 */ "\x00\x00\x00\x2C" // offset to LABL section
/* 1C */ "\x00\x00\x00\x0C" // length of LABL section

/* 20 */ "DATA"             // section magic
/* 24 */ "\x00\x00\x00\xFF" // error - section length
/* 28 */ "\x00\x00\x00\x0C" // section header size

/* 2C */ "LABL"             // section magic
/* 30 */ "\x00\x00\x00\x0C" // section length
/* 34 */ "\x00\x00\x00\x00" // number of labels: 0

/* 38 */ ""s;

    istringstream stm(STM_CONTENT);
    EXPECT_THROW(sequence::make_sequence(stm), integrity_error);
}

/**
 * Tests that when the DATA section's header size is not 0x0C,
 * integrity_error is thrown.
 */
TEST(sequence, DATA_section_header_size_integrity_error)
{
    const auto STM_CONTENT =
/* 00 */ "RSEQ"             // file magic
/* 04 */ "\xFE\xFF"         // big endian BOM
/* 06 */ "\x01\x00"         // file version
/* 08 */ "\x00\x00\x00\x38" // file length
/* 0C */ "\x00\x20"         // file header length
/* 0E */ "\x00\x02"         // number of file sections
/* 10 */ "\x00\x00\x00\x20" // offset to DATA section
/* 14 */ "\x00\x00\x00\x0C" // length of DATA section
/* 18 */ "\x00\x00\x00\x2C" // offset to LABL section
/* 1C */ "\x00\x00\x00\x0C" // length of LABL section

/* 20 */ "DATA"             // section magic
/* 24 */ "\x00\x00\x00\x0C" // section length
/* 28 */ "\x00\x00\x00\xFF" // error - section header size

/* 2C */ "LABL"             // section magic
/* 30 */ "\x00\x00\x00\x0C" // section length
/* 34 */ "\x00\x00\x00\x00" // number of labels: 0

/* 38 */ ""s;

    istringstream stm(STM_CONTENT);
    EXPECT_THROW(sequence::make_sequence(stm), integrity_error);
}

/**
 * Tests that when the second section's magic is not LABL, integrity_error is thrown.
 */
TEST(sequence, LABL_section_magic_integrity_error)
{
    const auto STM_CONTENT =
/* 00 */ "RSEQ"             // file magic
/* 04 */ "\xFE\xFF"         // big endian BOM
/* 06 */ "\x01\x00"         // file version
/* 08 */ "\x00\x00\x00\x38" // file length
/* 0C */ "\x00\x20"         // file header length
/* 0E */ "\x00\x02"         // number of file sections
/* 10 */ "\x00\x00\x00\x20" // offset to DATA section
/* 14 */ "\x00\x00\x00\x0C" // length of DATA section
/* 18 */ "\x00\x00\x00\x2C" // offset to LABL section
/* 1C */ "\x00\x00\x00\x0C" // length of LABL section

/* 20 */ "DATA"             // section magic
/* 24 */ "\x00\x00\x00\x0C" // section length
/* 28 */ "\x00\x00\x00\x0C" // section header size

/* 2C */ "LABO"             // error - section magic
/* 30 */ "\x00\x00\x00\x0C" // section length
/* 34 */ "\x00\x00\x00\x00" // number of labels: 0

/* 38 */ ""s;

    istringstream stm(STM_CONTENT);
    EXPECT_THROW(sequence::make_sequence(stm), integrity_error);
}

/**
 * Tests that when the second section's length does not match that specified in
 * the file header, integrity_error is thrown.
 */
TEST(sequence, LABL_section_length_integrity_error)
{
    const auto STM_CONTENT =
/* 00 */ "RSEQ"             // file magic
/* 04 */ "\xFE\xFF"         // big endian BOM
/* 06 */ "\x01\x00"         // file version
/* 08 */ "\x00\x00\x00\x38" // file length
/* 0C */ "\x00\x20"         // file header length
/* 0E */ "\x00\x02"         // number of file sections
/* 10 */ "\x00\x00\x00\x20" // offset to DATA section
/* 14 */ "\x00\x00\x00\x0C" // length of DATA section
/* 18 */ "\x00\x00\x00\x2C" // offset to LABL section
/* 1C */ "\x00\x00\x00\x0C" // length of LABL section

/* 20 */ "DATA"             // section magic
/* 24 */ "\x00\x00\x00\x0C" // section length
/* 28 */ "\x00\x00\x00\x0C" // section header size

/* 2C */ "LABL"             // section magic
/* 30 */ "\x00\x00\x00\xFF" // error - section length
/* 34 */ "\x00\x00\x00\x00" // number of labels: 0

/* 38 */ ""s;

    istringstream stm(STM_CONTENT);
    EXPECT_THROW(sequence::make_sequence(stm), integrity_error);
}

/**
 * Tests that a well-formed RSEQ file does not throw integrity_error.
 */
TEST(sequence, no_integrity_error)
{
    const auto STM_CONTENT =
/* 00 */ "RSEQ"             // file magic
/* 04 */ "\xFE\xFF"         // big endian BOM
/* 06 */ "\x01\x00"         // file version
/* 08 */ "\x00\x00\x00\x38" // file length
/* 0C */ "\x00\x20"         // file header length
/* 0E */ "\x00\x02"         // number of file sections
/* 10 */ "\x00\x00\x00\x20" // offset to DATA section
/* 14 */ "\x00\x00\x00\x0C" // length of DATA section
/* 18 */ "\x00\x00\x00\x2C" // offset to LABL section
/* 1C */ "\x00\x00\x00\x0C" // length of LABL section

/* 20 */ "DATA"             // section magic
/* 24 */ "\x00\x00\x00\x0C" // section length
/* 28 */ "\x00\x00\x00\x0C" // section header size

/* 2C */ "LABL"             // section magic
/* 30 */ "\x00\x00\x00\x0C" // section length
/* 34 */ "\x00\x00\x00\x00" // number of labels: 0

/* 38 */ ""s;

    istringstream stm(STM_CONTENT);
    EXPECT_NO_THROW(sequence::make_sequence(stm));
}

/**
 * Tests the parsing of a valid RSEQ file with just one note_on event.
 */
TEST(sequence, one_event)
{
    const auto STM_CONTENT =
/* 00 */ "RSEQ"             // file magic
/* 04 */ "\xFE\xFF"         // big endian BOM
/* 06 */ "\x01\x00"         // file version
/* 08 */ "\x00\x00\x00\x3C" // file length
/* 0C */ "\x00\x20"         // file header length
/* 0E */ "\x00\x02"         // number of file sections
/* 10 */ "\x00\x00\x00\x20" // offset to DATA section
/* 14 */ "\x00\x00\x00\x0F" // length of DATA section
/* 18 */ "\x00\x00\x00\x30" // offset to LABL section
/* 1C */ "\x00\x00\x00\x0C" // length of LABL section

/* 20 */ "DATA"             // section magic
/* 24 */ "\x00\x00\x00\x0F" // section length
/* 28 */ "\x00\x00\x00\x0C" // section header size
/* 2C */ "\x4F\x29\x30"     // note: 0x4F, pitch: 0x29, velocity: 0x30

/* 2F */ "\x00"             // padding, unused

/* 30 */ "LABL"             // section magic
/* 34 */ "\x00\x00\x00\x0C" // section length
/* 38 */ "\x00\x00\x00\x00" // number of labels: 0
/* 3C */ ""s;

    istringstream stm(STM_CONTENT);
    const auto seq = sequence::make_sequence(stm);

    mock_sequence_visitor visitor;
    EXPECT_CALL(visitor, 
                visit(Matcher<const note_on&>(AllOf(Property(&note_on::note,     0x4F),
                                                    Property(&note_on::pitch,    0x29),
                                                    Property(&note_on::velocity, 0x30)
                                              ))));

    seq.traverse(visitor);
}

/**
 * Tests the parsing of a valid RSEQ file with multiple events.
 */
TEST(sequence, multiple_events)
{
    const auto STM_CONTENT =
/* 00 */ "RSEQ"                         // file magic
/* 04 */ "\xFE\xFF"                     // big endian BOM
/* 06 */ "\x01\x00"                     // file version
/* 08 */ "\x00\x00\x00\x5C"             // file length
/* 0C */ "\x00\x20"                     // file header length
/* 0E */ "\x00\x02"                     // number of file sections
/* 10 */ "\x00\x00\x00\x20"             // offset to DATA section
/* 14 */ "\x00\x00\x00\x23"             // length of DATA section
/* 18 */ "\x00\x00\x00\x50"             // offset to LABL section
/* 1C */ "\x00\x00\x00\x0C"             // length of LABL section

/* 20 */ "DATA"                         // section magic
/* 24 */ "\x00\x00\x00\x23"             // section length
/* 28 */ "\x00\x00\x00\x0C"             // section header size
/* 2C */ "\x80\x02"                     // wait: 0x02 ticks
/* 2E */ "\x88\x04\x00\x0E\x12"         // open track number: 0x04, offset to track: 0x0E12
/* 33 */ "\xA0\x3C\x7F\x00\xC8\x01\x90" // note: 0x3C, pitch: 0x7F, velocity: random [0x00C8, 0x0190)
/* 3A */ "\xF0\x80\x10\x00\x0C"         // set variable 0x10 = 0x000C
/* 3F */ "\xA1\xC6\x10"                 // priority: value in variable 0x10 == 0x0C
/* 42 */ "\xFF"                         // fin

/* 43 */ "\x00\x00\x00\x00\x00\x00\x00" // padding
/* 4A */ "\x00\x00\x00\x00\x00\x00"     // padding

/* 50 */ "LABL"                         // section magic
/* 54 */ "\x00\x00\x00\x0C"             // section length
/* 58 */ "\x00\x00\x00\x00"             // number of labels: 0
/* 5C */ ""s;

    istringstream stm(STM_CONTENT);
    const auto seq = sequence::make_sequence(stm);
    mock_sequence_visitor visitor;

    // specify memory for variables to use
    int16_t memory[48] = {};
    brtools::data::types::variable::memory = memory;

    {   // expectations in order
        InSequence s;
        using wait = brtools::data::sequence::wait;
        EXPECT_CALL(visitor, 
                    visit(Matcher<const wait&>(Property(&wait::tick_count, 0x02))));

        EXPECT_CALL(visitor, 
                    visit(Matcher<const open_track&>(AllOf(
                              Property(&open_track::track_no, 0x04),
                              Property(&open_track::offset_to_track_data, 0x0E12)
                          ))));

        using random = brtools::data::sequence::random;
        EXPECT_CALL(visitor, 
                    visit(Matcher<const random&>(AllOf(
                              Property(&random::lower_bound, 0x00C8),
                              Property(&random::upper_bound, 0x0190),
                              Property(&random::event, WhenDynamicCastTo<const note_on&>(AllOf(
                                  Property(&note_on::note, 0x3C),
                                  Property(&note_on::pitch, 0x7F),
                                  Property(&note_on::velocity, AllOf(Ge(0x00C8), Lt(0x0190)))
                              )))
                          ))));

        EXPECT_CALL(visitor,
                    visit(Matcher<const set_v&>(AllOf(
                              Property(&set_v::l_operand_var, 0x10),
                              Property(&set_v::r_operand_val, 0x000C)
                          ))));

        EXPECT_CALL(visitor,
                    visit(Matcher<const variable&>(AllOf(
                              Property(&variable::slot,  0x10),
                              Property(&variable::value, 0x000C),
                              Property(&variable::event, WhenDynamicCastTo<const priority&>(
                                  Property(&priority::priority_value, 0x0C)
                              ))
                          ))));

        EXPECT_CALL(visitor, visit(A<const fin&>()));
    }

    seq.traverse(visitor);
}
