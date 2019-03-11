#ifndef BRTOOLS_DATA_SEQUENCE_EVENT_VISITOR_H
#define BRTOOLS_DATA_SEQUENCE_EVENT_VISITOR_H
#pragma once

#include <data/sequence/eventfwd.h>

namespace brtools
{
namespace data
{
namespace sequence
{
    struct visitor
    {
        virtual void visit(const event&) {}
        virtual ~visitor() = default;

        virtual void visit(const note_on&) {}
        virtual void visit(const wait&) {}
        virtual void visit(const program&) {}
        virtual void visit(const track_usage&) {}
        virtual void visit(const open_track&) {}
        virtual void visit(const jump&) {}
        virtual void visit(const call&) {}
        virtual void visit(const random&) {}
        virtual void visit(const variable&) {}
        virtual void visit(const if_statement&) {}
        virtual void visit(const time&) {}
        virtual void visit(const time_random&) {}
        virtual void visit(const time_variable&) {}
        virtual void visit(const timebase&) {}
        virtual void visit(const environment_hold&) {}
        virtual void visit(const polyphony&) {}
        virtual void visit(const velocity_range&) {}
        virtual void visit(const biquad_type&) {}
        virtual void visit(const biquad_value&) {}
        virtual void visit(const pan&) {}
        virtual void visit(const volume&) {}
        virtual void visit(const main_volume&) {}
        virtual void visit(const transpose&) {}
        virtual void visit(const pitch_bend&) {}
        virtual void visit(const pitch_bend_range&) {}
        virtual void visit(const priority&) {}
        virtual void visit(const note_wait&) {}
        virtual void visit(const tie&) {}
        virtual void visit(const portamento_control&) {}
        virtual void visit(const modulation_type&) {}
        virtual void visit(const modulation_depth&) {}
        virtual void visit(const modulation_speed&) {}
        virtual void visit(const modulation_range&) {}
        virtual void visit(const modulation_delay&) {}
        virtual void visit(const portamento_switch&) {}
        virtual void visit(const portamento_time&) {}
        virtual void visit(const attack&) {}
        virtual void visit(const decay&) {}
        virtual void visit(const sustain&) {}
        virtual void visit(const release&) {}
        virtual void visit(const loop_start&) {}
        virtual void visit(const loop_end&) {}
        virtual void visit(const expression&) {}
        virtual void visit(const print&) {}
        virtual void visit(const surround_pan&) {}
        virtual void visit(const low_pass_filter_cutoff&) {}
        virtual void visit(const effect_send_a&) {}
        virtual void visit(const effect_send_b&) {}
        virtual void visit(const effect_send_c&) {}
        virtual void visit(const main_send&) {}
        virtual void visit(const initial_pan&) {}
        virtual void visit(const mute&) {}
        virtual void visit(const damper&) {}
        virtual void visit(const tempo&) {}
        virtual void visit(const sweep_pitch&) {}
        virtual void visit(const set_v&) {}
        virtual void visit(const add_v&) {}
        virtual void visit(const sub_v&) {}
        virtual void visit(const mul_v&) {}
        virtual void visit(const div_v&) {}
        virtual void visit(const shift_v&) {}
        virtual void visit(const rand_v&) {}
        virtual void visit(const and_v&) {}
        virtual void visit(const or_v&) {}
        virtual void visit(const xor_v&) {}
        virtual void visit(const not_v&) {}
        virtual void visit(const mod_v&) {}
        virtual void visit(const eq_v&) {}
        virtual void visit(const ge_v&) {}
        virtual void visit(const gt_v&) {}
        virtual void visit(const le_v&) {}
        virtual void visit(const lt_v&) {}
        virtual void visit(const ne_v&) {}
        virtual void visit(const userproc_v&) {}
        virtual void visit(const environment_reset&) {}
        virtual void visit(const ret&) {}
        virtual void visit(const fin&) {}
    };
}
}
}

#endif

