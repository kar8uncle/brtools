#ifndef BRTOOLS_TEST_MOCK_MOCK_SEQUENCE_VISITOR_H
#define BRTOOLS_TEST_MOCK_MOCK_SEQUENCE_VISITOR_H
#pragma once

#include <gmock/gmock.h>
#include <data/sequence/visitor.h>
#include <data/sequence/events.h>

class mock_sequence_visitor : public brtools::data::sequence::visitor
{
public:
    // Sets default actions for certain types
    mock_sequence_visitor();

    MOCK_METHOD1(visit, void( const brtools::data::sequence::note_on&                ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::wait&                   ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::program&                ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::track_usage&            ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::open_track&             ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::jump&                   ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::call&                   ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::random&                 ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::variable&               ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::if_statement&           ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::time&                   ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::time_random&            ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::time_variable&          ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::timebase&               ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::environment_hold&       ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::polyphony&              ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::velocity_range&         ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::biquad_type&            ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::biquad_value&           ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::pan&                    ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::volume&                 ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::main_volume&            ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::transpose&              ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::pitch_bend&             ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::pitch_bend_range&       ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::priority&               ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::note_wait&              ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::tie&                    ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::portamento_control&     ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::modulation_type&        ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::modulation_depth&       ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::modulation_speed&       ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::modulation_range&       ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::modulation_delay&       ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::portamento_switch&      ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::portamento_time&        ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::attack&                 ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::decay&                  ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::sustain&                ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::release&                ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::loop_start&             ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::loop_end&               ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::expression&             ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::print&                  ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::surround_pan&           ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::low_pass_filter_cutoff& ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::effect_send_a&          ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::effect_send_b&          ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::effect_send_c&          ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::main_send&              ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::initial_pan&            ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::mute&                   ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::damper&                 ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::tempo&                  ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::sweep_pitch&            ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::set_v&                  ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::add_v&                  ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::sub_v&                  ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::mul_v&                  ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::div_v&                  ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::shift_v&                ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::rand_v&                 ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::and_v&                  ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::or_v&                   ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::xor_v&                  ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::not_v&                  ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::mod_v&                  ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::eq_v&                   ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::ge_v&                   ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::gt_v&                   ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::le_v&                   ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::lt_v&                   ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::ne_v&                   ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::userproc_v&             ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::environment_reset&      ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::ret&                    ));
    MOCK_METHOD1(visit, void( const brtools::data::sequence::fin&                    ));
};

#endif
