#ifndef BRTOOLS_DATA_SEQUENCE_RESOLVABLE_H
#define BRTOOLS_DATA_SEQUENCE_RESOLVABLE_H
#pragma once

#include <cassert>
#include <memory>       // std::unique_ptr
#include <iosfwd>       // std::streamoff
#include <functional>   // std::function
#include <data/sequence/sequence.h>

namespace brtools
{
namespace data
{
namespace sequence
{
    struct resolvable
    {
        using resolver = std::function<sequence::event_const_iterator(std::streamoff)>;
        virtual std::unique_ptr<event> resolve(const resolver&) const = 0;
    };
}
}
}

#endif
