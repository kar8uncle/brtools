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

    MOCK_METHOD2(visit, void( const brtools::data::sequence::note_on&               , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::wait&                  , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::program&               , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::track_usage&           , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::open_track&            , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::jump&                  , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::call&                  , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::random&                , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::variable&              , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::if_statement&          , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::time&                  , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::time_random&           , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::time_variable&         , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::timebase&              , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::environment_hold&      , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::polyphony&             , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::velocity_range&        , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::biquad_type&           , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::biquad_value&          , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::pan&                   , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::volume&                , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::main_volume&           , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::transpose&             , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::pitch_bend&            , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::pitch_bend_range&      , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::priority&              , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::note_wait&             , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::tie&                   , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::portamento_control&    , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::modulation_type&       , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::modulation_depth&      , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::modulation_speed&      , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::modulation_range&      , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::modulation_delay&      , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::portamento_switch&     , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::portamento_time&       , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::attack&                , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::decay&                 , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::sustain&               , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::release&               , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::loop_start&            , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::loop_end&              , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::expression&            , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::print&                 , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::surround_pan&          , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::low_pass_filter_cutoff&, const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::effect_send_a&         , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::effect_send_b&         , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::effect_send_c&         , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::main_send&             , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::initial_pan&           , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::mute&                  , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::damper&                , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::tempo&                 , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::sweep_pitch&           , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::set_v&                 , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::add_v&                 , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::sub_v&                 , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::mul_v&                 , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::div_v&                 , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::shift_v&               , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::rand_v&                , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::and_v&                 , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::or_v&                  , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::xor_v&                 , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::not_v&                 , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::mod_v&                 , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::eq_v&                  , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::ge_v&                  , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::gt_v&                  , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::le_v&                  , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::lt_v&                  , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::ne_v&                  , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::userproc_v&            , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::environment_reset&     , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::ret&                   , const operation& ));
    MOCK_METHOD2(visit, void( const brtools::data::sequence::fin&                   , const operation& ));
};

#endif
