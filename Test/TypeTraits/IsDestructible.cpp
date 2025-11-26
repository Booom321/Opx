#include "Framework/Framework.hpp"

#include <Opx/TypeTraits.hpp>

struct Foo {
    std::string str;
    ~Foo() noexcept {};
};

struct Bar {
    ~Bar() = default;
};

TEST_CASE(TypeTraits, IsDestructible) {
    TEST_EXPECT_TRUE(Opx::IsDestructible_V<std::string> == true);
    TEST_EXPECT_TRUE(Opx::IsTriviallyDestructible_V<Foo> == false);
    TEST_EXPECT_TRUE(Opx::IsNothrowDestructible_V<Foo> == true);
    TEST_EXPECT_TRUE(Opx::IsTriviallyDestructible_V<Bar> == true);
}