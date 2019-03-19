#ifndef BRTOOLS_DATA_SEQUENCE_VISITABLE_H
#define BRTOOLS_DATA_SEQUENCE_VISITABLE_H
#pragma once

#include <data/sequence/visitor.h>
#include <data/sequence/visitor_acceptable.h>

namespace brtools
{
namespace data
{
namespace sequence
{
    /**
     * Inherited by the specialized event classes. Implements virtual methods
     * of event_visitor_acceptable.
     */
    template<typename VisitableType>
    struct visitable : public virtual visitor_acceptable
    {
        void accept(visitor& visitor, const visitor::operation& op) const override
        {   visitor.visit(static_cast<const VisitableType&>(*this), op);  }
    };
}
}
}

#endif

