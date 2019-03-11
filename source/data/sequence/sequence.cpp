#include <data/sequence/events.h>
#include <data/sequence/sequence.h>
#include <data/sequence/parser.h>
#include <data/sequence/events_impl.h>
#include <io/stream_parser.h>
#include <iostream>     // for istream

using namespace brtools::io;
using namespace brtools::data::sequence;
using namespace std;

namespace
{
    // convenience template that typedefs to the template argument
    template<typename EventType> using IdentityEvent = EventType;

    template<template<typename> class TargetEventType = IdentityEvent>
    unique_ptr<event> build_event(stream_parser& sp)
    {
        if (sp.peek<uint8_t>() < 0x80)
        {
            return make_unique<TargetEventType<note_on_impl<>>>(sp);
        }

        switch (sp.read<uint8_t>())
        {
            case 0x80: return make_unique<TargetEventType< wait_impl<>                   >>(sp);
            case 0x81: return make_unique<TargetEventType< program_impl<>                >>(sp);
            case 0xFE: return make_unique<TargetEventType< track_usage_impl<>            >>(sp);
            case 0x88: return make_unique<TargetEventType< open_track_impl<>             >>(sp);
            case 0x89: return make_unique<TargetEventType< jump_impl<>                   >>(sp);
            case 0x8A: return make_unique<TargetEventType< call_impl<>                   >>(sp);
            case 0xA0: return build_event<                 random_impl                    >(sp);
            case 0xA1: return build_event<                 variable_impl                  >(sp);
            case 0xA2: return make_unique<                 if_statement_impl<>            >(sp);
            case 0xA3: return make_unique<                 time_impl<>                    >(sp);
            case 0xA4: return make_unique<                 time_random_impl<>             >(sp);
            case 0xA5: return make_unique<                 time_variable_impl<>           >(sp);
            case 0xB0: return make_unique<TargetEventType< timebase_impl<>               >>(sp);
            case 0xB1: return make_unique<TargetEventType< environment_hold_impl<>       >>(sp);
            case 0xB2: return make_unique<TargetEventType< polyphony_impl<>              >>(sp);
            case 0xB3: return make_unique<TargetEventType< velocity_range_impl<>         >>(sp);
            case 0xB4: return make_unique<TargetEventType< biquad_type_impl<>            >>(sp);
            case 0xB5: return make_unique<TargetEventType< biquad_value_impl<>           >>(sp);
            case 0xC0: return make_unique<TargetEventType< pan_impl<>                    >>(sp);
            case 0xC1: return make_unique<TargetEventType< volume_impl<>                 >>(sp);
            case 0xC2: return make_unique<TargetEventType< main_volume_impl<>            >>(sp);
            case 0xC3: return make_unique<TargetEventType< transpose_impl<>              >>(sp);
            case 0xC4: return make_unique<TargetEventType< pitch_bend_impl<>             >>(sp);
            case 0xC5: return make_unique<TargetEventType< pitch_bend_range_impl<>       >>(sp);
            case 0xC6: return make_unique<TargetEventType< priority_impl<>               >>(sp);
            case 0xC7: return make_unique<TargetEventType< note_wait_impl<>              >>(sp);
            case 0xC8: return make_unique<TargetEventType< priority_impl<>               >>(sp);
            case 0xC9: return make_unique<TargetEventType< portamento_control_impl<>     >>(sp);
            case 0xCC: return make_unique<TargetEventType< modulation_type_impl<>        >>(sp);
            case 0xCA: return make_unique<TargetEventType< modulation_depth_impl<>       >>(sp);
            case 0xCB: return make_unique<TargetEventType< modulation_speed_impl<>       >>(sp);
            case 0xCD: return make_unique<TargetEventType< modulation_range_impl<>       >>(sp);
            case 0xE0: return make_unique<TargetEventType< modulation_delay_impl<>       >>(sp);
            case 0xCE: return make_unique<TargetEventType< portamento_switch_impl<>      >>(sp);
            case 0xCF: return make_unique<TargetEventType< portamento_time_impl<>        >>(sp);
            case 0xD0: return make_unique<TargetEventType< attack_impl<>                 >>(sp);
            case 0xD1: return make_unique<TargetEventType< decay_impl<>                  >>(sp);
            case 0xD2: return make_unique<TargetEventType< sustain_impl<>                >>(sp);
            case 0xD3: return make_unique<TargetEventType< release_impl<>                >>(sp);
            case 0xD4: return make_unique<TargetEventType< loop_start_impl<>             >>(sp);
            case 0xFC: return make_unique<                 loop_end                       >(  );
            case 0xD5: return make_unique<TargetEventType< expression_impl<>             >>(sp);
            case 0xD6: return make_unique<TargetEventType< print_impl<>                  >>(sp);
            case 0xD7: return make_unique<TargetEventType< surround_pan_impl<>           >>(sp);
            case 0xD8: return make_unique<TargetEventType< low_pass_filter_cutoff_impl<> >>(sp);
            case 0xD9: return make_unique<TargetEventType< effect_send_a_impl<>          >>(sp);
            case 0xDA: return make_unique<TargetEventType< effect_send_b_impl<>          >>(sp);
            case 0xDE: return make_unique<TargetEventType< effect_send_c_impl<>          >>(sp);
            case 0xDB: return make_unique<TargetEventType< main_send_impl<>              >>(sp);
            case 0xDC: return make_unique<TargetEventType< initial_pan_impl<>            >>(sp);
            case 0xDD: return make_unique<TargetEventType< mute_impl<>                   >>(sp);
            case 0xDF: return make_unique<TargetEventType< damper_impl<>                 >>(sp);
            case 0xE1: return make_unique<TargetEventType< tempo_impl<>                  >>(sp);
            case 0xE3: return make_unique<TargetEventType< sweep_pitch_impl<>            >>(sp);
            case 0xF0:
            {
                switch (sp.read<uint8_t>())
                {
                    case 0x80: return make_unique< set_v_impl        >(sp);
                    case 0x81: return make_unique< add_v_impl        >(sp);
                    case 0x82: return make_unique< sub_v_impl        >(sp);
                    case 0x83: return make_unique< mul_v_impl        >(sp);
                    case 0x84: return make_unique< div_v_impl        >(sp);
                    case 0x85: return make_unique< shift_v_impl      >(sp);
                    case 0x86: return make_unique< rand_v_impl       >(sp);
                    case 0x87: return make_unique< and_v_impl        >(sp);
                    case 0x88: return make_unique< or_v_impl         >(sp);
                    case 0x89: return make_unique< xor_v_impl        >(sp);
                    case 0x8A: return make_unique< not_v_impl        >(sp);
                    case 0x8B: return make_unique< mod_v_impl        >(sp);
                    case 0x90: return make_unique< eq_v_impl         >(sp);
                    case 0x91: return make_unique< ge_v_impl         >(sp);
                    case 0x92: return make_unique< gt_v_impl         >(sp);
                    case 0x93: return make_unique< le_v_impl         >(sp);
                    case 0x94: return make_unique< lt_v_impl         >(sp);
                    case 0x95: return make_unique< ne_v_impl         >(sp);
                    case 0xE0: return make_unique< userproc_v_impl<> >(sp);
                    default: return nullptr;
                }
            }
            case 0xFD: return make_unique< ret >();
            case 0xFF: return make_unique< fin >();
            default: return nullptr; 
        }
    }
}

sequence sequence::make_sequence(istream& stm)
{
    stream_parser sp(stm);
    sequence result;
    {
        parser rseq(sp);
        unordered_map<streamoff, size_t> offset_2_index;

        // DATA section
        for (const auto data = rseq.data(); data.has_data_to_read();)
        {
            const auto offset = sp.tell_offset_from_base();
            if (auto event = build_event<>(sp))
            {
                // if event_ptr is null, the event is not recognized,
                // in that case we ignore it.
                result._m_events.push_back(move(event));
                offset_2_index[offset] = result._m_events.size() - 1;
            }
        }

        // LABL section
        {
            const auto labl = rseq.labl();
            for (const auto& offset_string_pair : labl.labels())
            {
                const auto offset_index_iter = offset_2_index.find(offset_string_pair.first);
                if (offset_index_iter != offset_2_index.end())
                {
                    result._m_labels[offset_string_pair.second] = offset_index_iter->second;
                }
            }
        }
    }
    return result;
}

void sequence::traverse(visitor& visitor) const
{
    for (const auto& event_ptr : _m_events)
    {
        event_ptr->accept(visitor);
    }
}
