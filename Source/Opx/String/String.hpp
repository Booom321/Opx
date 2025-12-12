#pragma once

#include "../Containers/Details/ContiguousContainer.hpp"
#include "../Utility/NumericLimits.hpp"
#include "../Memory/Memory.hpp"
#include "../Assert.hpp"
#include "../Containers/Array.hpp"
#include "../Containers/Vector.hpp"

#include "CharTraits.hpp"

#include <initializer_list>

OPX_NAMESPACE_BEGIN

template <typename T>
class BasicString : public Details::ContiguousContainer<T> {
public:
    static_assert(IsCharacter_V<T>, "T must be a character type.");

    using Base = Details::ContiguousContainer<T>;

    using SizeType = typename Base::SizeType;

    using ElementType = typename Base::ElementType;
    using ReferenceType = typename Base::ReferenceType;
    using PointerType = typename Base::PointerType;
    using ConstReferenceType = typename Base::ConstReferenceType;
    using ConstPointerType = typename Base::ConstPointerType;

    using IteratorType = typename Base::IteratorType;
    using ConstIteratorType = typename Base::ConstIteratorType;

    using Base::kGrowthFactor;
    using Base::kInvalidIndex;
    using Base::kMaxCapacity;
    using Base::kMaxGrowthCapacity;
    static OPX_CONSTEXPR SizeType kInitialCapacity = 32;
    static OPX_CONSTEXPR SizeType kNullChar = ElementType();

    using Base::mCapacity;
    using Base::mData;
    using Base::mSize;

    using CharTraitsType = CharTraits<ElementType>;

    BasicString();
    BasicString(std::initializer_list<ElementType> list);
    BasicString(ConstPointerType str);
    BasicString(ConstPointerType str, SizeType n);
    BasicString(SizeType n, ElementType c);
    BasicString(const BasicString& other);
    BasicString(BasicString&& other) noexcept;
    BasicString(NullPtrT) = delete;

    ~BasicString();

    void Release();

    using Base::GetCapacity;
    using Base::GetData;
    using Base::GetSize;
    using Base::IsEmpty;
    using Base::operator[];
    using Base::At;
    using Base::begin;
    using Base::end;
    using Base::First;
    using Base::Last;

    BasicString& Assign(SizeType len, ConstPointerType str);

    BasicString& operator=(ElementType c);
    BasicString& operator=(ConstPointerType str);
    BasicString& operator=(const BasicString& other);
    BasicString& operator=(BasicString&& other) noexcept;
    BasicString& operator=(NullPtrT) = delete;

    void Clear();
    void Reserve(SizeType newCapacity);
    void Shrink();
    void Resize(SizeType n, ElementType c = kNullChar);

    Bool Equals(const BasicString& other) const;
    Bool Equals(ConstPointerType str) const;

    Int32 Compare(const BasicString& other) const;
    Int32 Compare(ConstPointerType str) const;
    Int32 Compare(SizeType thisPos, SizeType thisN, const BasicString& other) const;
    Int32 Compare(SizeType thisPos, SizeType thisN, ConstPointerType other) const;
    Int32 Compare(SizeType thisPos,
                  SizeType thisN,
                  const BasicString& other,
                  SizeType otherPos,
                  SizeType otherN) const;
    Int32 Compare(SizeType thisPos,
                  SizeType thisN,
                  ConstPointerType other,
                  SizeType otherPos,
                  SizeType otherN) const;

    Bool StartsWith(const BasicString& other, SizeType pos = 0) const;
    Bool StartsWith(ConstPointerType str, SizeType pos = 0) const;
    Bool StartsWith(ElementType c, SizeType pos = 0) const;

    Bool EndsWith(const BasicString& other, SizeType pos = kInvalidIndex) const;
    Bool EndsWith(ConstPointerType str, SizeType pos = kInvalidIndex) const;
    Bool EndsWith(ElementType c, SizeType pos = kInvalidIndex) const;

    friend OPX_INLINE Bool operator==(const BasicString<T>& lhs, const BasicString<T>& rhs) {
        return lhs.Equals(rhs);
    }

    friend OPX_INLINE Bool operator!=(const BasicString<T>& lhs, const BasicString<T>& rhs) {
        return !lhs.Equals(rhs);
    }

    friend OPX_INLINE Bool operator==(const BasicString<T>& lhs,
                                      typename BasicString<T>::ConstPointerType rhs) {
        return lhs.Equals(rhs);
    }

    friend Bool operator!=(const BasicString<T>& lhs,
                           typename BasicString<T>::ConstPointerType rhs) {
        return !lhs.Equals(rhs);
    }

    friend OPX_INLINE Bool operator==(typename BasicString<T>::ConstPointerType lhs,
                                      const BasicString<T>& rhs) {
        return rhs.Equals(lhs);
    }

    friend OPX_INLINE Bool operator!=(typename BasicString<T>::ConstPointerType lhs,
                                      const BasicString<T>& rhs) {
        return !rhs.Equals(lhs);
    }

    ReferenceType Push(ElementType c);

    BasicString& Append(ConstPointerType str);
    BasicString& Append(ConstPointerType str, SizeType n);
    BasicString& Append(const BasicString& other);
    BasicString& Append(const BasicString& other, SizeType pos, SizeType n);
    BasicString& Append(SizeType n, ElementType c);
    template <typename... Chars>
    EnableIf_T<(IsConvertible_V<RemoveCV_T<Chars>, typename BasicString<T>::ElementType> && ...),
               BasicString<T>&>
    AppendChars(Chars... chars);

    BasicString& operator+=(const BasicString& other);
    BasicString& operator+=(ConstPointerType str);
    BasicString& operator+=(ElementType c);

    BasicString Slice(SizeType start);
    BasicString Slice(SizeType start, SizeType num);

    using Base::Reverse;
    void TrimRight();
    void TrimLeft();
    void Trim();

    friend OPX_INLINE BasicString operator+(const BasicString& lhs, const BasicString& rhs) {
        BasicString result{lhs};
        result.Append(rhs);
        return result;
    }

    friend OPX_INLINE BasicString operator+(const BasicString& lhs, ConstPointerType rhs) {
        BasicString result{lhs};
        result.Append(rhs);
        return result;
    }

    friend OPX_INLINE BasicString operator+(const BasicString& lhs, BasicString&& rhs) {
        rhs.Insert(0, lhs);
        return Move(rhs);
    }

    friend OPX_INLINE BasicString operator+(const BasicString& lhs, ElementType rhs) {
        BasicString result{lhs};
        result.Push(rhs);
        return result;
    }

    friend OPX_INLINE BasicString operator+(BasicString&& lhs, const BasicString& rhs) {
        BasicString result{Move(lhs)};
        result.Append(rhs);
        return result;
    }

    friend OPX_INLINE BasicString operator+(BasicString&& lhs, ConstPointerType rhs) {
        BasicString result{Move(lhs)};
        result.Append(rhs);
        return result;
    }

    friend OPX_INLINE BasicString operator+(BasicString&& lhs, BasicString&& rhs) {
        lhs.Append(rhs);
        return Move(lhs);
    }

    friend OPX_INLINE BasicString operator+(BasicString&& lhs, ElementType rhs) {
        BasicString result{Move(lhs)};
        result.Push(rhs);
        return result;
    }

    friend OPX_INLINE BasicString operator+(ConstPointerType lhs, const BasicString& rhs) {
        BasicString result{lhs};
        result.Append(rhs);
        return result;
    }

    friend OPX_INLINE BasicString operator+(ElementType lhs, const BasicString& rhs) {
        BasicString result(1, lhs);
        result.Append(rhs);
        return result;
    }

    friend OPX_INLINE BasicString operator+(ConstPointerType lhs, BasicString&& rhs) {
        rhs.Insert(0, lhs);
        return Move(rhs);
    }

    friend OPX_INLINE BasicString operator+(ElementType lhs, BasicString&& rhs) {
        rhs.Insert(0, 1, lhs);
        return Move(rhs);
    }

    SizeType Insert(SizeType index, ConstPointerType str);
    SizeType Insert(SizeType index, ConstPointerType str, SizeType n);
    SizeType Insert(SizeType index, const BasicString& other);
    SizeType Insert(SizeType index, const BasicString& other, SizeType pos, SizeType n);
    SizeType Insert(SizeType index, SizeType n, ElementType c);
    template <typename... Chars>
    EnableIf_T<(IsConvertible_V<RemoveCV_T<Chars>, typename BasicString<T>::ElementType> && ...),
               SizeType>
    InsertChars(SizeType index, Chars... chars);

    SizeType RemoveAt(SizeType index, SizeType n = 1);
    ElementType Pop();

    BasicString& Repeat(SizeType mult);
    BasicString& operator*=(SizeType n);

    friend OPX_INLINE BasicString operator*(const BasicString& str, SizeType n) {
        BasicString result{str};
        result.Repeat(n);
        return result;
    }

    friend OPX_INLINE BasicString operator*(SizeType n, const BasicString& str) {
        BasicString result{str};
        result.Repeat(n);
        return result;
    }

    friend OPX_INLINE BasicString operator*(BasicString&& str, SizeType n) {
        str.Repeat(n);
        return Move(str);
    }

    friend OPX_INLINE BasicString operator*(SizeType n, BasicString&& str) {
        str.Repeat(n);
        return Move(str);
    }

    SizeType Find(ElementType c, SizeType start = 0) const;
    SizeType Find(ConstPointerType str, SizeType start = 0) const;
    SizeType Find(const BasicString& str, SizeType start = 0) const;
    SizeType Find(ConstPointerType str, SizeType pos, SizeType n, SizeType start = 0) const;

    SizeType FindLast(ElementType c, SizeType start = kInvalidIndex) const;
    SizeType FindLast(ConstPointerType str, SizeType start = kInvalidIndex) const;
    SizeType FindLast(const BasicString& str, SizeType start = kInvalidIndex) const;
    SizeType FindLast(ConstPointerType str,
                      SizeType pos,
                      SizeType n,
                      SizeType start = kInvalidIndex) const;

    SizeType FindFirstOf(ElementType c, SizeType start = 0) const;
    SizeType FindFirstOf(ConstPointerType str, SizeType start = 0) const;
    SizeType FindFirstOf(const BasicString& str, SizeType start = 0) const;
    SizeType FindFirstOf(ConstPointerType str, SizeType pos, SizeType n, SizeType start = 0) const;

    SizeType FindFirstNotOf(ElementType c, SizeType start = 0) const;
    SizeType FindFirstNotOf(ConstPointerType str, SizeType start = 0) const;
    SizeType FindFirstNotOf(const BasicString& str, SizeType start = 0) const;
    SizeType FindFirstNotOf(ConstPointerType str,
                            SizeType pos,
                            SizeType n,
                            SizeType start = 0) const;

    SizeType FindLastOf(ElementType c, SizeType start = kInvalidIndex) const;
    SizeType FindLastOf(ConstPointerType str, SizeType start = kInvalidIndex) const;
    SizeType FindLastOf(const BasicString& str, SizeType start = kInvalidIndex) const;
    SizeType FindLastOf(ConstPointerType str,
                        SizeType pos,
                        SizeType n,
                        SizeType start = kInvalidIndex) const;

    SizeType FindLastNotOf(ElementType c, SizeType start = kInvalidIndex) const;
    SizeType FindLastNotOf(ConstPointerType str, SizeType start = kInvalidIndex) const;
    SizeType FindLastNotOf(const BasicString& str, SizeType start = kInvalidIndex) const;
    SizeType FindLastNotOf(ConstPointerType str,
                           SizeType pos,
                           SizeType n,
                           SizeType start = kInvalidIndex) const;

    SizeType Count(ElementType c, SizeType start = 0) const;
    SizeType Count(ConstPointerType str, SizeType start = 0) const;
    SizeType Count(const BasicString& str, SizeType start = 0) const;
    SizeType Count(ConstPointerType str, SizeType strLen, SizeType start = 0) const;

    /*void Replace(SizeType thisPos, SizeType thisN, const BasicString& str, SizeType start = 0);
    void Replace(SizeType thisPos, SizeType thisN, ConstPointerType str, SizeType start = 0);
    void Replace(SizeType thisPos,
                 SizeType thisN,
                 ConstPointerType str,
                 SizeType strLen,
                 SizeType start = 0);

    void Replace(ConstPointerType oldStr, ConstPointerType newStr, SizeType start = 0);
    void Replace(const BasicString& oldStr, const BasicString& newStr, SizeType start = 0);
    void Replace(ConstPointerType oldStr, const BasicString& newStr, SizeType start = 0);
    void Replace(const BasicString& oldStr, ConstPointerType newStr, SizeType start = 0);

    void ReplaceN(ConstPointerType oldStr,
                  ConstPointerType newStr,
                  SizeType limit,
                  SizeType start = 0);
    void ReplaceN(const BasicString& oldStr,
                  const BasicString& newStr,
                  SizeType limit,
                  SizeType start = 0);
    void ReplaceN(ConstPointerType oldStr,
                  const BasicString& newStr,
                  SizeType limit,
                  SizeType start = 0);
    void ReplaceN(const BasicString& oldStr,
                  ConstPointerType newStr,
                  SizeType limit,
                  SizeType start = 0);*/

private:
    void AllocateData(SizeType size, SizeType capacity);
    void Init(ConstPointerType str, SizeType n);
    void Init(SizeType n, ElementType c);
    SizeType GetNewCapacity(SizeType numCharsToAdd = 1);

    template <typename U, typename Fn, Fn FUNC>
    BasicString& AppendImpl(SizeType n, U value);
    template <typename U, typename Fn, Fn FUNC>
    SizeType InsertImpl(SizeType index, SizeType n, U value);
};

template <typename T>
OPX_INLINE BasicString<T>& BasicString<T>::operator=(ElementType c) {
    if (mCapacity < 1) {
        Reserve(1);
    }
    mSize = 1;
    mData[0] = c, mData[1] = kNullChar;
    return *this;
}

template <typename T>
OPX_INLINE BasicString<T>& BasicString<T>::operator=(ConstPointerType str) {
    return str != mData ? Assign(static_cast<SizeType>(CharTraitsType::Length(str)), str) : *this;
}

template <typename T>
OPX_INLINE BasicString<T>& BasicString<T>::operator=(const BasicString& other) {
    return this != &other ? Assign(other.mSize, other.mData) : *this;
}

template <typename T>
BasicString<T>& BasicString<T>::operator=(BasicString&& other) noexcept {
    if (this != &other) {
        PointerType tempData = mData;
        SizeType tempCapacity = mCapacity;

        mData = other.mData;
        mSize = other.mSize;
        mCapacity = other.mCapacity;

        other.mData = tempData;
        other.mSize = 0;
        other.mCapacity = tempCapacity;
        *other.mData = kNullChar;
    }
    return *this;
}

template <typename T>
OPX_INLINE void BasicString<T>::Clear() {
    if (mSize > 0) {
        mSize = 0;
        *mData = kNullChar;
    }
}

template <typename T>
OPX_INLINE Bool BasicString<T>::Equals(const BasicString& other) const {
    return mSize == other.mSize &&
           CharTraitsType::Compare(mData, other.mData, static_cast<SizeT>(mSize)) == 0;
}

template <typename T>
OPX_INLINE Bool BasicString<T>::Equals(ConstPointerType str) const {
    const auto len = static_cast<SizeType>(CharTraitsType::Length(str));
    return (mSize == len) && (CharTraitsType::Compare(mData, str, static_cast<SizeT>(len)) == 0);
}

template <typename T>
OPX_INLINE Int32 BasicString<T>::Compare(const BasicString& other) const {
    return Compare(0, mSize, other.mData, 0, other.mSize);
}

template <typename T>
OPX_INLINE Int32 BasicString<T>::Compare(ConstPointerType str) const {
    const auto len = static_cast<SizeType>(CharTraitsType::Length(str));
    return Compare(0, mSize, str, 0, len);
}

template <typename T>
OPX_INLINE Int32 BasicString<T>::Compare(SizeType thisPos,
                                         SizeType thisN,
                                         const BasicString& other) const {
    return Compare(thisPos, thisN, other.mData, 0, other.mSize);
}

template <typename T>
OPX_INLINE Int32 BasicString<T>::Compare(SizeType thisPos,
                                         SizeType thisN,
                                         ConstPointerType str) const {
    const auto len = static_cast<SizeType>(CharTraitsType::Length(str));
    return Compare(thisPos, thisN, str, 0, len);
}

template <typename T>
OPX_INLINE Int32 BasicString<T>::Compare(SizeType thisPos,
                                         SizeType thisN,
                                         const BasicString& other,
                                         SizeType otherPos,
                                         SizeType otherN) const {
    return Compare(thisPos, thisN, other.mData, otherPos, otherN);
}

template <typename T>
OPX_INLINE Bool BasicString<T>::StartsWith(ElementType c, SizeType pos) const {
    OPX_ASSERT(pos >= 0 && pos < mSize);
    return mData[pos] == c;
}

template <typename T>
OPX_INLINE Bool BasicString<T>::EndsWith(ElementType c, SizeType pos) const {
    OPX_ASSERT((pos = pos <= 0 ? mSize - 1 : pos) < mSize);
    return mData[pos] == c;
}

template <typename T>
OPX_INLINE BasicString<T> BasicString<T>::Slice(SizeType start) {
    OPX_ASSERT(start >= 0 && start < mSize);
    return BasicString<T>(mData + start, mSize - start);
}

template <typename T>
OPX_INLINE BasicString<T> BasicString<T>::Slice(SizeType start, SizeType num) {
    OPX_ASSERT(num >= 0 && start >= 0 && start + num <= mSize);
    return BasicString(mData + start, num);
}

template <typename T>
OPX_INLINE void BasicString<T>::Trim() {
    TrimLeft();
    TrimRight();
}

template <typename T>
OPX_INLINE BasicString<T>& BasicString<T>::Append(ConstPointerType str, SizeType n) {
    return AppendImpl<ConstPointerType, decltype(CharTraitsType::Copy), CharTraitsType::Copy>(n,
                                                                                              str);
}

template <typename T>
OPX_INLINE BasicString<T>& BasicString<T>::Append(ConstPointerType str) {
    return AppendImpl<ConstPointerType, decltype(CharTraitsType::Copy), CharTraitsType::Copy>(
        static_cast<SizeType>(CharTraitsType::Length(str)), str);
}

template <typename T>
OPX_INLINE BasicString<T>& BasicString<T>::Append(const BasicString& other) {
    return AppendImpl<ConstPointerType, decltype(CharTraitsType::Copy), CharTraitsType::Copy>(
        other.mSize, other.mData);
}

template <typename T>
OPX_INLINE BasicString<T>& BasicString<T>::Append(const BasicString& other,
                                                  SizeType pos,
                                                  SizeType n) {
    OPX_ASSERT(pos >= 0 && pos + n <= other.mSize);
    return AppendImpl<ConstPointerType, decltype(CharTraitsType::Copy), CharTraitsType::Copy>(
        n, other.mData + pos);
}

template <typename T>
OPX_INLINE BasicString<T>& BasicString<T>::Append(SizeType n, ElementType c) {
    return AppendImpl<ElementType, decltype(CharTraitsType::Fill), CharTraitsType::Fill>(n, c);
}

template <typename T>
template <typename... Chars>
EnableIf_T<(IsConvertible_V<RemoveCV_T<Chars>, typename BasicString<T>::ElementType> && ...),
           BasicString<T>&>
BasicString<T>::AppendChars(Chars... chars) {
    std::initializer_list<ElementType> list{static_cast<ElementType>(chars)...};
    return AppendImpl<ConstPointerType, decltype(CharTraitsType::Copy), CharTraitsType::Copy>(
        static_cast<SizeType>(list.size()), list.begin());
}

template <typename T>
OPX_INLINE BasicString<T>& BasicString<T>::operator+=(const BasicString& other) {
    return AppendImpl<ConstPointerType, decltype(CharTraitsType::Copy), CharTraitsType::Copy>(
        other.mSize, other.mData);
}

template <typename T>
OPX_INLINE BasicString<T>& BasicString<T>::operator+=(ConstPointerType str) {
    return AppendImpl<ConstPointerType, decltype(CharTraitsType::Copy), CharTraitsType::Copy>(
        static_cast<SizeType>(CharTraitsType::Length(str)), str);
}

template <typename T>
OPX_INLINE BasicString<T>& BasicString<T>::operator+=(ElementType c) {
    Push(c);
    return *this;
}

template <typename T>
OPX_INLINE typename BasicString<T>::SizeType BasicString<T>::Insert(SizeType index,
                                                                    ConstPointerType str) {
    return InsertImpl<ConstPointerType, decltype(CharTraitsType::Copy), CharTraitsType::Copy>(
        index, static_cast<SizeType>(CharTraitsType::Length(str)), str);
}

template <typename T>
OPX_INLINE typename BasicString<T>::SizeType BasicString<T>::Insert(SizeType index,
                                                                    ConstPointerType str,
                                                                    SizeType n) {
    return InsertImpl<ConstPointerType, decltype(CharTraitsType::Copy), CharTraitsType::Copy>(
        index, n, str);
}

template <typename T>
OPX_INLINE typename BasicString<T>::SizeType BasicString<T>::Insert(SizeType index,
                                                                    const BasicString& other) {
    return InsertImpl<ConstPointerType, decltype(CharTraitsType::Copy), CharTraitsType::Copy>(
        index, other.mSize, other.mData);
}

template <typename T>
OPX_INLINE typename BasicString<T>::SizeType BasicString<T>::Insert(SizeType index,
                                                                    const BasicString& other,
                                                                    SizeType pos,
                                                                    SizeType n) {
    OPX_ASSERT(pos >= 0 && pos + n <= other.mSize);
    return InsertImpl<ConstPointerType, decltype(CharTraitsType::Copy), CharTraitsType::Copy>(
        index, n, other.mData + pos);
}

template <typename T>
OPX_INLINE typename BasicString<T>::SizeType BasicString<T>::Insert(SizeType index,
                                                                    SizeType n,
                                                                    ElementType c) {
    return InsertImpl<ElementType, decltype(CharTraitsType::Fill), CharTraitsType::Fill>(index, n,
                                                                                         c);
}
template <typename T>
template <typename... Chars>
EnableIf_T<(IsConvertible_V<RemoveCV_T<Chars>, typename BasicString<T>::ElementType> && ...),
           typename BasicString<T>::SizeType>
BasicString<T>::InsertChars(SizeType index, Chars... chars) {
    std::initializer_list<ElementType> list{static_cast<ElementType>(chars)...};
    return InsertImpl<ConstPointerType, decltype(CharTraitsType::Copy), CharTraitsType::Copy>(
        index, static_cast<SizeType>(list.size()), list.begin());
}

template <typename T>
OPX_INLINE typename BasicString<T>::ElementType BasicString<T>::Pop() {
    ElementType c = Last();
    Last() = kNullChar;
    --mSize;
    return c;
}

template <typename T>
OPX_INLINE BasicString<T>& BasicString<T>::operator*=(SizeType n) {
    return Repeat(n);
}

template <typename T>
typename BasicString<T>::SizeType BasicString<T>::Find(ElementType c, SizeType start) const {
    if (mSize == 0 || start >= mSize) {
        return kInvalidIndex;
    }
    start = (start < 0) ? 0 : start;
    const auto result =
        CharTraitsType::FindChar(mData + start, c, static_cast<SizeT>(mSize - start));
    return result ? static_cast<SizeType>(result - mData) : kInvalidIndex;
}

template <typename T>
OPX_INLINE typename BasicString<T>::SizeType BasicString<T>::Find(ConstPointerType str,
                                                                  SizeType start) const {
    return Find(str, 0, static_cast<SizeType>(CharTraitsType::Length(str)), start);
}

template <typename T>
OPX_INLINE typename BasicString<T>::SizeType BasicString<T>::Find(const BasicString& str,
                                                                  SizeType start) const {
    return Find(str.mData, 0, str.mSize, start);
}

template <typename T>
typename BasicString<T>::SizeType BasicString<T>::Find(ConstPointerType str,
                                                       SizeType pos,
                                                       SizeType n,
                                                       SizeType start) const {
    OPX_ASSERT(pos >= 0 && n >= 0);
    if (n > mSize - start) {
        return kInvalidIndex;
    }
    start = (start < 0) ? 0 : start;
    const auto result = CharTraitsType::FindSubstr(mData + start, static_cast<SizeT>(mSize - start),
                                                   str + pos, static_cast<SizeT>(n));
    return result ? static_cast<SizeType>(result - mData) : kInvalidIndex;
}

template <typename T>
typename BasicString<T>::SizeType BasicString<T>::FindLast(ElementType c, SizeType start) const {
    start = (start >= mSize || start == kInvalidIndex) ? mSize - 1 : start;
    if (mSize == 0 || start < -1) {
        return kInvalidIndex;
    }
    const auto result = CharTraitsType::FindLastChar(mData, c, static_cast<SizeT>(start));
    return result ? static_cast<SizeType>(result - mData) : kInvalidIndex;
}

template <typename T>
OPX_INLINE typename BasicString<T>::SizeType BasicString<T>::FindLast(ConstPointerType str,
                                                                      SizeType start) const {
    return FindLast(str, 0, static_cast<SizeType>(CharTraitsType::Length(str)), start);
}

template <typename T>
OPX_INLINE typename BasicString<T>::SizeType BasicString<T>::FindLast(const BasicString& str,
                                                                      SizeType start) const {
    return FindLast(str.mData, 0, str.mSize, start);
}

template <typename T>
typename BasicString<T>::SizeType BasicString<T>::FindLast(ConstPointerType str,
                                                           SizeType pos,
                                                           SizeType n,
                                                           SizeType start) const {
    OPX_ASSERT(pos >= 0 && n >= 0);

    if (start >= mSize || start == kInvalidIndex) {
        start = mSize > n ? mSize - n : 0;
    } else if (start < 0) {
        return kInvalidIndex;
    }

    const auto result = CharTraitsType::FindLastSubstr(mData, static_cast<SizeT>(start), str + pos,
                                                       static_cast<SizeT>(n));
    return result ? static_cast<SizeType>(result - mData) : kInvalidIndex;
}

template <typename T>
typename BasicString<T>::SizeType BasicString<T>::FindFirstOf(ElementType c, SizeType start) const {
    return Find(c, start);
}

template <typename T>
typename BasicString<T>::SizeType BasicString<T>::FindFirstOf(ConstPointerType str,
                                                              SizeType start) const {
    return FindFirstOf(str, 0, static_cast<SizeType>(CharTraitsType::Length(str)), start);
}

template <typename T>
typename BasicString<T>::SizeType BasicString<T>::FindFirstOf(const BasicString& str,
                                                              SizeType start) const {
    return FindFirstOf(str.mData, 0, str.mSize, start);
}

template <typename T>
typename BasicString<T>::SizeType BasicString<T>::FindFirstOf(ConstPointerType str,
                                                              SizeType pos,
                                                              SizeType n,
                                                              SizeType start) const {
    OPX_ASSERT(pos >= 0 && n >= 0);
    const auto strPos = str + pos;
    for (auto i = (start < 0) ? 0 : start; i < mSize; ++i) {
        if (CharTraitsType::FindChar(strPos, mData[i], static_cast<SizeT>(n))) {
            return i;
        }
    }
    return kInvalidIndex;
}

template <typename T>
typename BasicString<T>::SizeType BasicString<T>::FindFirstNotOf(ElementType c,
                                                                 SizeType start) const {
    for (auto i = (start < 0) ? 0 : start; i < mSize; ++i) {
        if (mData[i] != c) {
            return i;
        }
    }
    return kInvalidIndex;
}

template <typename T>
typename BasicString<T>::SizeType BasicString<T>::FindFirstNotOf(ConstPointerType str,
                                                                 SizeType start) const {
    return FindFirstNotOf(str, 0, static_cast<SizeType>(CharTraitsType::Length(str)), start);
}

template <typename T>
typename BasicString<T>::SizeType BasicString<T>::FindFirstNotOf(const BasicString& str,
                                                                 SizeType start) const {
    return FindFirstNotOf(str.mData, 0, str.mSize, start);
}

template <typename T>
typename BasicString<T>::SizeType BasicString<T>::FindFirstNotOf(ConstPointerType str,
                                                                 SizeType pos,
                                                                 SizeType n,
                                                                 SizeType start) const {
    OPX_ASSERT(pos >= 0 && n >= 0);
    const auto strPos = str + pos;
    for (auto i = (start < 0) ? 0 : start; i < mSize; ++i) {
        if (CharTraitsType::FindChar(strPos, mData[i], static_cast<SizeT>(n)) == nullptr) {
            return i;
        }
    }
    return kInvalidIndex;
}

template <typename T>
typename BasicString<T>::SizeType BasicString<T>::FindLastOf(ElementType c, SizeType start) const {
    return FindLast(c, start);
}

template <typename T>
typename BasicString<T>::SizeType BasicString<T>::FindLastOf(ConstPointerType str,
                                                             SizeType start) const {
    return FindLastOf(str, 0, static_cast<SizeType>(CharTraitsType::Length(str)), start);
}

template <typename T>
typename BasicString<T>::SizeType BasicString<T>::FindLastOf(const BasicString& str,
                                                             SizeType start) const {
    return FindLastOf(str.mData, 0, str.mSize, start);
}

template <typename T>
typename BasicString<T>::SizeType BasicString<T>::FindLastOf(ConstPointerType str,
                                                             SizeType pos,
                                                             SizeType n,
                                                             SizeType start) const {
    OPX_ASSERT(pos >= 0 && n >= 0);
    const auto strPos = str + pos;
    for (auto i = (start >= mSize || start == kInvalidIndex) ? mSize - 1 : start; i >= 0; --i) {
        if (CharTraitsType::FindChar(strPos, mData[i], static_cast<SizeT>(n))) {
            return i;
        }
    }
    return kInvalidIndex;
}

template <typename T>
typename BasicString<T>::SizeType BasicString<T>::FindLastNotOf(ElementType c,
                                                                SizeType start) const {
    for (auto i = (start >= mSize || start == kInvalidIndex) ? mSize - 1 : start; i >= 0; --i) {
        if (mData[i] != c) {
            return i;
        }
    }
    return kInvalidIndex;
}

template <typename T>
typename BasicString<T>::SizeType BasicString<T>::FindLastNotOf(ConstPointerType str,
                                                                SizeType start) const {
    return FindLastNotOf(str, 0, static_cast<SizeType>(CharTraitsType::Length(str)), start);
}

template <typename T>
typename BasicString<T>::SizeType BasicString<T>::FindLastNotOf(const BasicString& str,
                                                                SizeType start) const {
    return FindLastNotOf(str.mData, 0, str.mSize, start);
}

template <typename T>
typename BasicString<T>::SizeType BasicString<T>::FindLastNotOf(ConstPointerType str,
                                                                SizeType pos,
                                                                SizeType n,
                                                                SizeType start) const {
    OPX_ASSERT(pos >= 0 && n >= 0);
    const auto strPos = str + pos;
    for (auto i = (start >= mSize || start == kInvalidIndex) ? mSize - 1 : start; i >= 0; --i) {
        if (CharTraitsType::FindChar(strPos, mData[i], static_cast<SizeT>(n)) == nullptr) {
            return i;
        }
    }
    return kInvalidIndex;
}

template <typename T>
typename BasicString<T>::SizeType BasicString<T>::Count(ElementType c, SizeType start) const {
    start = (start < 0) ? 0 : start;
    SizeType result = 0;
    for (auto i = start; i < mSize; ++i) {
        if (mData[i] == c) {
            ++result;
        }
    }
    return result;
}

template <typename T>
typename BasicString<T>::SizeType BasicString<T>::Count(ConstPointerType str,
                                                        SizeType start) const {
    return Count(str, static_cast<SizeType>(CharTraitsType::Length(str)), start);
}

template <typename T>
typename BasicString<T>::SizeType BasicString<T>::Count(const BasicString& str,
                                                        SizeType start) const {
    return Count(str.mData, str.mSize, start);
}

template <typename T>
typename BasicString<T>::SizeType BasicString<T>::Count(ConstPointerType str,
                                                        SizeType strLen,
                                                        SizeType start) const {
    if (strLen == 0) {
        return (mSize - start) + 1;
    }

    start = (start < 0) ? 0 : start;
    SizeType result = 0;
    for (auto i = start; i + strLen <= mSize;) {
        if (CharTraitsType::Compare(str, mData + i, static_cast<SizeT>(strLen)) == 0) {
            result += 1;
            i += strLen;
        } else {
            ++i;
        }
    }
    return result;
}

/*template <typename T>
void BasicString<T>::Replace(SizeType thisPos,
                             SizeType thisN,
                             const BasicString& str,
                             SizeType start) {}

template <typename T>
void BasicString<T>::Replace(SizeType thisPos,
                             SizeType thisN,
                             ConstPointerType str,
                             SizeType start) {}
template <typename T>
void BasicString<T>::Replace(SizeType thisPos,
                             SizeType thisN,
                             ConstPointerType str,
                             SizeType strLen,
                             SizeType start) {}

template <typename T>
void BasicString<T>::Replace(ConstPointerType oldStr, ConstPointerType newStr, SizeType start) {}
template <typename T>
void BasicString<T>::Replace(const BasicString& oldStr, const BasicString& newStr, SizeType start) {
}
template <typename T>
void BasicString<T>::Replace(ConstPointerType oldStr, const BasicString& newStr, SizeType start) {}
template <typename T>
void BasicString<T>::Replace(const BasicString& oldStr, ConstPointerType newStr, SizeType start) {}

template <typename T>
void BasicString<T>::ReplaceN(ConstPointerType oldStr,
                              ConstPointerType newStr,
                              SizeType limit,
                              SizeType start) {}
template <typename T>
void BasicString<T>::ReplaceN(const BasicString& oldStr,
                              const BasicString& newStr,
                              SizeType limit,
                              SizeType start) {}
template <typename T>
void BasicString<T>::ReplaceN(ConstPointerType oldStr,
                              const BasicString& newStr,
                              SizeType limit,
                              SizeType start) {}
template <typename T>
void BasicString<T>::ReplaceN(const BasicString& oldStr,
                              ConstPointerType newStr,
                              SizeType limit,
                              SizeType start) {}*/

using String = BasicString<Char>;
using WString = BasicString<WChar>;
#if defined(OPX_HAS_CHAR8_T)
using Utf8String = BasicString<char8_t>;
#endif
using Utf16String = BasicString<char16_t>;
using Utf32String = BasicString<char32_t>;

OPX_NAMESPACE_END