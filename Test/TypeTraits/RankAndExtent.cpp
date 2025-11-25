#include "Framework/Framework.hpp"

#include <Opx/TypeTraits.hpp>

TEST_CASE(TypeTraits, Rank) {
    TEST_EXPECT_EQ(Opx::Rank_V<int>, 0);
    TEST_EXPECT_EQ(Opx::Rank_V<int[5]>, 1);
    TEST_EXPECT_EQ(Opx::Rank_V<int[5][5]>, 2);
    TEST_EXPECT_EQ(Opx::Rank_V<int[][5][5]>, 3);

    OPX_MAYBE_UNUSED int arr[][3] = {{1, 2, 3}};

    TEST_EXPECT_EQ(Opx::Rank_V<decltype(arr[0])>, 0);
    TEST_EXPECT_EQ(Opx::Rank_V<Opx::RemoveCVRef_T<decltype(arr[0])>>, 1);
}

TEST_CASE(TypeTraits, Extent) {
    TEST_EXPECT_TRUE(Opx::Extent_V<int[3]> == 3 && Opx::Extent_V<int[3], 0> == 3 && Opx::Extent_V<int[3][4], 0> == 3 &&
                     Opx::Extent_V<int[3][4], 1> == 4 && Opx::Extent_V<int[3][4], 2> == 0 && Opx::Extent_V<int[]> == 0);

    OPX_MAYBE_UNUSED int ary[][3] = {{1, 2, 3}};

    TEST_EXPECT_EQ(Opx::Extent_V<decltype(ary[0])>, 0);
    TEST_EXPECT_EQ(Opx::Extent_V<Opx::RemoveCVRef_T<decltype(ary[0])>>, 3);
}