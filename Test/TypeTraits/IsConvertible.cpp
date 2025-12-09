#include "Framework/Framework.hpp"

#include <Opx/TypeTraits.hpp>

#include <string>
#include <string_view>

class E {
public:
    template <class T>
    E(T&&) {}
};
class A {};
class B : public A {};
class C {};
class D {
public:
    operator C() { return c; }
    C c;
};

TEST_CASE(TypeTraits, IsConvertible) {
    TEST_EXPECT_TRUE(Opx::IsConvertible_V<B*, A*>);
    TEST_EXPECT_TRUE(Opx::IsConvertible_V<D, C>);
    TEST_EXPECT_TRUE(Opx::IsConvertible_V<A, E>);

    TEST_EXPECT_TRUE(Opx::IsConvertible_V<std::string, std::string_view>);

    TEST_EXPECT_TRUE(!Opx::IsConvertible_V<A*, B*>);
    TEST_EXPECT_TRUE(!Opx::IsConvertible_V<B*, C*>);
    TEST_EXPECT_TRUE(!Opx::IsConvertible_V<std::string_view, std::string>);
}