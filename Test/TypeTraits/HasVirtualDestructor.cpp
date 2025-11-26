#include "Framework/Framework.hpp"

#include <Opx/TypeTraits.hpp>

struct S {};
struct B {
    virtual ~B() {}
};
struct D : B {
    ~D() {}
};

TEST_CASE(TypeTraits, HasVirtualDestructor) {
    TEST_EXPECT_TRUE(!Opx::HasVirtualDestructor_V<S>);
    TEST_EXPECT_TRUE(Opx::HasVirtualDestructor_V<B>);
    TEST_EXPECT_TRUE(Opx::HasVirtualDestructor_V<D>);
}