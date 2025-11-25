#include "Framework/Framework.hpp"

#include <Opx/TypeTraits.hpp>

class A {};
class B {
    int i;
};
enum C : int {
};
enum class D : int {
};

TEST_CASE(TypeTraits, IsSigned) {
    TEST_EXPECT_TRUE(Opx::IsSigned_V<A> == false);
    TEST_EXPECT_TRUE(Opx::IsSigned_V<B> == false);
    TEST_EXPECT_TRUE(Opx::IsSigned_V<C> == false);
    TEST_EXPECT_TRUE(Opx::IsSigned_V<D> == false);

    TEST_EXPECT_TRUE(Opx::IsSigned_V<signed int> == true && Opx::IsSigned_V<signed int> == true && Opx::IsSigned_V<signed int> == true &&
                     Opx::IsSigned_V<signed int> == true && Opx::IsSigned_V<unsigned int> == false && Opx::IsSigned_V<float> == true &&
                     Opx::IsSigned_V<bool> == false && Opx::IsSigned_V<signed char> == true && Opx::IsSigned_V<unsigned char> == false);
}