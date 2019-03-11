#ifndef BRTOOLS_DATA_SEQUENCE_SEQUENCE_H
#define BRTOOLS_DATA_SEQUENCE_SEQUENCE_H
#pragma once

#include <memory>   // for unique_ptr
#include <iosfwd>   // for forward-declaration of istream
#include <vector>
#include <unordered_map>
#include <util/unit_testable.h>

namespace brtools
{
namespace data
{
namespace sequence
{
    class event;
    class visitor;

    /**
     * Represents sequenced music containing events.
     *
     * TODO: make sequence satisfy Container requirement, to support editing.
     */
    class sequence
    {
        BRTOOLS_UNIT_TESTABLE
    public:
        static sequence make_sequence(std::istream&);
        void traverse(visitor&) const;
    private:
        std::vector<std::unique_ptr<event>>     _m_events;
        std::unordered_map<std::string, size_t> _m_labels;
    };
}
}
}
#endif
