#include "Framework/Framework.hpp"

#include <Opx/Iterator.hpp>

#include <vector>
#include <list>

struct MyIterator {
    using IteratorTag = Opx::RandomAccessIteratorTag;
    using OffsetType = Opx::PtrOffsetT;
    using ValueType = int;
    using PointerType = int*;
    using ReferenceType = int&;
};

template <typename Iterator>
OPX_CONSTEXPR auto BidirectionalIterator()
    -> Opx::EnableIf_T<Opx::IsBidirectionalIterator_V<Iterator>, int> {
    return 1;
}

template <typename Iterator>
OPX_CONSTEXPR auto RandomAccessIterator()
    -> Opx::EnableIf_T<Opx::IsRandomAccessIterator_V<Iterator>, int> {
    return 1;
}

static_assert(BidirectionalIterator<std::vector<int>::iterator>() == 1);
static_assert(BidirectionalIterator<std::list<int>::iterator>() == 1);

static_assert(BidirectionalIterator<int*>() == 1);
static_assert(RandomAccessIterator<int*>() == 1);

static_assert(RandomAccessIterator<std::vector<int>::iterator>() == 1);
// static_assert(RandomAccessIterator<std::list<int>::iterator>() == 1); // should be failed

static_assert(BidirectionalIterator<MyIterator>() == 1);
static_assert(RandomAccessIterator<MyIterator>() == 1);

TEST_CASE(Containers, Iterator) {
    TEST_EXPECT_TRUE(true);
}