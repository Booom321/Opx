#include "Framework/Framework.hpp"

#include <Opx/TypeTraits.hpp>

struct A {
    int m;
};
struct B {
    B(B const&) {}
};
struct C {
    virtual void Foo();
    virtual ~C() = default;
};
struct D {
    int m;
    D(D const&) = default;
    D(int x) : m(x + 1) {}
};

TEST_CASE(TypeTraits, IsTriviallyCopyable) {
    TEST_EXPECT_TRUE(Opx::IsTriviallyCopyable_V<A> == true);
    TEST_EXPECT_TRUE(Opx::IsTriviallyCopyable_V<B> == false);
    TEST_EXPECT_TRUE(Opx::IsTriviallyCopyable_V<C> == false);
    TEST_EXPECT_TRUE(Opx::IsTriviallyCopyable_V<D> == true);
}