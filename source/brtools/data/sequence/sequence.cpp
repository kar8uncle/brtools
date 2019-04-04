#include <brtools/data/sequence/events.h>
#include <brtools/data/sequence/sequence.h>
#include <brtools/data/sequence/parser.h>
#include <brtools/data/sequence/events_impl.h>
#include <brtools/io/stream_parser.h>
#include <iostream>     // for istream
#include <utility>      // for move
#include <forward_list>
#include <stack>
#include <cassert>

using namespace brtools::io;
using namespace brtools::data::sequence;
using namespace std;

namespace
{
    /**
     * A simple proxy container that tests if the new value is resolvable.
     * If so, track it.
     */
    struct ContainerProxy
    {
        template<typename ValueType, typename = enable_if_t<is_base_of<resolvable, ValueType>::value>>
        void push(ValueType* v)
        {
            events.emplace_back(v);
            resolvables.emplace_front(--events.end(),
                                      static_cast<const resolvable&>(static_cast<const ValueType&>(*v)));
        }

        void push(event* v)
        {
            events.emplace_back(v);
        }

        sequence::event_container& events;
        forward_list<tuple<sequence::event_iterator, const resolvable&>> resolvables;
    };

    // convenience template that typedefs to the template argument
    template<typename EventType> using IdentityEvent = EventType;

    template<template<typename> class TargetEventType = IdentityEvent, typename ContainerType>
    bool build_event(stream_parser& sp, ContainerType& events)
    {
        if (sp.peek<uint8_t>() < 0x80)
        {
            events.push(new TargetEventType<note_on_impl<>>(sp));
        }
        else switch (sp.read<uint8_t>())
        {
            case 0x80: events.push(new TargetEventType< wait_impl<>                   >(sp)); break;
            case 0x81: events.push(new TargetEventType< program_impl<>                >(sp)); break;
            case 0xFE: events.push(new TargetEventType< track_usage_impl<>            >(sp)); break;
            case 0x88: events.push(new                  open_track_impl<>              (sp)); break;
            case 0x89: events.push(new                  jump_impl<>                    (sp)); break;
            case 0x8A: events.push(new                  call_impl<>                    (sp)); break;
            case 0xA0: return build_event<              random_impl                   >(sp, events); break;
            case 0xA1: return build_event<              variable_impl                 >(sp, events); break;
            case 0xA2: events.push(new                  if_statement_impl<>            (sp)); break;
            case 0xA3: events.push(new                  time_impl<>                    (sp)); break;
            case 0xA4: events.push(new                  time_random_impl<>             (sp)); break;
            case 0xA5: events.push(new                  time_variable_impl<>           (sp)); break;
            case 0xB0: events.push(new TargetEventType< timebase_impl<>               >(sp)); break;
            case 0xB1: events.push(new TargetEventType< environment_hold_impl<>       >(sp)); break;
            case 0xB2: events.push(new TargetEventType< polyphony_impl<>              >(sp)); break;
            case 0xB3: events.push(new TargetEventType< velocity_range_impl<>         >(sp)); break;
            case 0xB4: events.push(new TargetEventType< biquad_type_impl<>            >(sp)); break;
            case 0xB5: events.push(new TargetEventType< biquad_value_impl<>           >(sp)); break;
            case 0xC0: events.push(new TargetEventType< pan_impl<>                    >(sp)); break;
            case 0xC1: events.push(new TargetEventType< volume_impl<>                 >(sp)); break;
            case 0xC2: events.push(new TargetEventType< main_volume_impl<>            >(sp)); break;
            case 0xC3: events.push(new TargetEventType< transpose_impl<>              >(sp)); break;
            case 0xC4: events.push(new TargetEventType< pitch_bend_impl<>             >(sp)); break;
            case 0xC5: events.push(new TargetEventType< pitch_bend_range_impl<>       >(sp)); break;
            case 0xC6: events.push(new TargetEventType< priority_impl<>               >(sp)); break;
            case 0xC7: events.push(new TargetEventType< note_wait_impl<>              >(sp)); break;
            case 0xC8: events.push(new TargetEventType< priority_impl<>               >(sp)); break;
            case 0xC9: events.push(new TargetEventType< portamento_control_impl<>     >(sp)); break;
            case 0xCC: events.push(new TargetEventType< modulation_type_impl<>        >(sp)); break;
            case 0xCA: events.push(new TargetEventType< modulation_depth_impl<>       >(sp)); break;
            case 0xCB: events.push(new TargetEventType< modulation_speed_impl<>       >(sp)); break;
            case 0xCD: events.push(new TargetEventType< modulation_range_impl<>       >(sp)); break;
            case 0xE0: events.push(new TargetEventType< modulation_delay_impl<>       >(sp)); break;
            case 0xCE: events.push(new TargetEventType< portamento_switch_impl<>      >(sp)); break;
            case 0xCF: events.push(new TargetEventType< portamento_time_impl<>        >(sp)); break;
            case 0xD0: events.push(new TargetEventType< attack_impl<>                 >(sp)); break;
            case 0xD1: events.push(new TargetEventType< decay_impl<>                  >(sp)); break;
            case 0xD2: events.push(new TargetEventType< sustain_impl<>                >(sp)); break;
            case 0xD3: events.push(new TargetEventType< release_impl<>                >(sp)); break;
            case 0xD4: events.push(new TargetEventType< loop_start_impl<>             >(sp)); break;
            case 0xFC: events.push(new                  loop_end                       (  )); break;
            case 0xD5: events.push(new TargetEventType< expression_impl<>             >(sp)); break;
            case 0xD6: events.push(new TargetEventType< print_impl<>                  >(sp)); break;
            case 0xD7: events.push(new TargetEventType< surround_pan_impl<>           >(sp)); break;
            case 0xD8: events.push(new TargetEventType< low_pass_filter_cutoff_impl<> >(sp)); break;
            case 0xD9: events.push(new TargetEventType< effect_send_a_impl<>          >(sp)); break;
            case 0xDA: events.push(new TargetEventType< effect_send_b_impl<>          >(sp)); break;
            case 0xDE: events.push(new TargetEventType< effect_send_c_impl<>          >(sp)); break;
            case 0xDB: events.push(new TargetEventType< main_send_impl<>              >(sp)); break;
            case 0xDC: events.push(new TargetEventType< initial_pan_impl<>            >(sp)); break;
            case 0xDD: events.push(new TargetEventType< mute_impl<>                   >(sp)); break;
            case 0xDF: events.push(new TargetEventType< damper_impl<>                 >(sp)); break;
            case 0xE1: events.push(new TargetEventType< tempo_impl<>                  >(sp)); break;
            case 0xE3: events.push(new TargetEventType< sweep_pitch_impl<>            >(sp)); break;
            case 0xF0:
            {
                switch (sp.read<uint8_t>())
                {
                    case 0x80: events.push(new set_v_impl        (sp)); break;
                    case 0x81: events.push(new add_v_impl        (sp)); break;
                    case 0x82: events.push(new sub_v_impl        (sp)); break;
                    case 0x83: events.push(new mul_v_impl        (sp)); break;
                    case 0x84: events.push(new div_v_impl        (sp)); break;
                    case 0x85: events.push(new shift_v_impl      (sp)); break;
                    case 0x86: events.push(new rand_v_impl       (sp)); break;
                    case 0x87: events.push(new and_v_impl        (sp)); break;
                    case 0x88: events.push(new or_v_impl         (sp)); break;
                    case 0x89: events.push(new xor_v_impl        (sp)); break;
                    case 0x8A: events.push(new not_v_impl        (sp)); break;
                    case 0x8B: events.push(new mod_v_impl        (sp)); break;
                    case 0x90: events.push(new eq_v_impl         (sp)); break;
                    case 0x91: events.push(new ge_v_impl         (sp)); break;
                    case 0x92: events.push(new gt_v_impl         (sp)); break;
                    case 0x93: events.push(new le_v_impl         (sp)); break;
                    case 0x94: events.push(new lt_v_impl         (sp)); break;
                    case 0x95: events.push(new ne_v_impl         (sp)); break;
                    case 0xE0: events.push(new userproc_v_impl<> (sp)); break;
                    default: return false;
                }
                break;
            }
            case 0xFD: events.push(new ret()); break;
            case 0xFF: events.push(new fin()); break;
            default: return false; 
        }

        return true;
    }
}

sequence sequence::make_sequence(istream& stm)
{
    stream_parser sp(stm);
    sequence result;
    {
        parser rseq(sp);

        // maps offset to event iterator;
        // offsets are only useful at the time of parsing, because when
        // we support editing, the offsets change all the time and thus
        // not really reliable to be used to map to events.
        unordered_map<streamoff, event_const_iterator> offset_2_iter;

        ContainerProxy container{result._m_events, {}};

        // DATA section
        for (auto data = rseq.data(); data.has_data_to_read() && sp.peek<uint8_t>() != 0x00;)
        {
            // remembers the offset of the current event
            const auto offset = sp.tell_offset_from_base();

            if (build_event<>(sp, container))
            {
                offset_2_iter[offset] = --result._m_events.cend();
            }
        }

        // resolve event references
        for (const auto& iter_resolvable : container.resolvables)
        {
            // wait for C++17 structured binding
            event_iterator    eit = get<0>(iter_resolvable); 
            const resolvable& r   = get<1>(iter_resolvable);

            // resolve reference by mapping from streamoff to event_const_iterator
            *eit = r.resolve([end = result._m_events.cend(), &offset_2_iter](const streamoff offset)
                   {
                      const auto mit = offset_2_iter.find(offset);
                      return mit == offset_2_iter.end() ? end : mit->second;
                   });

            // r should now destruct since it has been replaced with its resolved object,
            // so r is not safe to be accessed here anymore
        }

        // LABL section
        {
            auto labl = rseq.labl();
            for (const auto& offset_string_pair : labl.labels())
            {
                const auto offset_iter_pair = offset_2_iter.find(offset_string_pair.first);
                if (offset_iter_pair != offset_2_iter.end())
                {
                    result._m_label_2_event[offset_string_pair.second] = offset_iter_pair->second;
                }
            }
        }
    }
    return result;
}

void sequence::traverse(visitor& visitor) const
{
    struct concrete_operation final : visitor::operation
    {
        void jump(event_const_iterator it) const override
        {
            _m_next = it;
        }

        void call(event_const_iterator it) const override
        {
            _m_call_stack.push(_m_next);
            jump(it);
        }

        void return_from_call() const override
        {
            if (_m_call_stack.empty())
            {
                end();
            }
            else
            {
                jump(_m_call_stack.top());
                _m_call_stack.pop();
            }
        }

        void start_loop() const override
        {
            _m_loop_stack.push(_m_next);
        }

        void goto_loop_start() const override
        {
            if (_m_loop_stack.empty())
            {
                end();
            }
            else
            {
                jump(_m_loop_stack.top());
            }
        }

        void end_loop() const override
        {
            if (!_m_loop_stack.empty())
            {
                _m_loop_stack.pop();
            }
        }

        void end() const override
        {
            jump(_m_end);
        }

        concrete_operation(event_const_iterator& n, event_const_iterator e,
                           stack<event_const_iterator>& cs,
                           stack<event_const_iterator>& ls)
        : _m_next(n)
        , _m_end(move(e))
        , _m_call_stack(cs)
        , _m_loop_stack(ls)
        {}

    private:
        event_const_iterator& _m_next;
        event_const_iterator  _m_end;
        stack<event_const_iterator>& _m_call_stack;
        stack<event_const_iterator>& _m_loop_stack;
    };

    stack<event_const_iterator> call_stack;
    stack<event_const_iterator> loop_stack;

    for (auto cit = _m_events.cbegin(), cend = _m_events.cend(); 
         cit != cend;)
    {
        // when this assertion fails, we have an unresolved reference
        assert(cit != event_const_iterator());

        (*cit)->accept(visitor, concrete_operation(++cit, cend,
                                                   call_stack, 
                                                   loop_stack));
    }
}
