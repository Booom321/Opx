#include "Framework/Framework.hpp"

#include <Opx/TypeTraits.hpp>
struct A {
    int m;
};

class B : public A {
    int m;
};

struct C {
    virtual void Foo();
    virtual ~C() = default;
};
TEST_CASE(TypeTraits, IsStandardLayout) {
    TEST_EXPECT_TRUE(Opx::IsStandardLayout_V<A> == true);
    TEST_EXPECT_TRUE(Opx::IsStandardLayout_V<B> == false);
    TEST_EXPECT_TRUE(Opx::IsStandardLayout_V<C> == false);
}