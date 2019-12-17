#ifndef BRTOOLS_DATA_SEQUENCE_VISITOR_ACCEPTABLE_H
#define BRTOOLS_DATA_SEQUENCE_VISITOR_ACCEPTABLE_H
#pragma once

#include <brtools/data/sequence/sequence.h>
#include <brtools/data/sequence/visitor.h>

namespace brtools
{
namespace data
{
namespace sequence
{
    /**
     * Inherited by the generic event class. Interface to accept a visitor.
     * Intended to be virtually inherited.
     */
    struct visitor_acceptable
    {
        virtual void accept(visitor&, const visitor::operation&) const = 0;

    protected:
        ~visitor_acceptable() = default;
    };
}
}
}

#endif

