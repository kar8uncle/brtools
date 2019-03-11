#include <mocks/mock_sequence_visitor.h>

using namespace ::testing;
using namespace brtools::data::sequence;

mock_sequence_visitor::mock_sequence_visitor()
{
    // Action for set_v through userproc_v
    const auto PerformBinaryOperation = ::testing::Invoke([](const auto& binop) { binop(); });

    ON_CALL(*this, visit(A<const set_v&     >())).WillByDefault(PerformBinaryOperation);
    ON_CALL(*this, visit(A<const add_v&     >())).WillByDefault(PerformBinaryOperation);
    ON_CALL(*this, visit(A<const sub_v&     >())).WillByDefault(PerformBinaryOperation);
    ON_CALL(*this, visit(A<const mul_v&     >())).WillByDefault(PerformBinaryOperation);
    ON_CALL(*this, visit(A<const div_v&     >())).WillByDefault(PerformBinaryOperation);
    ON_CALL(*this, visit(A<const shift_v&   >())).WillByDefault(PerformBinaryOperation);
    ON_CALL(*this, visit(A<const rand_v&    >())).WillByDefault(PerformBinaryOperation);
    ON_CALL(*this, visit(A<const and_v&     >())).WillByDefault(PerformBinaryOperation);
    ON_CALL(*this, visit(A<const or_v&      >())).WillByDefault(PerformBinaryOperation);
    ON_CALL(*this, visit(A<const xor_v&     >())).WillByDefault(PerformBinaryOperation);
    ON_CALL(*this, visit(A<const not_v&     >())).WillByDefault(PerformBinaryOperation);
    ON_CALL(*this, visit(A<const mod_v&     >())).WillByDefault(PerformBinaryOperation);
    ON_CALL(*this, visit(A<const eq_v&      >())).WillByDefault(PerformBinaryOperation);
    ON_CALL(*this, visit(A<const ge_v&      >())).WillByDefault(PerformBinaryOperation);
    ON_CALL(*this, visit(A<const gt_v&      >())).WillByDefault(PerformBinaryOperation);
    ON_CALL(*this, visit(A<const le_v&      >())).WillByDefault(PerformBinaryOperation);
    ON_CALL(*this, visit(A<const lt_v&      >())).WillByDefault(PerformBinaryOperation);
    ON_CALL(*this, visit(A<const ne_v&      >())).WillByDefault(PerformBinaryOperation);
    ON_CALL(*this, visit(A<const userproc_v&>())).WillByDefault(PerformBinaryOperation);
}
