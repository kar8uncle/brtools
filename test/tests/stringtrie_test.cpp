#include <gtest/gtest.h>

#include <brtools/trie/stringtrie.h>
#include <initializer_list>
#include <string>
#include <stdexcept>

using namespace ::testing;
using namespace brtools::trie;
using namespace std;

TEST(stringtrie_test, insert_and_find_id)
{
    stringtrie st;
    {   // Test 1: simple string
        const string str = "foobar";
        const auto id = st.insert(str);
        EXPECT_EQ(id,  st.find_id(str));
        EXPECT_EQ(str, st.find_string(id));
    }

    {   // Test 2: similar simple string
        const string str = "foobar      ";
        const auto id = st.insert(str);
        EXPECT_EQ(id,  st.find_id(str));
        EXPECT_EQ(str, st.find_string(id));
        EXPECT_NE(id,  st.find_id("foobar"));
    }

    {   // Test 3: empty string
        const string str = "";
        const auto id = st.insert(str);
        EXPECT_EQ(id,  st.find_id(str));
        EXPECT_EQ(str, st.find_string(id));
    }

    {   // Test 4: inserting a duplicate
        const string str = "same";
        const auto id = st.insert(str);
        EXPECT_EQ(id, st.insert(str));
    }
}

TEST(stringtrie_test, initializer_list_constructor)
{
    const initializer_list<string>& init_list = {"foo", "bar", 
                                                 "foobar", "barfoo", 
                                                 "foobarfoo", "barfoobar"};
    stringtrie st(init_list);
    for (const auto& str : init_list)
    {
        EXPECT_EQ(str, st.find_string(st.find_id(str)));
    }
}

TEST(stringtrie_test, unsuccessful_finds)
{
    stringtrie st;

    {   // Test 1: exceptions thrown from find_string
        EXPECT_THROW(st.find_string(stringtrie::invalid), invalid_argument);
        EXPECT_THROW(st.find_string(0), out_of_range);
    }

    {   // Test 2: invalid string id returned from find_id
        EXPECT_EQ(stringtrie::invalid, st.find_id("foo"));
        EXPECT_EQ(stringtrie::invalid, st.find_id("bar"));
        EXPECT_EQ(stringtrie::invalid, st.find_id(""));
    }
}

