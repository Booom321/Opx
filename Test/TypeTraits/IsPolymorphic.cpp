#include "Framework/Framework.hpp"

#include <Opx/TypeTraits.hpp>

struct A {
    int m;
};
struct B {
    virtual void Foo();
    virtual ~B();
};
struct C : B {};
struct D {
    virtual ~D() = default;
};
struct E : A {};
struct F : virtual A {};
struct AX : A {};
struct AY : A {};
struct XY : virtual AX, virtual AY {};

TEST_CASE(TypeTraits, IsPolymorphic) {
    TEST_EXPECT_TRUE(!Opx::IsPolymorphic_V<A>);
    TEST_EXPECT_TRUE(Opx::IsPolymorphic_V<B>);
    TEST_EXPECT_TRUE(Opx::IsPolymorphic_V<C>);
    TEST_EXPECT_TRUE(Opx::IsPolymorphic_V<D>);
    TEST_EXPECT_TRUE(!Opx::IsPolymorphic_V<E>);
    TEST_EXPECT_TRUE(!Opx::IsPolymorphic_V<F>);
    TEST_EXPECT_TRUE(!Opx::IsPolymorphic_V<XY>);
}