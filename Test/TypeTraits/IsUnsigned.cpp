#include "Framework/Framework.hpp"

#include <Opx/TypeTraits.hpp>

class A {};
enum B : unsigned {
};
enum class C : unsigned {
};
struct S {
    unsigned p : 1;
    int q : 1;
};

TEST_CASE(TypeTraits, IsUnsigned) {
    TEST_EXPECT_TRUE(Opx::IsUnsigned_V<A> == false);
    TEST_EXPECT_TRUE(Opx::IsUnsigned_V<B> == false);
    TEST_EXPECT_TRUE(Opx::IsUnsigned_V<C> == false);
    TEST_EXPECT_TRUE(Opx::IsUnsigned_V<decltype(S::p)> != Opx::IsUnsigned_V<decltype(S::q)>);

    TEST_EXPECT_TRUE(Opx::IsUnsigned_V<float> == false && Opx::IsUnsigned_V<signed int> == false &&
                     Opx::IsUnsigned_V<unsigned int> == true && Opx::IsUnsigned_V<bool> == true);
}