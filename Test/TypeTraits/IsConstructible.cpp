#include "Framework/Framework.hpp"

#include <Opx/TypeTraits.hpp>

class Foo {
    int v1;
    double v2;

public:
    Foo(int n) : v1(n), v2() {}
    Foo(int n, double f) noexcept : v1(n), v2(f) {}
};

struct S1 {
    std::string s;
};

struct S2 {
    int n;
    S2() = default;
};

struct S3 {
    S3(const S3&) = delete;
};

struct S4 {
    S4(S4&) {};
};

struct Ex1 {
    std::string str;
};

struct Ex2 {
    int n;
    Ex2(Ex2&&) = default;
};

struct NoMove1 {
    NoMove1(const NoMove1&) {}
};

struct NoMove2 {
    NoMove2(NoMove2&) {}
};

TEST_CASE(TypeTraits, IsConstructible) {
    TEST_EXPECT_TRUE(Opx::IsTriviallyConstructible_V<Foo, const Foo&>);
    TEST_EXPECT_FALSE(Opx::IsTriviallyConstructible_V<Foo, int>);
    TEST_EXPECT_TRUE(Opx::IsConstructible_V<Foo, int>);
    TEST_EXPECT_FALSE(Opx::IsNothrowConstructible_V<Foo, int>);
    TEST_EXPECT_TRUE(Opx::IsNothrowConstructible_V<Foo, int, double>);

    TEST_EXPECT_TRUE(Opx::IsDefaultConstructible_V<S1> == true);
    TEST_EXPECT_TRUE(Opx::IsTriviallyDefaultConstructible_V<S1> == false);
    TEST_EXPECT_TRUE(Opx::IsTriviallyDefaultConstructible_V<S2> == true);
    TEST_EXPECT_TRUE(Opx::IsNothrowDefaultConstructible_V<S2> == true);

    TEST_EXPECT_TRUE(Opx::IsCopyConstructible_V<S1>);
    TEST_EXPECT_FALSE(Opx::IsTriviallyCopyConstructible_V<S1>);
    TEST_EXPECT_TRUE(Opx::IsTriviallyCopyConstructible_V<S2>);
    TEST_EXPECT_TRUE(Opx::IsNothrowCopyConstructible_V<S2>);
    TEST_EXPECT_FALSE(Opx::IsCopyConstructible_V<S3>);
    TEST_EXPECT_FALSE(Opx::IsCopyConstructible_V<S4>);

    TEST_EXPECT_TRUE(Opx::IsMoveConstructible_V<Ex1>);
    TEST_EXPECT_FALSE(Opx::IsTriviallyMoveConstructible_V<Ex1>);
    TEST_EXPECT_TRUE(Opx::IsNothrowMoveConstructible_V<Ex1>);
    TEST_EXPECT_TRUE(Opx::IsMoveConstructible_V<Ex2>);
    TEST_EXPECT_TRUE(Opx::IsTriviallyMoveConstructible_V<Ex2>);
    TEST_EXPECT_TRUE(Opx::IsNothrowMoveConstructible_V<Ex2>);
    TEST_EXPECT_TRUE(Opx::IsMoveConstructible_V<NoMove1>);
    TEST_EXPECT_FALSE(Opx::IsTriviallyMoveConstructible_V<NoMove1>);
    TEST_EXPECT_FALSE(Opx::IsNothrowMoveConstructible_V<NoMove1>);
    TEST_EXPECT_FALSE(Opx::IsMoveConstructible_V<NoMove2>);
    TEST_EXPECT_FALSE(Opx::IsTriviallyMoveConstructible_V<NoMove2>);
    TEST_EXPECT_FALSE(Opx::IsNothrowMoveConstructible_V<NoMove2>);
}