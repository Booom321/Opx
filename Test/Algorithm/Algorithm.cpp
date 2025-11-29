#include "Framework/Framework.hpp"

#include <Opx/Algorithm/AllOf.hpp>
#include <Opx/Algorithm/AnyOf.hpp>
#include <Opx/Algorithm/NoneOf.hpp>
#include <Opx/Algorithm/Contains.hpp>
#include <Opx/Algorithm/Equal.hpp>
#include <Opx/Algorithm/Find.hpp>
#include <Opx/Algorithm/FindLast.hpp>
#include <Opx/Algorithm/ForEach.hpp>
#include <Opx/Algorithm/Replace.hpp>

#include <Opx/Containers/Array.hpp>
#include <Opx/Containers/Vector.hpp>

static constexpr auto kArrayA = Opx::Array<Int32, 3>{1, 2, 3};
static constexpr auto kArrayB = Opx::Array<Int32, 3>{1, 2, 3};
static constexpr auto kArrayC = Opx::Array<Int32, 2>{7, 8};

TEST_CASE(Algorithm, Algorithm) {
    using namespace Opx;

    TEST_COMMENT("Contains");
    {
        static_assert(Algorithm::Contains(kArrayA.begin(), kArrayA.end(), 1));
        static_assert(!Algorithm::Contains(kArrayA.begin(), kArrayA.end(), 999));

        Vector<int> a{1, 2, 3};
        Vector<int> b{1, 2, 3};
        Vector<int> c{1, 2};
        TEST_EXPECT_TRUE(Algorithm::Contains(a.begin(), a.end(), 2));
        TEST_EXPECT_TRUE(!Algorithm::Contains(c.begin(), c.end(), 5));

        TEST_EXPECT_TRUE(
            Algorithm::ContainsIf(a.begin(), a.end(), [](int x) -> Bool { return x % 2 == 0; }));
        TEST_EXPECT_TRUE(
            !Algorithm::ContainsIf(c.begin(), c.end(), [](int x) -> Bool { return x % 3 == 0; }));

        TEST_EXPECT_TRUE(
            Algorithm::ContainsIfNot(a.begin(), a.end(), [](int x) -> Bool { return x == 1; }));
        TEST_EXPECT_TRUE(
            Algorithm::ContainsIfNot(c.begin(), c.end(), [](int x) -> Bool { return x == 999; }));
    }

    TEST_COMMENT("Equal");
    {
        static_assert(Algorithm::Equal(kArrayA.begin(), kArrayA.end(), kArrayB.begin()));
        static_assert(!Algorithm::Equal(kArrayA.begin(), kArrayA.end(), kArrayC.begin()));

        Vector<int> a{1, 2, 3};
        Vector<int> b{1, 2, 3};
        Vector<int> c{1, 2};
        TEST_EXPECT_TRUE(Algorithm::Equal(a.begin(), a.end(), b.begin()));
        TEST_EXPECT_TRUE(!Algorithm::Equal(a.begin(), a.end(), c.begin()));

        TEST_EXPECT_TRUE(Algorithm::Equal(a.begin(), a.end(), b.begin(),
                                          [](int lhs, int rhs) -> Bool { return lhs == rhs; }));
        TEST_EXPECT_TRUE(!Algorithm::Equal(a.begin(), a.end(), c.begin(),
                                           [](int lhs, int rhs) -> Bool { return lhs == rhs; }));
    }

    TEST_COMMENT("Find / FindLast");
    {
        static_assert(Algorithm::Find(kArrayA.begin(), kArrayA.end(), 2) == &kArrayA[1]);
        static_assert(Algorithm::Find(kArrayA.begin(), kArrayA.end(), 5) ==
                      &kArrayA[kArrayA.GetSize()]);

        static constexpr auto kArrayD = Opx::Array<Int32, 7>{1, 2, 3, 4, 2, 1, 3};
        static_assert(Algorithm::FindLast(kArrayD.begin(), kArrayD.end(), 2) == &kArrayD[4]);
        static_assert(Algorithm::FindLast(kArrayD.begin(), kArrayD.begin() + 3, 2) == &kArrayD[1]);
        static_assert(Algorithm::FindLast(kArrayD.begin(), kArrayD.end(), 5) == kArrayD.end());

        Vector<int> a{1, 2, 3, 4, 2, 1, 3};
        TEST_EXPECT_EQ(Algorithm::Find(a.begin(), a.end(), 2), &a[1]);
        TEST_EXPECT_EQ(Algorithm::Find(a.begin(), a.end(), 7), a.end());
        TEST_EXPECT_EQ(Algorithm::FindIf(a.begin(), a.end(), [](int x) { return x == 3; }), &a[2]);
        TEST_EXPECT_EQ(Algorithm::FindIfNot(a.begin(), a.end(), [](int x) { return x == 1; }),
                       &a[1]);

        TEST_EXPECT_EQ(Algorithm::FindLast(a.begin(), a.end(), 3), &a[6]);
        TEST_EXPECT_EQ(Algorithm::FindLast(a.begin(), a.end(), 7), a.end());
        TEST_EXPECT_EQ(Algorithm::FindLastIf(a.begin(), a.end(), [](int x) { return x % 2 == 0; }),
                       &a[4]);
        TEST_EXPECT_EQ(Algorithm::FindLastIfNot(a.begin(), a.end(), [](int x) { return x == 3; }),
                       &a[5]);
    }

    TEST_COMMENT("ForEach");
    {
        Vector<int> a{1, 2, 3, 4, 5, 6, 7};
        Algorithm::ForEach(a.begin(), a.begin() + 4, [](int& x) { x += 1; });
        Vector<int> expected{2, 3, 4, 5, 5, 6, 7};
        TEST_EXPECT_EQ(a, expected);

        Algorithm::ForEach(a.begin(), a.end(), [](int& x) { x = 1; });
        expected = {1, 1, 1, 1, 1, 1, 1};
        TEST_EXPECT_TRUE(a == expected);
    }

    TEST_COMMENT("Replace");
    {
        Vector<int> a{1, 2, 3, 2, 2, 4, 5};
        Algorithm::Replace(a.begin(), a.end(), 2, 0);
        Vector<int> expected{1, 0, 3, 0, 0, 4, 5};
        TEST_EXPECT_EQ(a, expected);

        Algorithm::Replace(a.begin(), a.begin() + 3, 0, 1);
        expected = {1, 1, 3, 0, 0, 4, 5};
        TEST_EXPECT_EQ(a, expected);

        Algorithm::ReplaceIf(a.begin(), a.end(), [](int x) { return x == 1 || x == 0; }, 5);
        expected = {5, 5, 3, 5, 5, 4, 5};
        TEST_EXPECT_EQ(a, expected);

        Algorithm::ReplaceIf(a.begin(), a.begin() + 4, [](int x) { return x == 5; }, 0);
        expected = {0, 0, 3, 0, 5, 4, 5};
        TEST_EXPECT_EQ(a, expected);
    }

    TEST_COMMENT("AllOf, AnyOf, NoneOf");
    {
        auto IsOdd = [](int x) -> Bool { return x % 2 == 1; };
        auto IsEven = [](int x) -> Bool { return x % 2 == 0; };

        Vector<int> a{2, 4, 6, 8, 2, 4};
        Vector<int> b{2, 4, 6, 3, 2, 4};

        TEST_EXPECT_FALSE(Algorithm::AllOf(a.begin(), a.end(), IsOdd));
        TEST_EXPECT_TRUE(Algorithm::AllOf(a.begin(), a.end(), IsEven));

        TEST_EXPECT_TRUE(Algorithm::AnyOf(b.begin(), b.end(), IsOdd));
        TEST_EXPECT_TRUE(Algorithm::AnyOf(b.begin(), b.end(), IsEven));
        TEST_EXPECT_FALSE(Algorithm::AnyOf(b.begin(), b.end(), [](int x) { return x == 9999; }));

        TEST_EXPECT_TRUE(Algorithm::NoneOf(a.begin(), a.end(), IsOdd));
        TEST_EXPECT_FALSE(Algorithm::NoneOf(a.begin(), a.end(), IsEven));
    }
}