#include "Framework/Framework.hpp"

#include <Opx/TypeTraits.hpp>

class A {};
class B final {};
union U final {
    int x;
    double d;
};

TEST_CASE(TypeTraits, IsFinal) {
    TEST_EXPECT_TRUE(Opx::IsFinal_V<A> == false);
    TEST_EXPECT_TRUE(Opx::IsFinal_V<B> == true);
    TEST_EXPECT_TRUE(Opx::IsFinal_V<U> == true);
}