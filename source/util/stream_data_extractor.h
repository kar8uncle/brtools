#ifndef BRTOOLS_UTIL_STREAM_DATA_EXTRACTOR_H
#define BRTOOLS_UTIL_STREAM_DATA_EXTRACTOR_H
#pragma once

#include <tuple>
#include <io/stream_parser.h>

namespace brtools
{
namespace util
{
    /**
     * Convenience class to extract stream data according to data types given.
     * Extracted data are stored in a tuple accessible from subclasses.
     *
     * @tparam ParamTypes...
     *         Types of paramters to extract from stream. Stream operator of
     *         stream_parser(operator>>) must support all these types.
     */
    template<typename... ParamTypes>
    struct stream_data_extractor
    {
        /**
         * Populates _m_arguments with data extracted from stream. Reads values
         * from the given stream_parser.
         */
        stream_data_extractor(io::stream_parser& sp)
        : _m_arguments(sp.read<ParamTypes>()...)
        {}

        // for subclass and outsiders to conveniently refer to its parent(this) class
        using extractor_type = stream_data_extractor<ParamTypes...>;

        std::tuple<ParamTypes...> _m_arguments;
    };

    /**
     * Convenience binder to change the last stream data type to the given BindType.
     *
     * Useful for variable, random events.
     */ 
    template<typename Extractor, typename BindType>
    using type_bind = void;

    template<typename... HeadTypes, typename TailType, typename BindType>
    using type_bind<stream_data_extractor<HeadTypes..., TailType>, BindType> = stream_data_extractor<HeadTypes..., BindType>;
}
}
#endif

