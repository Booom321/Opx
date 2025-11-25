#include "Framework/Framework.hpp"

#include <Opx/TypeTraits.hpp>

struct A {};

struct B {
    int m;
};

struct C {
    static int m;
};

struct D {
    virtual ~D();
};

union E {};

struct F {
    int : 0;
};

TEST_CASE(TypeTraits, IsEmpty) {
    TEST_EXPECT_TRUE(Opx::IsEmpty_V<A> == true);
    TEST_EXPECT_TRUE(Opx::IsEmpty_V<B> == false);
    TEST_EXPECT_TRUE(Opx::IsEmpty_V<C> == true);
    TEST_EXPECT_TRUE(Opx::IsEmpty_V<D> == false);
    TEST_EXPECT_TRUE(Opx::IsEmpty_V<E> == false);
    TEST_EXPECT_TRUE(Opx::IsEmpty_V<F> == true);
}