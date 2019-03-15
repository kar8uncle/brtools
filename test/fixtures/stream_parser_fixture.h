#ifndef BRTOOLS_TEST_FIXTURES_STREAM_PARSER_FIXTURE_H
#define BRTOOLS_TEST_FIXTURES_STREAM_PARSER_FIXTURE_H
#pragma once

#include <gtest/gtest.h>
#include <io/stream_parser.h>

#include <sstream>  // std::istringstream
#include <utility>  // std::forward
#include <string>   // std::string literal

class stream_parser_fixture : public ::testing::Test
{
protected:
    template<typename... ArgTypes>
    brtools::io::stream_parser parser(ArgTypes&&... args)
    {
        brtools::io::stream_parser sp(std::forward<ArgTypes>(args)...);

        // We don't know the endianness of the machine, so it is imperative that we
        // check if we need to reverse the byte order in order to run the tests in
        // a deterministic fashion. For readability, let's use big endian.
        {
            using namespace std::string_literals;

            const auto BYTE_ORDER_MARK = "\xFF\xFE"s;
            std::istringstream stm(BYTE_ORDER_MARK);
            if (brtools::io::stream_parser(stm).read<uint16_t>() != 0xFFFE)
            {
                sp.reverse_byte_order();
            }
        }
        
        return sp;
    }
};

#endif
