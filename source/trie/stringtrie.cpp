#include <trie/stringtrie.h>
#include "detail/string_bit_index_type.h"

#include <functional>   // reference_wrapper, ref
#include <limits>       // numeric_limits
#include <tuple>        // tuple, tie, ignore
#include <stdexcept>    // logic_error, invalid_argument, out_of_range
#include <deque>

using namespace brtools::trie;
using namespace brtools::trie::detail;
using namespace std;

using diff_index_type = string_bit_index_type<uint16_t>;

namespace brtools
{
namespace trie
{
namespace detail
{
    struct trie_node
    {
        enum flag_type : uint16_t
        { 
            branch_node = 0u,
            leaf_node   = 1u,
        } flag;

        diff_index_type first_diff_bit_index;

        unique_ptr<trie_node> left;
        unique_ptr<trie_node> right;

        stringtrie::string_id_type id;
    };
}
}
}

namespace
{
    /**
     * Zips two strings for iterating through characters in the two strings at
     * the same time. If one string is longer than the other, and the iterator
     * has exausted all characters in the shorter string, then the NUL character
     * is returned for the shorter string.
     */
    class string_zip
    {
    public:
        class iterator
        {
        public:
            using difference_type   = size_t;
            using value_type        = tuple<char, char>;
            using reference         = value_type&;
            using pointer           = value_type*;
            using iterator_category = input_iterator_tag;
        
        public:
            iterator(const string::const_iterator& iter1, 
                     const string::const_iterator& iter1_end,
                     const string::const_iterator& iter2,
                     const string::const_iterator& iter2_end
                     )
            : _m_iter1(iter1), _m_end1(iter1_end)
            , _m_iter2(iter2), _m_end2(iter2_end) {}
            
            iterator& operator++()
            { return _m_iter1 == _m_end1 ? _m_end1 : ++_m_iter1, 
                     _m_iter2 == _m_end2 ? _m_end2 : ++_m_iter2, 
                     *this; }

            iterator  operator++(int)
            { return iterator(_m_iter1 == _m_end1 ? _m_end1 : _m_iter1++, _m_end1,
                              _m_iter2 == _m_end2 ? _m_end2 : _m_iter2++, _m_end2); }

            bool operator==(const iterator& other) const
            { return _m_iter1 == other._m_iter1 && _m_iter2 == other._m_iter2; }

            bool operator!=(const iterator& other) const
            { return !(*this == other); }

            value_type operator*() const
            { return make_tuple(
                        _m_iter1 == _m_end1 ? '\0' : *_m_iter1,
                        _m_iter2 == _m_end2 ? '\0' : *_m_iter2
                     ); }
        private:
            string::const_iterator _m_iter1, _m_end1;
            string::const_iterator _m_iter2, _m_end2;
        };

    public:
        string_zip(const string& str1, const string& str2)
        : _m_str1(str1), _m_str2(str2) {}

        iterator begin() { return iterator(_m_str1.cbegin(), _m_str1.cend(), 
                                           _m_str2.cbegin(), _m_str2.cend()); }
        iterator end() { return iterator(_m_str1.cend(), _m_str1.cend(),
                                         _m_str2.cend(), _m_str2.cend()); }
        
    private:
        const string& _m_str1;
        const string& _m_str2;
    };


    /**
     * Finds the index of first occurrence of 1 in the given argument. Behavior
     * is only defined if num is not 0, and _Tp is an unsigned integral type.
     */
    template<typename _Tp>
    constexpr size_t count_leading_zeros(const _Tp num)
    {
        return 1 == num >> (numeric_limits<_Tp>::digits - 1) ? 0 : 1 + count_leading_zeros<_Tp>(num << 1);
    }

    /**
     * Finds the bit index of the first bit where the two given strings differ.
     *
     * If the length of the two strings differ, then the diff bit is the first
     * 1 bit in the longer string.
     *
     * If the two strings are completely identical, then there is no diff bit
     * and the result is the max value.
     *
     * If either str1 or str2 or both contain the NUL char, the behavior is
     * not defined.
     */
    auto find_first_diff_bit_index(const string& str1, const string& str2)
    {
        diff_index_type result;
        result.char_index = 0;
        for (const auto& chars : string_zip(str1, str2))
        {
            if (const diff_index_type::index_type diff = get<0>(chars) xor get<1>(chars))
            {
                result.bit_index = count_leading_zeros(diff);
                return result;
            }
            result.char_index = result.char_index + 1;
        }
        result.index = diff_index_type::max;

        return result;
    }

    /**
     * Gets the next node to traverse to given the string and the node.
     * If the bit in string at bit_index is 0, the left node is chosen,
     * otherwise the right node is chosen.
     *
     * @param  node Must not be nullptr and must not be a leaf.
     * @return tie(next, other) next  is the chosen node by the algorithm,
     *                          other is the node not chosen. 
     */
    template<typename _TrieNodeType>
    auto next_node(const string& str, _TrieNodeType& node)
    {
        if (node->first_diff_bit_index[str] == 0)
        {
            return tie(node->left, node->right);
        }
        else
        {
            return tie(node->right, node->left);
        }
    }

    /**
     * Retrieves the path to a leaf node in the stringtrie.
     *
     * @param root Root of the trie or subtrie. Must not be nullptr.
     */
    template<typename _TrieNodeType>
    auto find_path_to_leaf(const string& str, _TrieNodeType& root)
    {
        auto node = ref(root);
        deque<decltype(node)> path;

        while (static_cast<decltype(root)>(node)->flag != trie_node::leaf_node)
        {
            path.push_back(node);
            tie(node, ignore) = next_node(str, static_cast<_TrieNodeType&>(node));
        }

        path.push_back(node);

        return path;
    }
}


stringtrie::stringtrie() = default;
stringtrie::~stringtrie() = default;

stringtrie::stringtrie(const initializer_list<string>& strs)
{
    for (const auto& str : strs)
    {
        insert(str);
    }
}

stringtrie::string_id_type stringtrie::insert(string str)
{
    using node_ref = unique_ptr<trie_node>&;

    auto new_leaf = make_unique<trie_node>();
    new_leaf->flag = trie_node::leaf_node;

    if (!_m_root)
    {
        _m_root = move(new_leaf);
        _m_strings[_m_root->id = _m_id_dispenser.dispense()] = move(str);
        return _m_root->id;
    }
    else
    {
        const auto& path_to_leaf   = find_path_to_leaf(str, _m_root);
        auto new_branch = make_unique<trie_node>();
        new_branch->flag = trie_node::branch_node;
        new_branch->id   = invalid;

        {   // determine if the string already exists
            const auto& diff_with_leaf = static_cast<node_ref>(path_to_leaf.back());
            const auto& diff_with_str  = _m_strings[diff_with_leaf->id];

            if ((new_branch->first_diff_bit_index = find_first_diff_bit_index(str, diff_with_str))
                 == diff_index_type::max)
            {
                // two strings are identical,
                // no new string inserted
                return diff_with_leaf->id;
            }
        }

        // string is not in trie yet, find on the path where to add it
        const auto id = new_leaf->id = _m_id_dispenser.dispense();

        for (auto& pn : path_to_leaf)
        {
            auto& path_node = static_cast<node_ref>(pn);
            if (path_node->flag == trie_node::leaf_node ||
                // nodes with smaller first_diff_bit_index should be closer to root
                new_branch->first_diff_bit_index < path_node->first_diff_bit_index)
            {
                // this means: [ next, other ] = [ new_leaf, path_node ]
                // points the next node to the new leaf,
                // points the other node to the existing branch.
                next_node(str, new_branch) = forward_as_tuple(move(new_leaf), move(path_node));
                // points path_node to the new branch 
                path_node = move(new_branch);
                _m_strings[id] = move(str);
                return id;
            }
        }

        throw logic_error("This should be unreachable.");
    }
}

stringtrie::string_id_type stringtrie::find_id(const string& str) const
{
    if (!_m_root) 
    {
        return invalid;
    }
    else
    {
        return static_cast<const unique_ptr<trie_node>&>(find_path_to_leaf(str, _m_root).back())->id;
    }
}

string stringtrie::find_string(const string_id_type str_id) const
{
    if (str_id == invalid)
    {
        throw invalid_argument("The given string id is invalid.");
    }
    else 
    {
        const auto it = _m_strings.find(str_id);
        if (it != _m_strings.end())
        {
            return it->second;
        }
        else
        {
            throw out_of_range("The given string id does not correspond to any string.");
        }
    }
}
