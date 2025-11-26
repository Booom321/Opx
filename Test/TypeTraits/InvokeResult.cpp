#include "Framework/Framework.hpp"

#include <Opx/TypeTraits.hpp>

struct S {
    int f(int) { return 1; }
    double g() const { return 0.0; }
};

int h(float) { return 5; }

struct Fun {
    long operator()(int) { return 2; }
};

TEST_CASE(TypeTraits, InvokeResult) {
    TEST_EXPECT_TRUE(Opx::IsSame_V<Opx::InvokeResult_T<decltype(h), float>, int> && Opx::IsSame_V<Opx::InvokeResult_T<int (*)(float), float>, int> &&
                     Opx::IsSame_V<Opx::InvokeResult_T<Fun, int>, long> && Opx::IsSame_V<Opx::InvokeResult_T<decltype(&S::f), S&, int>, int> &&
                     Opx::IsSame_V<Opx::InvokeResult_T<decltype(&S::g), const S&>, double> && Opx::IsSame_V<Opx::InvokeResult_T<void (*)()>, void>);
}