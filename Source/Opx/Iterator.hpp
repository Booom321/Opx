#pragma once

#include "DataTypes.hpp"
#include "TypeTraits.hpp"
#include <iterator>

OPX_NAMESPACE_BEGIN

struct InputIteratorTag {};
struct OutputIteratorTag {};
struct ForwardIteratorTag : InputIteratorTag {};
struct BidirectionalIteratorTag : ForwardIteratorTag {};
struct RandomAccessIteratorTag : BidirectionalIteratorTag {};

namespace Details {
    template <typename Tag, typename Offset, typename Value, typename Pointer, typename Reference>
    struct IteratorTraitsImpl {
        using IteratorTag = Tag;
        using OffsetType = Offset;
        using ValueType = Value;
        using PointerType = Pointer;
        using ReferenceType = Reference;
    };

    template <typename StdIteratorTag>
    struct MapStdIteratorTag {
        using Type = InputIteratorTag;
    };
    template <>
    struct MapStdIteratorTag<std::input_iterator_tag> {
        using Type = InputIteratorTag;
    };
    template <>
    struct MapStdIteratorTag<std::forward_iterator_tag> {
        using Type = ForwardIteratorTag;
    };
    template <>
    struct MapStdIteratorTag<std::bidirectional_iterator_tag> {
        using Type = BidirectionalIteratorTag;
    };
    template <>
    struct MapStdIteratorTag<std::random_access_iterator_tag> {
        using Type = RandomAccessIteratorTag;
    };
}  // namespace Details

template <typename Iterator, typename = void>
struct IteratorTraits : Details::IteratorTraitsImpl<typename Iterator::IteratorTag,
                                                    typename Iterator::OffsetType,
                                                    typename Iterator::ValueType,
                                                    typename Iterator::PointerType,
                                                    typename Iterator::ReferenceType> {};

template <typename Iterator>
struct IteratorTraits<Iterator,
                      Void_T<typename Iterator::iterator_category,
                             typename Iterator::difference_type,
                             typename Iterator::value_type,
                             typename Iterator::pointer,
                             typename Iterator::reference>>
    : Details::IteratorTraitsImpl<
          typename Details::MapStdIteratorTag<typename Iterator::iterator_category>::Type,
          typename Iterator::difference_type,
          typename Iterator::value_type,
          typename Iterator::pointer,
          typename Iterator::reference> {};

template <typename T>
struct IteratorTraits<T*>
    : Details::IteratorTraitsImpl<RandomAccessIteratorTag, PtrOffsetT, T, T*, T&> {};

template <typename T>
struct IteratorTraits<const T*>
    : Details::IteratorTraitsImpl<RandomAccessIteratorTag, PtrOffsetT, T, const T*, const T&> {};

template <typename, typename = void>
struct IteratorCategoryOf {
    using Type = void;
};
template <typename T>
struct IteratorCategoryOf<T, Void_T<typename IteratorTraits<T>::IteratorTag>> {
    using Type = typename IteratorTraits<T>::IteratorTag;
};
template <typename T>
using IteratorCategoryOf_T = typename IteratorCategoryOf<T>::Type;

template <typename T>
struct IsInputIterator : IsBaseOf<InputIteratorTag, IteratorCategoryOf_T<T>> {};
template <typename T>
struct IsOutputIterator : IsBaseOf<OutputIteratorTag, IteratorCategoryOf_T<T>> {};
template <typename T>
struct IsForwardIterator : IsBaseOf<ForwardIteratorTag, IteratorCategoryOf_T<T>> {};
template <typename T>
struct IsBidirectionalIterator : IsBaseOf<BidirectionalIteratorTag, IteratorCategoryOf_T<T>> {};
template <typename T>
struct IsRandomAccessIterator : IsBaseOf<RandomAccessIteratorTag, IteratorCategoryOf_T<T>> {};

template <typename T>
OPX_CONSTEXPR Bool IsInputIterator_V = IsInputIterator<T>::value;
template <typename T>
OPX_CONSTEXPR Bool IsOutputIterator_V = IsOutputIterator<T>::value;
template <typename T>
OPX_CONSTEXPR Bool IsForwardIterator_V = IsForwardIterator<T>::value;
template <typename T>
OPX_CONSTEXPR Bool IsBidirectionalIterator_V = IsBidirectionalIterator<T>::value;
template <typename T>
OPX_CONSTEXPR Bool IsRandomAccessIterator_V = IsRandomAccessIterator<T>::value;

OPX_NAMESPACE_END