#include <fstream>
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <brtools/data/sequence/events.h>
#include <brtools/data/sequence/sequence.h>
#include <brtools/data/sequence/visitor.h>
#include <brtools/error/integrity_error.h>

using namespace std;
using namespace brtools::data::sequence;
using brtools::error::integrity_error;

struct event_printer : visitor
{
    event_printer()  { cout << left;  }
    ~event_printer() { cout << right; }

    static constexpr size_t line_num_width = 8;
    static constexpr size_t event_width = 20;
    static constexpr size_t attr_width = 20;


    void line_num() { cout << left << setw(line_num_width) << ++evt_count; } 
    static auto event_name() { return setw(event_width); }
    static auto attr_sl()    { return setw(attr_width); }
    static auto attr_nl()    { return setw(line_num_width + event_width + attr_width); }

    void visit(const note_on& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "note on"
             << right << attr_sl() <<     "note: " << static_cast<int>(e.note())     << endl
                      << attr_nl() <<    "pitch: " << static_cast<int>(e.pitch())    << endl
                      << attr_nl() << "velocity: " << static_cast<int>(e.velocity()) << endl;
    }

    void visit(const brtools::data::sequence::wait& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "wait" 
             << right << attr_sl() << "tick count: " << static_cast<int>(e.tick_count()) << endl;
    }

    void visit(const program& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "program"
             << right << attr_sl() << "number: " << static_cast<int>(e.program_no()) << endl
                      << attr_nl() <<   "bank: " << static_cast<int>(e.bank())       << endl;
    }
    void visit(const track_usage& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "track usage"
             << right << attr_sl() << "tracks used: "; 
        for (size_t track_id = 0; track_id < 16; ++track_id) if (e.is_track_used(track_id))
        {
            cout << static_cast<int>(track_id) << " ";
        }
        cout << endl;
    }
    void visit(const open_track& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "open track"
             << right << attr_sl() << "track no: " << static_cast<int>(e.track_no()) << endl;
    }
    void visit(const jump& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "jump" << endl;
    }
    void visit(const call& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "call" << endl;
    }
    void visit(const brtools::data::sequence::random& e, const operation& op) override
    {
        cout << left << setw(line_num_width) << ""
             << setw(event_width) << "random"
             << right << attr_sl() << "lower bound: " << static_cast<int>(e.lower_bound()) << endl
                      << attr_nl() << "upper bound: " << static_cast<int>(e.upper_bound()) << endl;

        e.event().accept(*this, op);
    }
    void visit(const variable& e, const operation& op) override
    {
        cout << left << setw(line_num_width) << ""
             << setw(event_width) << "variable"
             << right << attr_sl() << "slot: " << static_cast<int>(e.slot()) << endl;

        e.event().accept(*this, op);
    }
    void visit(const if_statement& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "if" << endl;
    }
    void visit(const brtools::data::sequence::time& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "time" << endl;
    }
    void visit(const time_random& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "time random" << endl;
    }
    void visit(const time_variable& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "time variable" << endl;
    }
    void visit(const timebase& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "timebase" << endl;
    }
    void visit(const environment_hold& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "environment hold" << endl;
    }
    void visit(const polyphony& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << ( e.enabled() ? "polyphony" : "monophony" ) << endl;
    }
    void visit(const velocity_range& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "velocity range" << endl;
    }
    void visit(const biquad_type& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "biquad type" << endl;
    }
    void visit(const biquad_value& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "biquad value" << endl;
    }
    void visit(const pan& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "pan"
             << right << attr_sl() << "position: " << static_cast<int>(e.position()) << endl;
    }
    void visit(const volume& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "volume"
             << right << attr_sl() << "position: " << static_cast<int>(e.position()) << endl;
    }
    void visit(const main_volume& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "main volume"
             << right << attr_sl() << "position: " << static_cast<int>(e.position()) << endl;
    }
    void visit(const transpose& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "transpose" << endl;
    }
    void visit(const pitch_bend& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "pitch bend"
             << right << attr_sl() << "position: " << static_cast<int>(e.position()) << endl;
    }
    void visit(const pitch_bend_range& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "pitch bend range"
             << right << attr_sl() << "semitones: " << static_cast<int>(e.semitones()) << endl;
    }
    void visit(const priority& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "priority"
             << right << attr_sl() << "value: " << static_cast<int>(e.priority_value()) << endl;
    }
    void visit(const note_wait& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "note wait"
             << right << attr_sl() << "duration: " << static_cast<int>(e.duration()) << endl;
    }
    void visit(const brtools::data::sequence::tie& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "tie"
             << right << attr_sl() << "duration: " << static_cast<int>(e.duration()) << endl;
    }
    void visit(const portamento_control& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "portamento control"
             << right << attr_sl() << "amount: " << static_cast<int>(e.amount()) << endl;
    }
    void visit(const modulation_type& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "modulation"
             << right << attr_sl() << "type: ";
        switch (e.type())
        {
            case modulation_type::mod_type::pitch:  cout << "pitch";  break;
            case modulation_type::mod_type::volume: cout << "volume"; break;
            case modulation_type::mod_type::pan:    cout << "pan";    break;
            default: break;
        }
        cout << endl;
    }
    void visit(const modulation_depth& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "modulation"
             << right << attr_sl() << "depth: " << static_cast<int>(e.value()) << endl;
    }
    void visit(const modulation_speed& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "modulation"
             << right << attr_sl() << "speed: " << static_cast<int>(e.value()) << endl;
    }
    void visit(const modulation_range& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "modulation"
             << right << attr_sl() << "range: " << static_cast<int>(e.value()) << endl;
    }
    void visit(const modulation_delay& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "modulation"
             << right << attr_sl() << "delay: " << static_cast<int>(e.value()) << endl;
    }
    void visit(const portamento_switch& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << ("portamento " + string(e.on() ? "on" : "off")) << endl;
    }
    void visit(const portamento_time& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "portamento"
             << right << attr_sl() << "time: " << static_cast<int>(e.rate()) << endl;
    }
    void visit(const attack& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "attack"
             << right << attr_sl() << "time: " << static_cast<int>(e.time()) << endl;
    }
    void visit(const brtools::data::sequence::decay& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "decay"
             << right << attr_sl() << "time: " << static_cast<int>(e.time()) << endl;
    }
    void visit(const sustain& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "sustain"
             << right << attr_sl() << "time: " << static_cast<int>(e.time()) << endl;
    }
    void visit(const release& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "release"
             << right << attr_sl() << "time: " << static_cast<int>(e.time()) << endl;
    }
    void visit(const loop_start& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "loop"
             << right << attr_sl() << "times: " << static_cast<int>(e.times()) << endl;
    }
    void visit(const loop_end& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "loop end" << endl;
    }
    void visit(const expression& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "expression"
             << right << attr_sl() << "percent: " << (e.percent() * 100 / static_cast<uint8_t>(~0u)) << endl;
    }
    void visit(const print& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "print" << endl;
    }
    void visit(const surround_pan& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "surround pan" << endl;
    }
    void visit(const low_pass_filter_cutoff& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "low pass filter cutoff" << endl;
    }
    void visit(const effect_send_a& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "fxsend a" << endl;
    }
    void visit(const effect_send_b& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "fxsend b" << endl;
    }
    void visit(const effect_send_c& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "fxsend c" << endl;
    }
    void visit(const main_send& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "main send" << endl;
    }
    void visit(const initial_pan& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "initial pan" << endl;
    }
    void visit(const mute& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "mute" << endl;
    }
    void visit(const damper& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "damper" << endl;
    }
    void visit(const tempo& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "tempo"
             << right << attr_sl() << "bpm: " << static_cast<int>(e.bpm()) << endl;
    }
    void visit(const sweep_pitch& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "sweep pitch"
             << endl;
    }
    void visit(const set_v& e, const operation&) override
    {
        line_num(); 
        cout << "v(" << static_cast<int>(e.l_operand_var()) << ")" << " = " << e.r_operand_val() << endl;
    }
    void visit(const add_v& e, const operation&) override
    {
        line_num(); 
        cout << "v(" << static_cast<int>(e.l_operand_var()) << ")" << " += " << e.r_operand_val() << endl;
    }
    void visit(const sub_v& e, const operation&) override
    {
        line_num(); 
        cout << "v(" << static_cast<int>(e.l_operand_var()) << ")" << " -= " << e.r_operand_val() << endl;
    }
    void visit(const mul_v& e, const operation&) override
    {
        line_num(); 
        cout << "v(" << static_cast<int>(e.l_operand_var()) << ")" << " *= " << e.r_operand_val() << endl;
    }
    void visit(const div_v& e, const operation&) override
    {
        line_num(); 
        cout << "v(" << static_cast<int>(e.l_operand_var()) << ")" << " /= " << e.r_operand_val() << endl;
    }
    void visit(const shift_v& e, const operation&) override
    {
        line_num(); 
        cout << "v(" << static_cast<int>(e.l_operand_var()) << ")" << " <<= " << e.r_operand_val() << endl;
    }
    void visit(const rand_v& e, const operation&) override
    {
        line_num(); 
        cout << "v(" << static_cast<int>(e.l_operand_var()) << ")" << " = ? " << endl;
    }
    void visit(const and_v& e, const operation&) override
    {
        line_num(); 
        cout << "v(" << static_cast<int>(e.l_operand_var()) << ")" << " &= " << e.r_operand_val() << endl;
    }
    void visit(const or_v& e, const operation&) override
    {
        line_num(); 
        cout << "v(" << static_cast<int>(e.l_operand_var()) << ")" << " |= " << e.r_operand_val() << endl;
    }
    void visit(const xor_v& e, const operation&) override
    {
        line_num(); 
        cout << "v(" << static_cast<int>(e.l_operand_var()) << ")" << " ^= " << e.r_operand_val() << endl;
    }
    void visit(const not_v& e, const operation&) override
    {
        line_num(); 
        cout << "~= v(" << static_cast<int>(e.l_operand_var()) << ")" << endl;
    }
    void visit(const mod_v& e, const operation&) override
    {
        line_num(); 
        cout << "v(" << static_cast<int>(e.l_operand_var()) << ")" << " %= " << e.r_operand_val() << endl;
    }
    void visit(const eq_v& e, const operation&) override
    {
        line_num(); 
        cout << "v(" << static_cast<int>(e.l_operand_var()) << ")" << " == " << e.r_operand_val() << endl;
    }
    void visit(const ge_v& e, const operation&) override
    {
        line_num(); 
        cout << "v(" << static_cast<int>(e.l_operand_var()) << ")" << " >= " << e.r_operand_val() << endl;
    }
    void visit(const gt_v& e, const operation&) override
    {
        line_num(); 
        cout << "v(" << static_cast<int>(e.l_operand_var()) << ")" << " >  " << e.r_operand_val() << endl;
    }
    void visit(const le_v& e, const operation&) override
    {
        line_num(); 
        cout << "v(" << static_cast<int>(e.l_operand_var()) << ")" << " <= " << e.r_operand_val() << endl;
    }
    void visit(const lt_v& e, const operation&) override
    {
        line_num(); 
        cout << "v(" << static_cast<int>(e.l_operand_var()) << ")" << " <  " << e.r_operand_val() << endl;
    }
    void visit(const ne_v& e, const operation&) override
    {
        line_num(); 
        cout << "v(" << static_cast<int>(e.l_operand_var()) << ")" << " != " << e.r_operand_val() << endl;
    }
    void visit(const userproc_v& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "user process" << endl;
    }
    void visit(const environment_reset& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "environment reset" << endl;
    }
    void visit(const ret& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "return" << endl;
    }
    void visit(const fin& e, const operation&) override
    {
        line_num(); 
        cout << setw(event_width) << "fin" << endl;
    }

    size_t evt_count = 0;
};

void print_file(istream& stm)
{
    const auto seq = brtools::data::sequence::sequence::make_sequence(stm);
    event_printer p;
    seq.traverse(p);
}

int main(int argc, char* argv[]) try
{
    if (argc == 2)
    {
        // the argument should be the filename
        ifstream f(argv[1]);
        if (!f.fail())
        {
            print_file(f);
            return EXIT_SUCCESS;
        }
    }

    // otherwise, try reading from stdin
    print_file(cin);
    return EXIT_SUCCESS;
}
catch (integrity_error err)
{
    cerr << "Integrity error: " << err.what() << endl;
    return EXIT_FAILURE;
}
