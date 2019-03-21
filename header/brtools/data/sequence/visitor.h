#ifndef BRTOOLS_DATA_SEQUENCE_VISITOR_H
#define BRTOOLS_DATA_SEQUENCE_VISITOR_H
#pragma once

#include <brtools/data/sequence/eventfwd.h>
#include <brtools/data/sequence/sequence.h>

namespace brtools
{
namespace data
{
namespace sequence
{
    struct visitor
    {
        /**
         * Additional information passed to the visitor for finer-grained control
         * over the visit order.
         */
        struct operation
        {
            /**
             * Causes visitor to jump to the given event pointed to by
             * the iterator.
             */
            virtual void jump(sequence::event_const_iterator) const = 0;

            /**
             * Similar to jump, but creates a call stack so it can be
             * returned to later.
             */ 
            virtual void call(sequence::event_const_iterator) const = 0;
            /**
             * Returns to visit the events after the calling event.
             *
             * If there was no call stack, immediately ends traversal.
             */
            virtual void return_from_call() const = 0;

            /**
             * Marks the next event as the start of a loop. Creates a loop stack.
             */
            virtual void start_loop() const = 0;
            /**
             * Jumps back to the event last marked as the start of a loop.
             *
             * If there was no loop start, immediately ends traversal.
             */
            virtual void goto_loop_start() const = 0;
            /**
             * Exits the loop. Essentially pops a loop stack.
             */
            virtual void end_loop() const = 0;

            /**
             * Ends traversal.
             */
            virtual void end() const = 0;
        };

        virtual void visit(const event&                 , const operation&) {}
        virtual ~visitor() = default;

        virtual void visit(const note_on&               , const operation&) {}
        virtual void visit(const wait&                  , const operation&) {}
        virtual void visit(const program&               , const operation&) {}
        virtual void visit(const track_usage&           , const operation&) {}
        virtual void visit(const open_track&            , const operation&) {}
        virtual void visit(const jump&                  , const operation&) {}
        virtual void visit(const call&                  , const operation&) {}
        virtual void visit(const random&                , const operation&) {}
        virtual void visit(const variable&              , const operation&) {}
        virtual void visit(const if_statement&          , const operation&) {}
        virtual void visit(const time&                  , const operation&) {}
        virtual void visit(const time_random&           , const operation&) {}
        virtual void visit(const time_variable&         , const operation&) {}
        virtual void visit(const timebase&              , const operation&) {}
        virtual void visit(const environment_hold&      , const operation&) {}
        virtual void visit(const polyphony&             , const operation&) {}
        virtual void visit(const velocity_range&        , const operation&) {}
        virtual void visit(const biquad_type&           , const operation&) {}
        virtual void visit(const biquad_value&          , const operation&) {}
        virtual void visit(const pan&                   , const operation&) {}
        virtual void visit(const volume&                , const operation&) {}
        virtual void visit(const main_volume&           , const operation&) {}
        virtual void visit(const transpose&             , const operation&) {}
        virtual void visit(const pitch_bend&            , const operation&) {}
        virtual void visit(const pitch_bend_range&      , const operation&) {}
        virtual void visit(const priority&              , const operation&) {}
        virtual void visit(const note_wait&             , const operation&) {}
        virtual void visit(const tie&                   , const operation&) {}
        virtual void visit(const portamento_control&    , const operation&) {}
        virtual void visit(const modulation_type&       , const operation&) {}
        virtual void visit(const modulation_depth&      , const operation&) {}
        virtual void visit(const modulation_speed&      , const operation&) {}
        virtual void visit(const modulation_range&      , const operation&) {}
        virtual void visit(const modulation_delay&      , const operation&) {}
        virtual void visit(const portamento_switch&     , const operation&) {}
        virtual void visit(const portamento_time&       , const operation&) {}
        virtual void visit(const attack&                , const operation&) {}
        virtual void visit(const decay&                 , const operation&) {}
        virtual void visit(const sustain&               , const operation&) {}
        virtual void visit(const release&               , const operation&) {}
        virtual void visit(const loop_start&            , const operation&) {}
        virtual void visit(const loop_end&              , const operation&) {}
        virtual void visit(const expression&            , const operation&) {}
        virtual void visit(const print&                 , const operation&) {}
        virtual void visit(const surround_pan&          , const operation&) {}
        virtual void visit(const low_pass_filter_cutoff&, const operation&) {}
        virtual void visit(const effect_send_a&         , const operation&) {}
        virtual void visit(const effect_send_b&         , const operation&) {}
        virtual void visit(const effect_send_c&         , const operation&) {}
        virtual void visit(const main_send&             , const operation&) {}
        virtual void visit(const initial_pan&           , const operation&) {}
        virtual void visit(const mute&                  , const operation&) {}
        virtual void visit(const damper&                , const operation&) {}
        virtual void visit(const tempo&                 , const operation&) {}
        virtual void visit(const sweep_pitch&           , const operation&) {}
        virtual void visit(const set_v&                 , const operation&) {}
        virtual void visit(const add_v&                 , const operation&) {}
        virtual void visit(const sub_v&                 , const operation&) {}
        virtual void visit(const mul_v&                 , const operation&) {}
        virtual void visit(const div_v&                 , const operation&) {}
        virtual void visit(const shift_v&               , const operation&) {}
        virtual void visit(const rand_v&                , const operation&) {}
        virtual void visit(const and_v&                 , const operation&) {}
        virtual void visit(const or_v&                  , const operation&) {}
        virtual void visit(const xor_v&                 , const operation&) {}
        virtual void visit(const not_v&                 , const operation&) {}
        virtual void visit(const mod_v&                 , const operation&) {}
        virtual void visit(const eq_v&                  , const operation&) {}
        virtual void visit(const ge_v&                  , const operation&) {}
        virtual void visit(const gt_v&                  , const operation&) {}
        virtual void visit(const le_v&                  , const operation&) {}
        virtual void visit(const lt_v&                  , const operation&) {}
        virtual void visit(const ne_v&                  , const operation&) {}
        virtual void visit(const userproc_v&            , const operation&) {}
        virtual void visit(const environment_reset&     , const operation&) {}
        virtual void visit(const ret&                   , const operation&) {}
        virtual void visit(const fin&                   , const operation&) {}
    };
}
}
}

#endif

