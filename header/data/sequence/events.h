#ifndef BRTOOLS_DATA_SEQUENCE_EVENTS_H
#define BRTOOLS_DATA_SEQUENCE_EVENTS_H
#pragma once

#include <data/sequence/sequence.h>
#include <data/sequence/eventfwd.h>
#include <data/sequence/visitable.h>
#include <data/sequence/visitor_acceptable.h>

namespace brtools
{
namespace data
{
namespace sequence
{
    /**
     * Represents a generic sequence event.
     */
    struct event : virtual visitor_acceptable {};

    // 0x00 through 0x7F
    struct note_on : event, visitable<note_on>
    {
        virtual uint8_t note() const = 0;
        virtual uint8_t pitch() const = 0;
        virtual uint32_t velocity() const = 0;
    };

    // 0x80
    struct wait : event, visitable<wait>
    {
        virtual uint32_t tick_count() const = 0;
    };

    // 0x81
    struct program : event, visitable<program>
    {
        virtual uint8_t program_no() const = 0;
        virtual uint16_t bank() const = 0;
    };

    // 0xFE
    struct track_usage : event, visitable<track_usage>
    {
        virtual bool is_track_used(size_t track_no) const = 0;
    };

    // 0x88
    struct open_track : event, visitable<open_track>
    {
        virtual size_t track_no() const = 0;
        virtual sequence::event_const_iterator track_data() const = 0;
    };

    // 0x89
    struct jump : event, visitable<jump>
    {
        virtual sequence::event_const_iterator destination() const = 0;
    };

    // 0x8A
    struct call : event, visitable<call>
    {
        virtual sequence::event_const_iterator destination() const = 0;
    };

    // 0xA0
    struct random : event, visitable<random>
    {
        virtual const event& event() const = 0;
        virtual int32_t lower_bound() const = 0;
        virtual int32_t upper_bound() const = 0;
    };

    // 0xA1
    struct variable : event, visitable<variable>
    {
        virtual const event& event() const = 0;
        virtual uint8_t slot() const = 0;
        virtual int16_t value() const = 0;
    };

    // 0xA2
    struct if_statement : event, visitable<if_statement>
    {
        // TODO: unknown
    };

    // 0xA3
    struct time : event, visitable<time>
    {
        // TODO: unknown
    };

    // 0xA4
    struct time_random : event, visitable<time_random>
    {
        // TODO: unknown
    };

    // 0xA5
    struct time_variable : event, visitable<time_variable>
    {
        // TODO: unknown
    };

    // 0xB0
    struct timebase : event, visitable<timebase>
    {
        // TODO: unknown
    };

    // 0xB1
    struct environment_hold : event, visitable<environment_hold>
    {
        // TODO: unknown
    };

    // 0xB2
    struct polyphony : event, visitable<polyphony>
    {
        virtual bool enabled() const = 0;
    };

    // 0xB3
    struct velocity_range : event, visitable<velocity_range>
    {
        // TODO: unknown
    };

    // 0xB4
    struct biquad_type : event, visitable<biquad_type>
    {
        // TODO: unknown
    };

    // 0xB5
    struct biquad_value : event, visitable<biquad_value>
    {
        // TODO: unknown
    };

    // 0xC0
    struct pan : event, visitable<pan>
    {
        virtual uint8_t position() const = 0;
    };

    // 0xC1
    struct volume : event, visitable<volume>
    {
        virtual uint8_t position() const = 0;
    };

    // 0xC2
    struct main_volume : event, visitable<main_volume>
    {
        virtual uint8_t position() const = 0;
    };

    // 0xC3
    struct transpose : event, visitable<transpose>
    {
        virtual int8_t step() const = 0;
    };

    // 0xC4
    struct pitch_bend : event, visitable<pitch_bend>
    {
        virtual int8_t position() const = 0;
    };

    // 0xC5
    struct pitch_bend_range : event, visitable<pitch_bend_range>
    {
        virtual uint8_t semitones() const = 0;
    };

    // 0xC6
    struct priority : event, visitable<priority>
    {
        virtual uint8_t priority_value() const = 0;
    };

    // 0xC7
    struct note_wait : event, visitable<note_wait>
    {
        virtual uint8_t duration() const = 0;
    };

    // 0xC8
    struct tie : event, visitable<tie>
    {
        virtual uint8_t duration() const = 0;
    };

    // 0xC9
    struct portamento_control : event, visitable<portamento_control>
    {
        virtual uint8_t amount() const = 0;
    };

    // 0xCC
    struct modulation_type : event, visitable<modulation_type>
    {
        enum class mod_type {
            pitch, volume, pan
        };
        virtual mod_type type() const = 0;
    };

    // 0xCA
    struct modulation_depth : event, visitable<modulation_depth>
    {
        virtual bool on() const = 0;
    };

    // 0xCB
    struct modulation_speed : event, visitable<modulation_speed>
    {
        virtual uint8_t value() const = 0;
    };

    // 0xCD
    struct modulation_range : event, visitable<modulation_range>
    {
        virtual uint8_t value() const = 0;
    };

    // 0xE0
    struct modulation_delay : event, visitable<modulation_delay>
    {
        virtual uint16_t value() const = 0;
    };

    // 0xCE
    struct portamento_switch : event, visitable<portamento_switch>
    {
        virtual bool on() const = 0;
    };

    // 0xCF
    struct portamento_time : event, visitable<portamento_time>
    {
        virtual uint8_t rate() const = 0;
    };

    // 0xD0
    struct attack : event, visitable<attack>
    {
        virtual uint8_t time() const = 0;
    };

    // 0xD1
    struct decay : event, visitable<decay>
    {
        virtual uint8_t time() const = 0;
    };

    // 0xD2
    struct sustain : event, visitable<sustain>
    {
        virtual uint8_t time() const = 0;
    };

    // 0xD3
    struct release : event, visitable<release>
    {
        virtual uint8_t time() const = 0;
    };

    // 0xD4
    struct loop_start : event, visitable<loop_start>
    {
        virtual uint8_t times() const = 0;
    };

    // 0xFC
    struct loop_end : event, visitable<loop_end> {};

    // 0xD5
    struct expression : event, visitable<expression>
    {
        virtual uint8_t percent() const = 0;
    };

    // 0xD6
    struct print : event, visitable<print>
    {
        // TODO: unknown
    };

    // 0xD7
    struct surround_pan : event, visitable<surround_pan>
    {
        // TODO: unknown
    };

    // 0xD8
    struct low_pass_filter_cutoff : event, visitable<low_pass_filter_cutoff>
    {
        // TODO: unknown
    };

    // 0xD9
    struct effect_send_a : event, visitable<effect_send_a>
    {
        // TODO: unknown
    };

    // 0xDA
    struct effect_send_b : event, visitable<effect_send_b>
    {
        // TODO: unknown
    };

    // 0xDE
    struct effect_send_c : event, visitable<effect_send_c>
    {
        // TODO: unknown
    };

    // 0xDB
    struct main_send : event, visitable<main_send>
    {
        // TODO: unknown
    };

    // 0xDC
    struct initial_pan : event, visitable<initial_pan>
    {
        // TODO: unknown
    };

    // 0xDD
    struct mute : event, visitable<mute>
    {
        // TODO: unknown
    };

    // 0xDF
    struct damper : event, visitable<damper>
    {
        // TODO: unknown
    };

    // 0xE1
    struct tempo : event, visitable<tempo>
    {
        virtual uint16_t bpm() const = 0;
    };

    // 0xE3
    struct sweep_pitch : event, visitable<sweep_pitch>
    {
        // TODO: unknown
    };

    // 0xF0
    struct var_binop : event
    {
        virtual uint8_t l_operand_var() const = 0;
        virtual int16_t r_operand_val() const = 0;
        virtual void operator()() const = 0;
    };

    // 0xF0 0x80
    struct set_v : virtual var_binop, visitable<set_v> {};
    // 0xF0 0x81
    struct add_v : virtual var_binop, visitable<add_v> {};
    // 0xF0 0x82
    struct sub_v : virtual var_binop, visitable<sub_v> {};
    // 0xF0 0x83
    struct mul_v : virtual var_binop, visitable<mul_v> {};
    // 0xF0 0x84
    struct div_v : virtual var_binop, visitable<div_v> {};
    // 0xF0 0x85
    struct shift_v : virtual var_binop, visitable<shift_v> {};
    // 0xF0 0x86
    struct rand_v : virtual var_binop, visitable<rand_v> {};
    // 0xF0 0x87
    struct and_v : virtual var_binop, visitable<and_v> {};
    // 0xF0 0x88
    struct or_v : virtual var_binop, visitable<or_v> {};
    // 0xF0 0x89
    struct xor_v : virtual var_binop, visitable<xor_v> {};
    // 0xF0 0x8A
    struct not_v : virtual var_binop, visitable<not_v> {};
    // 0xF0 0x8B
    struct mod_v : virtual var_binop, visitable<mod_v> {};
    // 0xF0 0x90
    struct eq_v : virtual var_binop, visitable<eq_v> {};
    // 0xF0 0x91
    struct ge_v : virtual var_binop, visitable<ge_v> {};
    // 0xF0 0x92
    struct gt_v : virtual var_binop, visitable<gt_v> {};
    // 0xF0 0x93
    struct le_v : virtual var_binop, visitable<le_v> {};
    // 0xF0 0x94
    struct lt_v : virtual var_binop, visitable<lt_v> {};
    // 0xF0 0x95
    struct ne_v : virtual var_binop, visitable<ne_v> {};
    // 0xF0 0xE0
    struct userproc_v : event, visitable<userproc_v>
    {
        virtual void operator()() const = 0;
    };

    // 0xFB
    struct environment_reset : event, visitable<environment_reset>
    {
        // TODO: unknown
    };

    struct ret : event, visitable<ret> {};
    struct fin : event, visitable<fin> {};
}
}
}
#endif

