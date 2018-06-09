#ifndef BRTOOLS_TRIE_STRINGTRIE_H
#define BRTOOLS_TRIE_STRINGTRIE_H
#pragma once

#include <unordered_map>
#include <memory>       // std::unique_ptr
#include <string>
#include <cstdint>      // uint32_t
#include <initializer_list>

#include <util/id_dispenser.h>

namespace brtools
{
    namespace io
    {
    namespace detail
    {
        class stream_parser;
    }
    }

    namespace trie
    {
        namespace detail
        {
            struct trie_node;
        }

        class stringtrie
        {
            friend io::detail::stream_parser& operator>>(io::detail::stream_parser&, stringtrie&);

        public:
            using string_id_type = uint32_t;
            enum : string_id_type
            { invalid = static_cast<uint32_t>(~0u) };

        public:
            stringtrie();
            ~stringtrie();
            /**
             * Initializes a trie with the given strings in the initializer list.
             */
            stringtrie(const std::initializer_list<std::string>&);

        public:
            /**
             * Inserts a string into this trie. If the string already exists in
             * the trie, then the id associated previously with the string is
             * returned, and no new string will be inserted.
             *
             * @return The id associated with the string.
             */
            string_id_type insert(std::string);

            /**
             * Retrieves the id of the given string. If the string is not in
             * the trie, the invalid id is returned.
             */
            string_id_type find_id(const std::string&) const;

            /**
             * Retrieves the string associated with the given id.
             *
             * @throws std::out_of_range if the id is not associated with any
             *                           string in the trie.
             * @throws std::invalid_argument if the given id is an invalid id.
             */
            std::string find_string(string_id_type) const;

        private:
            std::unique_ptr<detail::trie_node>              _m_root;
            std::unordered_map<string_id_type, std::string> _m_strings;
            util::id_dispenser<string_id_type>              _m_id_dispenser;
        };
    }

}

#endif
