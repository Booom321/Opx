#include "Framework/Framework.hpp"

#include <Opx/TypeTraits.hpp>

struct A {
    int m;
};
struct B {
    virtual void Foo();
    virtual ~B();
};
struct C {
    virtual void Foo() = 0;
    virtual ~C();
};
struct D : C {};
struct E : C {
    void Foo() override {}
};

TEST_CASE(TypeTraits, IsAbstract) {
    TEST_EXPECT_TRUE(Opx::IsAbstract_V<A> == false);
    TEST_EXPECT_TRUE(Opx::IsAbstract_V<B> == false);
    TEST_EXPECT_TRUE(Opx::IsAbstract_V<C> == true);
    TEST_EXPECT_TRUE(Opx::IsAbstract_V<D> == true);
    TEST_EXPECT_TRUE(Opx::IsAbstract_V<E> == false);
}