#ifndef BRTOOLS_DATA_SEQUENCE_SEQUENCE_H
#define BRTOOLS_DATA_SEQUENCE_SEQUENCE_H
#pragma once

#include <memory>   // for unique_ptr
#include <iosfwd>   // for forward-declaration of istream
#include <string>
#include <list>
#include <unordered_map>
#include <brtools/util/unit_testable.h>
#include <brtools/data/playable.h>

namespace brtools
{
namespace data
{
namespace sequence
{
    struct visitor;
    struct event;

    /**
     * Represents sequenced music containing events.
     *
     * TODO: make sequence satisfy Container requirement, to support editing.
     */
    class sequence : public playable
    {
    public:
        using event_container = std::list<std::unique_ptr<event>>;
        using event_const_iterator = event_container::const_iterator;
        using event_iterator       = event_container::iterator;

        static sequence make_sequence(std::istream&);
        void traverse(visitor&) const;

    private:
        using label_event_map = std::unordered_map<std::string, event_const_iterator>;

        event_container _m_events;
        label_event_map _m_label_2_event;
    };
}
}
}
#endif
