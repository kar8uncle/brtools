#ifndef BRTOOLS_DATA_SEQUENCE_EVENT_EVENTS_IMPL_H
#define BRTOOLS_DATA_SEQUENCE_EVENT_EVENTS_IMPL_H
#pragma once

#include <brtools/util/stream_data_extractor.h>
#include <brtools/data/sequence/events.h>
#include <brtools/data/sequence/resolvable.h>
#include <brtools/data/types/uint.h>
#include <brtools/data/types/varint.h>
#include <brtools/data/types/random.h>
#include <brtools/data/types/variable.h>

#include <utility>  // std::declval
#include <type_traits> // std::is_signed, std::conditional

namespace brtools
{
namespace data
{
namespace sequence
{
    template<typename Extractor = util::stream_data_extractor<uint8_t, uint8_t, types::basic_varint<uint32_t>>>
    struct note_on_impl : note_on
    , public Extractor
    {
        template<typename E> using self = note_on_impl<E>;
        using Extractor::Extractor;

        uint8_t note() const override
        {    return std::get<0>(this->_m_arguments); }

        uint8_t pitch() const override
        {    return std::get<1>(this->_m_arguments); }

        uint32_t velocity() const override
        {    return std::get<2>(this->_m_arguments); }
    };

    template<typename Extractor = util::stream_data_extractor<types::basic_varint<uint32_t>>>
    struct wait_impl : wait
    , public Extractor
    {
        template<typename E> using self = wait_impl<E>;
        using Extractor::Extractor;

        uint32_t tick_count() const override
        {   return std::get<0>(this->_m_arguments); }
    };

    template<typename Extractor = util::stream_data_extractor<types::basic_varint<uint32_t>>>
    struct program_impl : program
    , public Extractor
    {
        template<typename E> using self = program_impl<E>;
        using Extractor::Extractor;

        uint8_t program_no() const override
        {   return std::get<0>(this->_m_arguments) & 0x7F; }

        uint16_t bank() const override
        {   return ((std::get<0>(this->_m_arguments) >> 8) & 0x7F) 
                 | ((std::get<0>(this->_m_arguments) >> 16) & 0x7F); }
    };

    template<typename Extractor = util::stream_data_extractor<uint16_t>>
    struct track_usage_impl : track_usage
    , public Extractor
    {
        template<typename E> using self = track_usage_impl<E>;
        using Extractor::Extractor;

        bool is_track_used(size_t track_no) const override
        {   return std::get<0>(this->_m_arguments) >> track_no & 1; }
    };

    template<typename Extractor = util::stream_data_extractor<uint8_t, types::uint<24>>>
    struct open_track_impl : open_track
    , public resolvable
    , public Extractor
    {
        template<typename E> using self = open_track_impl<E>;
        using Extractor::Extractor;

        size_t track_no() const override
        {   return std::get<0>(this->_m_arguments); }

        sequence::event_const_iterator track_data() const override
        {   return {}; }

        std::unique_ptr<event> resolve(const resolver& r) const override
        {
            struct resolved_open_track : open_track
            , std::tuple<size_t, sequence::event_const_iterator>
            {
                using std::tuple<size_t, sequence::event_const_iterator>::tuple;
                size_t track_no() const override
                {   return std::get<0>(*this); }

                sequence::event_const_iterator track_data() const override
                {   return std::get<1>(*this); }
            };
            return std::make_unique<resolved_open_track>(track_no(), r(std::get<1>(this->_m_arguments)));
        }
    };
    
    template<typename Extractor = util::stream_data_extractor<types::uint<24>>>
    struct jump_impl : jump
    , public resolvable
    , public Extractor
    {
        template<typename E> using self = jump_impl<E>;
        using Extractor::Extractor;
        
        sequence::event_const_iterator destination() const override
        {   return {}; }

        std::unique_ptr<event> resolve(const resolver& r) const override
        {
            struct resolved_jump : jump
            , std::tuple<sequence::event_const_iterator>
            {
                using std::tuple<sequence::event_const_iterator>::tuple;
                sequence::event_const_iterator destination() const override
                {   return std::get<0>(*this); }
            };
            return std::make_unique<resolved_jump>(r(std::get<0>(this->_m_arguments)));
        }
    };

    template<typename Extractor = util::stream_data_extractor<types::uint<24>>>
    struct call_impl : call
    , public resolvable
    , public Extractor
    {
        template<typename E> using self = call_impl<E>;
        using Extractor::Extractor;
        
        sequence::event_const_iterator destination() const override
        {   return {}; }

        std::unique_ptr<event> resolve(const resolver& r) const override
        {
            struct resolved_call : call
            , std::tuple<sequence::event_const_iterator>
            {
                using std::tuple<sequence::event_const_iterator>::tuple;
                sequence::event_const_iterator destination() const override
                {   return std::get<0>(*this); }
            };

            return std::make_unique<resolved_call>(r(std::get<0>(this->_m_arguments)));
        }
    };

    /**
     * @tparam EventType
     *         Other event impl types.
     *
     * @tparam RandomType
     *         Either srandom or urandom.
     */
    template<typename EventType>
    struct random_impl : random
    {
        random_impl(io::stream_parser& sp)
        : _m_event(sp)
        {}

        const struct event& event() const override
        {   return _m_event; }

        int32_t lower_bound() const override
        {   return std::get<RandomType>(_m_event._m_arguments).lower_bound(); }

        int32_t upper_bound() const override
        {   return std::get<RandomType>(_m_event._m_arguments).upper_bound(); }

    private:
        // chooses the correct sign of random type depending on the type to be bound
        using RandomType = std::conditional_t<
                             std::is_signed<typename util::last_type_of_extractor<typename EventType::extractor_type>::type>::value,
                             types::s_rand, types::u_rand
                           >;
        using EventBase = typename EventType::template self<decltype(util::type_bind<RandomType>(std::declval<typename EventType::extractor_type>()))>;
        struct : EventBase
        {
            using EventBase::EventBase;
        } _m_event;
    };

    /**
     * @tparam EventType
     *         Other event impl types.
     */
    template<typename EventType>
    struct variable_impl : variable
    {
        variable_impl(io::stream_parser& sp)
        : _m_event(sp)
        {}

        const struct event& event() const override
        {   return _m_event; }

        uint8_t slot() const override
        {   return std::get<types::variable>(_m_event._m_arguments).slot(); }

        int16_t value() const override
        {   return std::get<types::variable>(_m_event._m_arguments); }

        using EventBase = typename EventType::template self<decltype(util::type_bind<types::variable>(std::declval<typename EventType::extractor_type>()))>;
        struct : EventBase
        {
            using EventBase::EventBase;
        } _m_event;
    };

    template<typename Extractor = util::stream_data_extractor<uint8_t, uint8_t, uint8_t, types::uint<24>>>
    struct if_statement_impl : if_statement
    , public Extractor
    {
        using Extractor::Extractor;
    };

    template<typename Extractor = util::stream_data_extractor<uint8_t, uint8_t>>
    struct time_impl : time
    , public Extractor
    {
        using Extractor::Extractor;
    };

    template<typename Extractor = util::stream_data_extractor<uint8_t, int16_t, int16_t>>
    struct time_random_impl : time_random
    , public Extractor
    {
        using Extractor::Extractor;
    };

    template<typename Extractor = util::stream_data_extractor<uint8_t, uint8_t>>
    struct time_variable_impl : time_variable
    , public Extractor
    {
        using Extractor::Extractor;
    };

    template<typename Extractor = util::stream_data_extractor<uint8_t>>
    struct timebase_impl : timebase
    , public Extractor
    {
        template<typename E> using self = timebase_impl<E>;
        using Extractor::Extractor;
    };

    template<typename Extractor = util::stream_data_extractor<int8_t>>
    struct environment_hold_impl : environment_hold
    , public Extractor
    {
        template<typename E> using self = environment_hold_impl<E>;
        using Extractor::Extractor;
    };

    template<typename Extractor = util::stream_data_extractor<uint8_t>>
    struct polyphony_impl : polyphony
    , public Extractor
    {
        template<typename E> using self = polyphony_impl<E>;
        using Extractor::Extractor;
        bool enabled() const override
        {   return std::get<0>(this->_m_arguments); }
    };

    template<typename Extractor = util::stream_data_extractor<uint8_t>>
    struct velocity_range_impl : velocity_range
    , public Extractor
    {
        template<typename E> using self = velocity_range_impl<E>;
        using Extractor::Extractor;
    };

    template<typename Extractor = util::stream_data_extractor<uint8_t>>
    struct biquad_type_impl : biquad_type
    , public Extractor
    {
        template<typename E> using self = biquad_type_impl<E>;
        using Extractor::Extractor;
    };

    template<typename Extractor = util::stream_data_extractor<uint8_t>>
    struct biquad_value_impl : biquad_value
    , public Extractor
    {
        template<typename E> using self = biquad_value_impl<E>;
        using Extractor::Extractor;
    };

    template<typename Extractor = util::stream_data_extractor<uint8_t>>
    struct pan_impl : pan
    , public Extractor
    {
        template<typename E> using self = pan_impl<E>;
        using Extractor::Extractor;
        uint8_t position() const override
        {   return std::get<0>(this->_m_arguments); }
    };

    template<typename Extractor = util::stream_data_extractor<uint8_t>>
    struct volume_impl : volume
    , public Extractor
    {
        template<typename E> using self = volume_impl<E>;
        using Extractor::Extractor;
        uint8_t position() const override
        {   return std::get<0>(this->_m_arguments); }
    };

    template<typename Extractor = util::stream_data_extractor<uint8_t>>
    struct main_volume_impl : main_volume
    , public Extractor
    {
        template<typename E> using self = main_volume_impl<E>;
        using Extractor::Extractor;
        uint8_t position() const override
        {   return std::get<0>(this->_m_arguments); }
    };

    template<typename Extractor = util::stream_data_extractor<int8_t>>
    struct transpose_impl : transpose
    , public Extractor
    {
        template<typename E> using self = transpose_impl<E>;
        using Extractor::Extractor;
        int8_t step() const override
        {   return std::get<0>(this->_m_arguments); }
    };

    template<typename Extractor = util::stream_data_extractor<int8_t>>
    struct pitch_bend_impl : pitch_bend
    , public Extractor
    {
        template<typename E> using self = pitch_bend_impl<E>;
        using Extractor::Extractor;
        int8_t position() const override
        {   return std::get<0>(this->_m_arguments); }
    };

    template<typename Extractor = util::stream_data_extractor<uint8_t>>
    struct pitch_bend_range_impl : pitch_bend_range
    , public Extractor
    {
        template<typename E> using self = pitch_bend_range_impl<E>;
        using Extractor::Extractor;
        uint8_t semitones() const override
        {   return std::get<0>(this->_m_arguments); }
    };

    template<typename Extractor = util::stream_data_extractor<uint8_t>>
    struct priority_impl : priority
    , public Extractor
    {
        template<typename E> using self = priority_impl<E>;
        using Extractor::Extractor;
        uint8_t priority_value() const override
        {   return std::get<0>(this->_m_arguments); }
    };

    template<typename Extractor = util::stream_data_extractor<uint8_t>>
    struct note_wait_impl : note_wait
    , public Extractor
    {
        template<typename E> using self = note_wait_impl<E>;
        using Extractor::Extractor;
        uint8_t duration() const override
        {   return std::get<0>(this->_m_arguments); }
    };

    template<typename Extractor = util::stream_data_extractor<uint8_t>>
    struct tie_impl : tie
    , public Extractor
    {
        template<typename E> using self = tie_impl<E>;
        using Extractor::Extractor;
        uint8_t duration() const override
        {   return std::get<0>(this->_m_arguments); }
    };

    template<typename Extractor = util::stream_data_extractor<uint8_t>>
    struct portamento_control_impl : portamento_control
    , public Extractor
    {
        template<typename E> using self = portamento_control_impl<E>;
        using Extractor::Extractor;
        uint8_t amount() const override
        {   return std::get<0>(this->_m_arguments); }
    };

    template<typename Extractor = util::stream_data_extractor<uint8_t>>
    struct modulation_type_impl : modulation_type
    , public Extractor
    {
        template<typename E> using self = modulation_type_impl<E>;
        using Extractor::Extractor;
        mod_type type() const override
        {   return static_cast<mod_type>(static_cast<uint8_t>(std::get<0>(this->_m_arguments))); }
    };

    template<typename Extractor = util::stream_data_extractor<uint8_t>>
    struct modulation_depth_impl : modulation_depth
    , public Extractor
    {
        template<typename E> using self = modulation_depth_impl<E>;
        using Extractor::Extractor;
        uint8_t value() const override
        {   return std::get<0>(this->_m_arguments); }
    };

    template<typename Extractor = util::stream_data_extractor<uint8_t>>
    struct modulation_speed_impl : modulation_speed
    , public Extractor
    {
        template<typename E> using self = modulation_speed_impl<E>;
        using Extractor::Extractor;
        uint8_t value() const override
        {   return std::get<0>(this->_m_arguments); }
    };

    template<typename Extractor = util::stream_data_extractor<uint8_t>>
    struct modulation_range_impl : modulation_range
    , public Extractor
    {
        template<typename E> using self = modulation_range_impl<E>;
        using Extractor::Extractor;
        uint8_t value() const override
        {   return std::get<0>(this->_m_arguments); }
    };

    template<typename Extractor = util::stream_data_extractor<uint16_t>>
    struct modulation_delay_impl : modulation_delay
    , public Extractor
    {
        template<typename E> using self = modulation_delay_impl<E>;
        using Extractor::Extractor;
        uint16_t value() const override
        {   return std::get<0>(this->_m_arguments); }
    };

    template<typename Extractor = util::stream_data_extractor<uint8_t>>
    struct portamento_switch_impl : portamento_switch
    , public Extractor
    {
        template<typename E> using self = portamento_switch_impl<E>;
        using Extractor::Extractor;
        bool on() const override
        {   return std::get<0>(this->_m_arguments); }
    };

    template<typename Extractor = util::stream_data_extractor<uint8_t>>
    struct portamento_time_impl : portamento_time
    , public Extractor
    {
        template<typename E> using self = portamento_time_impl<E>;
        using Extractor::Extractor;
        uint8_t rate() const override
        {   return std::get<0>(this->_m_arguments); }
    };

    template<typename Extractor = util::stream_data_extractor<int8_t>>
    struct attack_impl : attack
    , public Extractor
    {
        template<typename E> using self = attack_impl<E>;
        using Extractor::Extractor;
        uint8_t time() const override
        {   return std::get<0>(this->_m_arguments); }
    };

    template<typename Extractor = util::stream_data_extractor<int8_t>>
    struct decay_impl : decay
    , public Extractor
    {
        template<typename E> using self = decay_impl<E>;
        using Extractor::Extractor;
        uint8_t time() const override
        {   return std::get<0>(this->_m_arguments); }
    };

    template<typename Extractor = util::stream_data_extractor<int8_t>>
    struct sustain_impl : sustain
    , public Extractor
    {
        template<typename E> using self = sustain_impl<E>;
        using Extractor::Extractor;
        uint8_t time() const override
        {   return std::get<0>(this->_m_arguments); }
    };

    template<typename Extractor = util::stream_data_extractor<int8_t>>
    struct release_impl : release
    , public Extractor
    {
        template<typename E> using self = release_impl<E>;
        using Extractor::Extractor;
        uint8_t time() const override
        {   return std::get<0>(this->_m_arguments); }
    };

    template<typename Extractor = util::stream_data_extractor<uint8_t>>
    struct loop_start_impl : loop_start
    , public Extractor
    {
        template<typename E> using self = loop_start_impl<E>;
        using Extractor::Extractor;
        uint8_t times() const override
        {   return std::get<0>(this->_m_arguments); }
    };

    template<typename Extractor = util::stream_data_extractor<uint8_t>>
    struct expression_impl : expression
    , public Extractor
    {
        template<typename E> using self = expression_impl<E>;
        using Extractor::Extractor;
        uint8_t percent() const override
        {   return std::get<0>(this->_m_arguments); }
    };

    template<typename Extractor = util::stream_data_extractor<uint8_t>>
    struct print_impl : print
    , public Extractor
    {
        template<typename E> using self = print_impl<E>;
        using Extractor::Extractor;
    };

    template<typename Extractor = util::stream_data_extractor<uint8_t>>
    struct surround_pan_impl : surround_pan
    , public Extractor
    {
        template<typename E> using self = surround_pan_impl<E>;
        using Extractor::Extractor;
    };

    template<typename Extractor = util::stream_data_extractor<uint8_t>>
    struct low_pass_filter_cutoff_impl : low_pass_filter_cutoff
    , public Extractor
    {
        template<typename E> using self = low_pass_filter_cutoff_impl<E>;
        using Extractor::Extractor;
    };

    template<typename Extractor = util::stream_data_extractor<uint8_t>>
    struct effect_send_a_impl : effect_send_a
    , public Extractor
    {
        template<typename E> using self = effect_send_a_impl<E>;
        using Extractor::Extractor;
    };

    template<typename Extractor = util::stream_data_extractor<uint8_t>>
    struct effect_send_b_impl : effect_send_b
    , public Extractor
    {
        template<typename E> using self = effect_send_b_impl<E>;
        using Extractor::Extractor;
    };

    template<typename Extractor = util::stream_data_extractor<uint8_t>>
    struct effect_send_c_impl : effect_send_c
    , public Extractor
    {
        template<typename E> using self = effect_send_c_impl<E>;
        using Extractor::Extractor;
    };

    template<typename Extractor = util::stream_data_extractor<uint8_t>>
    struct main_send_impl : main_send
    , public Extractor
    {
        template<typename E> using self = main_send_impl<E>;
        using Extractor::Extractor;
    };

    template<typename Extractor = util::stream_data_extractor<uint8_t>>
    struct initial_pan_impl : initial_pan
    , public Extractor
    {
        template<typename E> using self = initial_pan_impl<E>;
        using Extractor::Extractor;
    };

    template<typename Extractor = util::stream_data_extractor<uint8_t>>
    struct mute_impl : mute
    , public Extractor
    {
        template<typename E> using self = mute_impl<E>;
        using Extractor::Extractor;
    };

    template<typename Extractor = util::stream_data_extractor<uint8_t>>
    struct damper_impl : damper
    , public Extractor
    {
        template<typename E> using self = damper_impl<E>;
        using Extractor::Extractor;
    };

    template<typename Extractor = util::stream_data_extractor<uint16_t>>
    struct tempo_impl : tempo
    , public Extractor
    {
        template<typename E> using self = tempo_impl<E>;
        using Extractor::Extractor;
        uint16_t bpm() const override
        {   return std::get<0>(this->_m_arguments); }
    };

    template<typename Extractor = util::stream_data_extractor<uint16_t>>
    struct sweep_pitch_impl : sweep_pitch
    , public Extractor
    {
        template<typename E> using self = sweep_pitch_impl<E>;
        using Extractor::Extractor;
    };

    template<typename Extractor = util::stream_data_extractor<uint8_t>>
    struct environment_reset_impl : environment_reset
    , public Extractor
    {
        template<typename E> using self = environment_reset_impl<E>;
        using Extractor::Extractor;
    };

    struct var_binop_impl : virtual var_binop
    , util::stream_data_extractor<types::variable, int16_t>
    {
        using extractor_type::extractor_type;

        uint8_t l_operand_var() const override
        {   return std::get<0>(this->_m_arguments).slot(); }

        int16_t r_operand_val() const override
        {   return std::get<1>(this->_m_arguments); }
    };

    struct set_v_impl : set_v, var_binop_impl
    {
        using var_binop_impl::var_binop_impl;

        void operator()() const override
        {   std::get<types::variable>(this->_m_arguments) = r_operand_val();    }
    };

    struct add_v_impl : add_v, var_binop_impl
    {
        using var_binop_impl::var_binop_impl;

        void operator()() const override
        {   std::get<types::variable>(this->_m_arguments) += r_operand_val();    }
    };

    struct sub_v_impl : sub_v, var_binop_impl
    {
        using var_binop_impl::var_binop_impl;

        void operator()() const override
        {   std::get<types::variable>(this->_m_arguments) -= r_operand_val();    }
    };

    struct mul_v_impl : mul_v, var_binop_impl
    {
        using var_binop_impl::var_binop_impl;

        void operator()() const override
        {   std::get<types::variable>(this->_m_arguments) *= r_operand_val();    }
    };

    struct div_v_impl : div_v, var_binop_impl
    {
        using var_binop_impl::var_binop_impl;

        void operator()() const override
        {   std::get<types::variable>(this->_m_arguments) /= r_operand_val();    }
    };

    struct shift_v_impl : shift_v, var_binop_impl
    {
        using var_binop_impl::var_binop_impl;

        // TODO: figure out shift direction
        void operator()() const override
        {   std::get<types::variable>(this->_m_arguments) <<= r_operand_val();    }
    };

    struct rand_v_impl : rand_v, var_binop_impl
    {
        using var_binop_impl::var_binop_impl;

        // TODO: unknown
        void operator()() const override {}
    };

    struct and_v_impl : and_v, var_binop_impl
    {
        using var_binop_impl::var_binop_impl;

        void operator()() const override 
        {   std::get<types::variable>(this->_m_arguments) &= r_operand_val();    }
    };

    struct or_v_impl : or_v, var_binop_impl
    {
        using var_binop_impl::var_binop_impl;

        void operator()() const override 
        {   std::get<types::variable>(this->_m_arguments) |= r_operand_val();    }
    };

    struct xor_v_impl : xor_v, var_binop_impl
    {
        using var_binop_impl::var_binop_impl;

        void operator()() const override 
        {   std::get<types::variable>(this->_m_arguments) ^= r_operand_val();    }
    };

    struct not_v_impl : not_v, var_binop_impl
    {
        using var_binop_impl::var_binop_impl;

        // TODO: not sure
        void operator()() const override 
        {   std::get<types::variable>(this->_m_arguments) = ~std::get<types::variable>(this->_m_arguments);    }
    };

    struct mod_v_impl : mod_v, var_binop_impl
    {
        using var_binop_impl::var_binop_impl;

        void operator()() const override 
        {   std::get<types::variable>(this->_m_arguments) %= r_operand_val();    }
    };

    struct eq_v_impl : eq_v, var_binop_impl
    {
        using var_binop_impl::var_binop_impl;

        // TODO: not sure
        void operator()() const override 
        {   std::get<types::variable>(this->_m_arguments) = std::get<types::variable>(this->_m_arguments) == r_operand_val();    }
    };

    struct ge_v_impl : ge_v, var_binop_impl
    {
        using var_binop_impl::var_binop_impl;

        // TODO: not sure
        void operator()() const override 
        {   std::get<types::variable>(this->_m_arguments) = std::get<types::variable>(this->_m_arguments) >= r_operand_val();    }
    };

    struct gt_v_impl : gt_v, var_binop_impl
    {
        using var_binop_impl::var_binop_impl;

        // TODO: not sure
        void operator()() const override 
        {   std::get<types::variable>(this->_m_arguments) = std::get<types::variable>(this->_m_arguments) > r_operand_val();    }
    };

    struct le_v_impl : le_v, var_binop_impl
    {
        using var_binop_impl::var_binop_impl;

        // TODO: not sure
        void operator()() const override 
        {   std::get<types::variable>(this->_m_arguments) = std::get<types::variable>(this->_m_arguments) <= r_operand_val();    }
    };

    struct lt_v_impl : lt_v, var_binop_impl
    {
        using var_binop_impl::var_binop_impl;

        // TODO: not sure
        void operator()() const override 
        {   std::get<types::variable>(this->_m_arguments) = std::get<types::variable>(this->_m_arguments) < r_operand_val();    }
    };

    struct ne_v_impl : ne_v, var_binop_impl
    {
        using var_binop_impl::var_binop_impl;

        // TODO: not sure
        void operator()() const override 
        {   std::get<types::variable>(this->_m_arguments) = std::get<types::variable>(this->_m_arguments) != r_operand_val();    }
    };

    template<typename Extractor = util::stream_data_extractor<uint16_t>>
    struct userproc_v_impl : userproc_v
    , public Extractor
    {
        using Extractor::Extractor;

        // TODO: not sure
        void operator()() const override {}
    };
}
}
}

#endif
