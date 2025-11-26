#include "Framework/Framework.hpp"

#include <Opx/TypeTraits.hpp>

struct Foo {
    int n;
};

struct NoMove {
    NoMove& operator=(const NoMove&) { return *this; }
};

TEST_CASE(TypeTraits, IsAssignable) {
    TEST_EXPECT_FALSE(Opx::IsAssignable_V<int, int>);
    TEST_EXPECT_TRUE(Opx::IsAssignable_V<int&, int>);
    TEST_EXPECT_FALSE(Opx::IsAssignable_V<int, double>);
    TEST_EXPECT_TRUE(Opx::IsNothrowAssignable_V<int&, double>);
    TEST_EXPECT_TRUE(Opx::IsAssignable_V<std::string, double>);
    TEST_EXPECT_TRUE(Opx::IsTriviallyAssignable_V<Foo&, const Foo&>);

    TEST_EXPECT_TRUE(Opx::IsTriviallyCopyAssignable_V<Foo>);
    TEST_EXPECT_FALSE(Opx::IsCopyAssignable_V<int[2]>);
    TEST_EXPECT_TRUE(Opx::IsNothrowCopyAssignable_V<int>);

    TEST_EXPECT_TRUE(Opx::IsNothrowMoveAssignable_V<std::string>);
    TEST_EXPECT_FALSE(Opx::IsMoveAssignable_V<int[2]>);
    TEST_EXPECT_TRUE(Opx::IsTriviallyMoveAssignable_V<Foo>);
    TEST_EXPECT_TRUE(Opx::IsMoveAssignable_V<NoMove>);
    TEST_EXPECT_FALSE(Opx::IsNothrowMoveAssignable_V<NoMove>);
}