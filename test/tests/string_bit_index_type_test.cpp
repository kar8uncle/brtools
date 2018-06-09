#include <gtest/gtest.h>

#include <trie/detail/string_bit_index_type.h>

#include <cstdint>      // uint16_t
#include <type_traits>  // integral_constant

using namespace ::testing;
using namespace brtools::trie::detail;
using namespace std;

/**
 * Tests that the public constants defined in the class are correct.
 *
 * EXPECT_TRUE must be used because the constants are compile-time
 * only and are not actualized, so otherwise we would get linker
 * error if we used EXPECT_EQ.
 */
TEST(bit_field_test, value_bounds_correctness)
{
    {   // Test 1: 2-bit bit field
        using field_type = bit_field<2, 0, uint16_t>;

        EXPECT_TRUE(2 == field_type::value_size);
        EXPECT_TRUE(0 == field_type::value_start);
        EXPECT_TRUE(2 == field_type::value_end);
    }

    {   // Test 2: 0-bit bit field
        using field_type = bit_field<0, 0, uint16_t>;

        EXPECT_TRUE(0 == field_type::value_size);
        EXPECT_TRUE(0 == field_type::value_start);
        EXPECT_TRUE(0 == field_type::value_end);
    }

    {   // Test 3: 8-bit bit field
        using field_type = bit_field<8, 0, uint8_t>;

        EXPECT_TRUE(8 == field_type::value_size);
        EXPECT_TRUE(0 == field_type::value_start);
        EXPECT_TRUE(8 == field_type::value_end);
    }

    {   // Test 4: 7-bit bit field with 1-bit shift
        using field_type = bit_field<7, 1, uint8_t>;

        EXPECT_TRUE(7 == field_type::value_size);
        EXPECT_TRUE(1 == field_type::value_start);
        EXPECT_TRUE(8 == field_type::value_end);
    }

    {   // Test 5: 0-bit bit field with 16-bit shift
        using field_type = bit_field<0, 16, uint16_t>;

        EXPECT_TRUE(0 == field_type::value_size);
        EXPECT_TRUE(16 == field_type::value_start);
        EXPECT_TRUE(16 == field_type::value_end);
    }
}

/**
 * Tests that the masks to truncate and clear bits are correct.
 */
TEST(bit_field_test, masks_correctness)
{
    {   // Test 1: 2-bit bit field
        using field_type = bit_field<2, 0, uint16_t>;

        EXPECT_TRUE(16 == field_type::bits_in_raw_value);
        EXPECT_TRUE(0b0000'0000'0000'0011 == field_type::truncate_mask);
        EXPECT_TRUE(0b1111'1111'1111'1100 == field_type::clear_mask);
    }

    {   // Test 2: 0-bit bit field
        using field_type = bit_field<0, 0, uint16_t>;

        EXPECT_TRUE(16 == field_type::bits_in_raw_value);
        EXPECT_TRUE(0b0000'0000'0000'0000 == field_type::truncate_mask);
        EXPECT_TRUE(0b1111'1111'1111'1111 == field_type::clear_mask);
    }

    {   // Test 3: 8-bit bit field
        using field_type = bit_field<8, 0, uint8_t>;

        EXPECT_TRUE(8 == field_type::bits_in_raw_value);
        EXPECT_TRUE(0b1111'1111 == field_type::truncate_mask);
        EXPECT_TRUE(0b0000'0000 == field_type::clear_mask);
    }

    {   // Test 4: 7-bit bit field with 1-bit shift
        using field_type = bit_field<7, 1, uint8_t>;

        EXPECT_TRUE(8 == field_type::bits_in_raw_value);
        EXPECT_TRUE(0b011'11111 == field_type::truncate_mask);
        EXPECT_TRUE(0b000'00001 == field_type::clear_mask);
    }

    {   // Test 5: 0-bit bit field with 16-bit shift
        using field_type = bit_field<0, 16, uint16_t>;

        EXPECT_TRUE(16 == field_type::bits_in_raw_value);
        EXPECT_TRUE(0b0000'0000'0000'0000 == field_type::truncate_mask);
        EXPECT_TRUE(0b1111'1111'1111'1111 == field_type::clear_mask);
    }
}

/**
 * Tests that the bit field access and assignment operators behave correctly
 * when there is only one bit field reading from and writing to a packed value.
 */
TEST(bit_field_test, single_bit_field_behavior)
{
    {   // Test 1: 2-bit bit field
        uint16_t packed = 0b11;
        bit_field<2, 0, uint16_t> field(packed);
        EXPECT_EQ(0b11, field);
        
        field = 0b10;
        EXPECT_EQ(0b10, field);
    }

    {   // Test 2: 2-bit bit field, verify assignment truncation
        uint16_t packed = 0b00;
        bit_field<2, 0, uint16_t> field(packed);
        EXPECT_EQ(0b00, field);
        
        field = 0b1110;
        EXPECT_EQ(0b10, field);
    }
}

/**
 * Tests that the bit field access and assignment operators behave correctly
 * when there are more than one bit fields reading from and writing to a
 * single packed value.
 */
TEST(bit_field_test, multiple_bit_field_behavior)
{
    {   // Test 1: 2-bit, 4-bit bit fields
        uint8_t packed = 0b1100'10;
        bit_field<2, 0, uint8_t> field1(packed);
        bit_field<4, 2, uint8_t> field2(packed);

        EXPECT_EQ(0b10, field1);
        EXPECT_EQ(0b1100, field2);
        EXPECT_EQ(0b1100'10, packed);
        
        field1 = 0b11;
        EXPECT_EQ(0b11, field1);
        EXPECT_EQ(0b1100, field2);
        EXPECT_EQ(0b1100'11, packed);

        field2 = 0b0000;
        EXPECT_EQ(0b11, field1);
        EXPECT_EQ(0b0000, field2);
        EXPECT_EQ(0b0000'11, packed);
    }

    {   // Test 2: 2-bit, 4-bit bit fields with unused bits in between
        uint8_t packed = 0b1111'00'11;
        bit_field<2, 0, uint8_t> field1(packed);
        bit_field<4, 4, uint8_t> field2(packed);
        
        EXPECT_EQ(0b11, field1);
        EXPECT_EQ(0b1111, field2);
        EXPECT_EQ(0b1111'00'11, packed);
        
        field1 = 0b01;
        EXPECT_EQ(0b01, field1);
        EXPECT_EQ(0b1111, field2);
        EXPECT_EQ(0b1111'00'01, packed);

        field2 = 0b0010;
        EXPECT_EQ(0b01, field1);
        EXPECT_EQ(0b0010, field2);
        EXPECT_EQ(0b0010'00'01, packed);

        // truncation test
        field1 = 0b000011'11;
        EXPECT_EQ(0b11, field1);
        EXPECT_EQ(0b0010, field2);
        EXPECT_EQ(0b0010'00'11, packed);
    }

    {   // Test 3: 2-bit, 4-bit bit fields with an overlapping bit
        uint8_t packed = 0b111'1'0;
        bit_field<2, 0, uint8_t> field1(packed);
        bit_field<4, 1, uint8_t> field2(packed);
        
        EXPECT_EQ(0b1'0, field1);
        EXPECT_EQ(0b111'1, field2);
        EXPECT_EQ(0b111'1'0, packed);
        
        field1 = 0b0'1;
        EXPECT_EQ(0b0'1, field1);
        EXPECT_EQ(0b111'0, field2);
        EXPECT_EQ(0b111'0'1, packed);

        field2 = 0b001'1;
        EXPECT_EQ(0b1'1, field1);
        EXPECT_EQ(0b001'1, field2);
        EXPECT_EQ(0b001'1'1, packed);

        // truncation test
        field1 = 0b000'0'0;
        EXPECT_EQ(0b0'0, field1);
        EXPECT_EQ(0b001'0, field2);
        EXPECT_EQ(0b001'0'0, packed);
    }
}

/**
 * Tests the operator overloads of string_bit_index_test class.
 */
TEST(string_bit_index_test, operators)
{
    using idx_type = string_bit_index_type<uint32_t>;
    {   // Test 1: equality (==)
        EXPECT_TRUE(idx_type{1u} == idx_type{1u}); 
        EXPECT_TRUE(idx_type{0u} == idx_type{0u}); 
        EXPECT_TRUE(idx_type{} == idx_type{}); 
        EXPECT_TRUE(idx_type{42u} == idx_type{42u}); 
        EXPECT_TRUE(idx_type{0xDEADBEEFu} == idx_type{0xDEADBEEFu}); 
    }

    {   // Test 2: inequality (!=)
        EXPECT_TRUE(idx_type{1u} != idx_type{2u}); 
        EXPECT_TRUE(idx_type{0u} != idx_type{1u}); 
        EXPECT_TRUE(idx_type{idx_type::max} != idx_type{idx_type::max - 1}); 
        EXPECT_TRUE(idx_type{42u} != idx_type{24u}); 
        EXPECT_TRUE(idx_type{0xDEADBEEFu} != idx_type{0xCAFEBABEu}); 
    }

    {   // Test 3: less than (<, <=)
        EXPECT_TRUE(idx_type{1u} <  idx_type{2u}); 
        EXPECT_TRUE(idx_type{1u} <= idx_type{2u}); 
        EXPECT_TRUE(idx_type{1u} <= idx_type{1u}); 

        EXPECT_TRUE(idx_type{0u} <  idx_type{1u}); 
        EXPECT_TRUE(idx_type{0u} <= idx_type{1u}); 
        EXPECT_TRUE(idx_type{0u} <= idx_type{0u}); 

        EXPECT_TRUE(idx_type{idx_type::max - 1} < idx_type{idx_type::max}); 
        EXPECT_TRUE(idx_type{idx_type::max - 1} <= idx_type{idx_type::max}); 
        EXPECT_TRUE(idx_type{idx_type::max} <= idx_type{idx_type::max}); 

        EXPECT_TRUE(idx_type{24u} <  idx_type{42u}); 
        EXPECT_TRUE(idx_type{24u} <= idx_type{42u}); 
        EXPECT_TRUE(idx_type{24u} <= idx_type{24u}); 

        EXPECT_TRUE(idx_type{0xCAFEBABEu} <  idx_type{0xDEADBEEFu}); 
        EXPECT_TRUE(idx_type{0xCAFEBABEu} <= idx_type{0xDEADBEEFu}); 
        EXPECT_TRUE(idx_type{0xCAFEBABEu} <= idx_type{0xCAFEBABEu}); 
    }

    {   // Test 4: greater than (>, >=)
        EXPECT_TRUE(idx_type{2u} >  idx_type{1u}); 
        EXPECT_TRUE(idx_type{2u} >= idx_type{1u}); 
        EXPECT_TRUE(idx_type{2u} >= idx_type{2u}); 

        EXPECT_TRUE(idx_type{1u} >  idx_type{0u}); 
        EXPECT_TRUE(idx_type{1u} >= idx_type{0u}); 
        EXPECT_TRUE(idx_type{1u} >= idx_type{1u}); 

        EXPECT_TRUE(idx_type{idx_type::max} >  idx_type{idx_type::max - 1}); 
        EXPECT_TRUE(idx_type{idx_type::max} >= idx_type{idx_type::max - 1}); 
        EXPECT_TRUE(idx_type{idx_type::max} >= idx_type{idx_type::max}); 

        EXPECT_TRUE(idx_type{42u} >  idx_type{24u}); 
        EXPECT_TRUE(idx_type{42u} >= idx_type{24u}); 
        EXPECT_TRUE(idx_type{42u} >= idx_type{42u}); 

        EXPECT_TRUE(idx_type{0xDEADBEEFu} >  idx_type{0xCAFEBABEu}); 
        EXPECT_TRUE(idx_type{0xDEADBEEFu} >= idx_type{0xCAFEBABEu}); 
        EXPECT_TRUE(idx_type{0xDEADBEEFu} >= idx_type{0xDEADBEEFu}); 
    }                                                   

    {   // Test 5: assignment (=)
        idx_type aux;
        EXPECT_TRUE(&(aux = idx_type{1u}) == &aux); 
        EXPECT_TRUE(aux == idx_type{1u}); 

        EXPECT_TRUE(&(aux = idx_type{0u}) == &aux); 
        EXPECT_TRUE(aux == idx_type{0u}); 

        EXPECT_TRUE(&(aux = idx_type{}) == &aux); 
        EXPECT_TRUE(aux == idx_type{}); 

        EXPECT_TRUE(&(aux = idx_type{42u}) == &aux); 
        EXPECT_TRUE(aux == idx_type{42u}); 

        EXPECT_TRUE(&(aux = idx_type{0xDEADBEEFu}) == &aux); 
        EXPECT_TRUE(aux == idx_type{0xDEADBEEFu}); 
    }

    {   // Test 6: addition (+, ++, +=)
        idx_type aux = 0u;
        EXPECT_TRUE(&(aux += idx_type{1u}) == &aux); 
        EXPECT_TRUE(aux == idx_type{1u}); 

        EXPECT_TRUE(&(++aux) == &aux); 
        EXPECT_TRUE(aux == idx_type{2u}); 

        EXPECT_TRUE(idx_type{aux} == aux++); 
        EXPECT_TRUE(aux == idx_type{3u}); 

        aux = idx_type::max;
        EXPECT_TRUE(aux + idx_type{1u} == idx_type::max); 
        EXPECT_TRUE((aux += idx_type{42u}) == idx_type::max); 

        aux = idx_type::max - 5u; 
        EXPECT_TRUE(aux + idx_type{10u} == idx_type::max); 
    }

    {   // Test 7: subtraction (-, --, -=)
        idx_type aux = 10u;
        EXPECT_TRUE(&(aux -= idx_type{1u}) == &aux); 
        EXPECT_TRUE(aux == idx_type{9u}); 

        EXPECT_TRUE(&(--aux) == &aux); 
        EXPECT_TRUE(aux == idx_type{8u}); 

        EXPECT_TRUE(idx_type{aux} == aux--); 
        EXPECT_TRUE(aux == idx_type{7u}); 

        aux = 2u;
        EXPECT_TRUE(aux - idx_type{2u} == idx_type{0u}); 
        EXPECT_TRUE(aux - idx_type{5u} == idx_type{0u}); 
        EXPECT_TRUE((aux -= idx_type{10u}) == idx_type{0u}); 
    }
}

/**
 * Tests that the subscript operator to index bits in a string yields the
 * expected bit. Indexing past the end if string yields 0.
 */
TEST(string_bit_index_test, indexing)
{
    using idx_type = string_bit_index_type<uint16_t>;

    string s;
    s.push_back(static_cast<char>(0b10010000));
    s.push_back(static_cast<char>(0b01011111));

    EXPECT_EQ(1, idx_type{0}[s]);   EXPECT_EQ(0, idx_type{8}[s]);
    EXPECT_EQ(0, idx_type{1}[s]);   EXPECT_EQ(1, idx_type{9}[s]);
    EXPECT_EQ(0, idx_type{2}[s]);   EXPECT_EQ(0, idx_type{10}[s]);
    EXPECT_EQ(1, idx_type{3}[s]);   EXPECT_EQ(1, idx_type{11}[s]);
    EXPECT_EQ(0, idx_type{4}[s]);   EXPECT_EQ(1, idx_type{12}[s]);
    EXPECT_EQ(0, idx_type{5}[s]);   EXPECT_EQ(1, idx_type{13}[s]);
    EXPECT_EQ(0, idx_type{6}[s]);   EXPECT_EQ(1, idx_type{14}[s]);
    EXPECT_EQ(0, idx_type{7}[s]);   EXPECT_EQ(1, idx_type{15}[s]);

    // past end of string, should always return 0
    EXPECT_EQ(0, idx_type{16}[s]);  EXPECT_EQ(0, idx_type{24}[s]);
    EXPECT_EQ(0, idx_type{17}[s]);  EXPECT_EQ(0, idx_type{25}[s]);
    EXPECT_EQ(0, idx_type{18}[s]);  EXPECT_EQ(0, idx_type{26}[s]);
    EXPECT_EQ(0, idx_type{19}[s]);  EXPECT_EQ(0, idx_type{27}[s]);
    EXPECT_EQ(0, idx_type{20}[s]);  EXPECT_EQ(0, idx_type{28}[s]);
    EXPECT_EQ(0, idx_type{21}[s]);  EXPECT_EQ(0, idx_type{29}[s]);
    EXPECT_EQ(0, idx_type{22}[s]);  EXPECT_EQ(0, idx_type{30}[s]);
    EXPECT_EQ(0, idx_type{23}[s]);  EXPECT_EQ(0, idx_type{31}[s]);
}
