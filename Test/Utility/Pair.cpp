#include "Framework/Framework.hpp"

#include <Opx/Utility/Pair.hpp>

#include <string_view>

using namespace std::literals;
static OPX_CONSTEXPR Opx::Pair<float, int> p1{1.f, 2};
static OPX_CONSTEXPR Opx::Pair<float, int> p2{p1};
static OPX_CONSTEXPR Opx::Pair<std::string_view, std::string_view> p3{"foo"sv, "bar"sv};

static_assert(p1.first == 1.f);
static_assert(p1.second == 2);
static_assert(p2.first == p1.first);
static_assert(p2.second == p2.second);
static_assert(p1 == p2);

static_assert(p3.first == "foo");
static_assert(p3.second == "bar");

TEST_CASE(Utility, Pair) {
    using namespace Opx;
    Pair<int, int> p01{};
    TEST_EXPECT_EQ(p01.first, 0);
    TEST_EXPECT_EQ(p01.second, 0);

    p01 = Pair<Int64, int>{2ll, 2};
    TEST_EXPECT_EQ(p01.first, 2);
    TEST_EXPECT_EQ(p01.second, 2);

    std::string s("bar");
    Pair<std::string_view, std::string_view> p02{static_cast<const char*>("foo"), s};
    TEST_EXPECT_EQ(p02.first, "foo");
    TEST_EXPECT_EQ(p02.second, "bar");

    s = "other 2";
    p02 = Pair<const char*, std::string>("other 1", s);
    TEST_EXPECT_EQ(p02.first, "other 1");
    TEST_EXPECT_EQ(p02.second, "other 2");

    Pair<std::string_view, std::string_view> p03{"other 1", "other 2"};

    p01 = Pair<int, int>{1, 2};
    Pair<int, int> p04{2, 3};
    TEST_EXPECT_EQ(p02, p03);
    TEST_EXPECT_NEQ(p01, p04);

    Pair<std::string_view, std::string_view> p05{Pair<std::string, const char*>{"hello", "world"}};
    TEST_EXPECT_STR_EQ(p05.first.data(), "hello");
    TEST_EXPECT_STR_EQ(p05.second.data(), "world");
}