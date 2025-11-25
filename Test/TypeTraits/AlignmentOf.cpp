#include "Framework/Framework.hpp"

#include <Opx/TypeTraits.hpp>

struct A {};
struct B {
    std::int8_t p;
    std::int16_t q;
};

TEST_CASE(TypeTraits, AligmentOf) {
    TEST_EXPECT_EQ(Opx::AlignmentOf_V<A>, 1);
    TEST_EXPECT_EQ(Opx::AlignmentOf_V<B>, 2);
    TEST_EXPECT_EQ(Opx::AlignmentOf_V<int>, 4);
    TEST_EXPECT_EQ(Opx::AlignmentOf_V<double>, 8);
}