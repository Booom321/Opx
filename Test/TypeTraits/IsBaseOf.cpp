#include "Framework/Framework.hpp"

#include <Opx/TypeTraits.hpp>

class A {};
class B : A {};
class C : B {};
class D {};
union E {};
using I = int;

TEST_CASE(TypeTraits, IsBaseOf) {
    TEST_EXPECT_TRUE(Opx::IsBaseOf_V<A, A> == true && Opx::IsBaseOf_V<A, B> == true && Opx::IsBaseOf_V<A, C> == true &&
                     Opx::IsBaseOf_V<A, D> != true && Opx::IsBaseOf_V<B, A> != true && Opx::IsBaseOf_V<E, E> != true &&
                     Opx::IsBaseOf_V<I, I> != true);
}