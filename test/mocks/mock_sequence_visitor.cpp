#include <mocks/mock_sequence_visitor.h>

using namespace ::testing;
using namespace brtools::data::sequence;

mock_sequence_visitor::mock_sequence_visitor()
{
    {
        // Action for set_v through userproc_v
        const auto PerformBinaryOperation = Invoke([](const auto& binop, const auto&) { binop(); });

        ON_CALL(*this, visit(A<const set_v&     >(), A<const operation&>())).WillByDefault(PerformBinaryOperation);
        ON_CALL(*this, visit(A<const add_v&     >(), A<const operation&>())).WillByDefault(PerformBinaryOperation);
        ON_CALL(*this, visit(A<const sub_v&     >(), A<const operation&>())).WillByDefault(PerformBinaryOperation);
        ON_CALL(*this, visit(A<const mul_v&     >(), A<const operation&>())).WillByDefault(PerformBinaryOperation);
        ON_CALL(*this, visit(A<const div_v&     >(), A<const operation&>())).WillByDefault(PerformBinaryOperation);
        ON_CALL(*this, visit(A<const shift_v&   >(), A<const operation&>())).WillByDefault(PerformBinaryOperation);
        ON_CALL(*this, visit(A<const rand_v&    >(), A<const operation&>())).WillByDefault(PerformBinaryOperation);
        ON_CALL(*this, visit(A<const and_v&     >(), A<const operation&>())).WillByDefault(PerformBinaryOperation);
        ON_CALL(*this, visit(A<const or_v&      >(), A<const operation&>())).WillByDefault(PerformBinaryOperation);
        ON_CALL(*this, visit(A<const xor_v&     >(), A<const operation&>())).WillByDefault(PerformBinaryOperation);
        ON_CALL(*this, visit(A<const not_v&     >(), A<const operation&>())).WillByDefault(PerformBinaryOperation);
        ON_CALL(*this, visit(A<const mod_v&     >(), A<const operation&>())).WillByDefault(PerformBinaryOperation);
        ON_CALL(*this, visit(A<const eq_v&      >(), A<const operation&>())).WillByDefault(PerformBinaryOperation);
        ON_CALL(*this, visit(A<const ge_v&      >(), A<const operation&>())).WillByDefault(PerformBinaryOperation);
        ON_CALL(*this, visit(A<const gt_v&      >(), A<const operation&>())).WillByDefault(PerformBinaryOperation);
        ON_CALL(*this, visit(A<const le_v&      >(), A<const operation&>())).WillByDefault(PerformBinaryOperation);
        ON_CALL(*this, visit(A<const lt_v&      >(), A<const operation&>())).WillByDefault(PerformBinaryOperation);
        ON_CALL(*this, visit(A<const ne_v&      >(), A<const operation&>())).WillByDefault(PerformBinaryOperation);
        ON_CALL(*this, visit(A<const userproc_v&>(), A<const operation&>())).WillByDefault(PerformBinaryOperation);
    }

    // some standard operations for certain events
    ON_CALL(*this, visit(A<const jump&>(), A<const operation&>()))
        .WillByDefault(Invoke([](const auto& j, const auto& vop) { vop.jump(j.destination()); }));

    ON_CALL(*this, visit(A<const call&>(), A<const operation&>()))
        .WillByDefault(Invoke([](const auto& c, const auto& vop) { vop.call(c.destination()); }));
    ON_CALL(*this, visit(A<const ret& >(), A<const operation&>()))
        .WillByDefault(Invoke([](const auto&, const auto& vop)   { vop.return_from_call();    }));

    // not setting default actions for loop end since we may want to actually loop a few times
    ON_CALL(*this, visit(A<const loop_start&>(), A<const operation&>()))
        .WillByDefault(Invoke([](const auto&, const auto& vop)   { vop.start_loop();         }));
}
