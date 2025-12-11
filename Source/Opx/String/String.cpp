#include "OpxPCH.hpp"

OPX_NAMESPACE_BEGIN

template <typename T>
BasicString<T>::BasicString() {
    AllocateData(0, kInitialCapacity);
}

template <typename T>
BasicString<T>::BasicString(std::initializer_list<ElementType> list) {
    Init(list.begin(), static_cast<SizeType>(list.size()));
}

template <typename T>
BasicString<T>::BasicString(ConstPointerType str) {
    Init(str, static_cast<SizeType>(CharTraitsType::Length(str)));
}

template <typename T>
BasicString<T>::BasicString(ConstPointerType str, SizeType n) {
    Init(str, n);
}

template <typename T>
BasicString<T>::BasicString(SizeType n, ElementType c) {
    Init(n, c);
}

template <typename T>
BasicString<T>::BasicString(const BasicString& other) {
    Init(other.mData, other.mSize);
}

template <typename T>
BasicString<T>::BasicString(BasicString&& other) noexcept {
    mData = other.mData;
    mSize = other.mSize;
    mCapacity = other.mCapacity;
    other.AllocateData(0, kInitialCapacity);
}

template <typename T>
BasicString<T>::~BasicString() {
    Release();
}

template <typename T>
void BasicString<T>::Release() {
    if (mData) {
        Memory::Free(mData);
        mData = nullptr;
        mSize = 0;
        mCapacity = 0;
    }
}

template <typename T>
void BasicString<T>::AllocateData(SizeType size, SizeType capacity) {
    OPX_ASSERT(size >= 0 && (capacity + 1) <= kMaxCapacity);
    mSize = size;
    mCapacity = capacity;
    mData = static_cast<PointerType>(
        Memory::Allocate(static_cast<SizeT>(mCapacity + 1) * sizeof(ElementType)));
    mData[size] = kNullChar;
}

template <typename T>
void BasicString<T>::Init(ConstPointerType str, SizeType n) {
    OPX_ASSERT(n >= 0);
    AllocateData(n, OPX_MAX(n, kInitialCapacity));
    CharTraitsType::Copy(mData, str, static_cast<SizeT>(n));
}

template <typename T>
void BasicString<T>::Init(SizeType n, ElementType c) {
    OPX_ASSERT(n >= 0);
    AllocateData(n, OPX_MAX(n, kInitialCapacity));
    CharTraitsType::Fill(mData, c, static_cast<SizeT>(n));
}

template <typename T>
BasicString<T>& BasicString<T>::Assign(SizeType len, ConstPointerType str) {
    OPX_ASSERT(len >= 0);

    if (len > mCapacity) {
        PointerType oldData = mData;
        AllocateData(len, len);
        CharTraitsType::Copy(mData, str, static_cast<SizeT>(len));
        Memory::Free(oldData);
    } else {
        mSize = len;
        CharTraitsType::Copy(mData, str, static_cast<SizeT>(len));
    }
    return *this;
}

template <typename T>
void BasicString<T>::Reserve(SizeType newCapacity) {
    if (newCapacity > mCapacity) {
        PointerType oldData = mData;
        AllocateData(mSize, newCapacity);
        CharTraitsType::Copy(mData, oldData, static_cast<SizeT>(mSize));
        Memory::Free(oldData);
    }
}

template <typename T>
void BasicString<T>::Shrink() {
    if (mCapacity != mSize) {
        PointerType oldData = mData;
        AllocateData(mSize, mSize);
        CharTraitsType::Copy(mData, oldData, static_cast<SizeT>(mSize));
        Memory::Free(oldData);
    }
}

template <typename T>
void BasicString<T>::Resize(SizeType n, ElementType c) {
    OPX_ASSERT(n >= 0);

    if (n == mSize) {
        return;
    }

    if (n > mCapacity) {
        Reserve(n);
    }

    if (n > mSize) {
        CharTraitsType::Fill(mData + mSize, c, static_cast<SizeT>(n - mSize));
    }

    mData[n] = kNullChar;
    mSize = n;
}

template <typename T>
Int32 BasicString<T>::Compare(SizeType thisPos,
                              SizeType thisN,
                              ConstPointerType other,
                              SizeType otherPos,
                              SizeType otherN) const {
    OPX_ASSERT(thisN >= 0 && thisPos >= 0 && thisPos + thisN <= mSize);
    const auto result = CharTraitsType::Compare(mData + thisPos, other + otherPos,
                                                static_cast<SizeT>(OPX_MIN(thisN, otherN)));
    return result != 0 ? result : thisN > otherN ? 1 : thisN < otherN ? -1 : 0;
}

template <typename T>
Bool BasicString<T>::StartsWith(const BasicString& other, SizeType pos) const {
    return other.mSize + pos > mSize ? false
                                     : Compare(pos, other.mSize, other.mData, 0, other.mSize) == 0;
}

template <typename T>
Bool BasicString<T>::StartsWith(ConstPointerType str, SizeType pos) const {
    const auto len = static_cast<SizeType>(CharTraitsType::Length(str));
    return len + pos > mSize ? false : Compare(pos, len, str, 0, len) == 0;
}

template <typename T>
Bool BasicString<T>::EndsWith(const BasicString& other, SizeType pos) const {
    OPX_ASSERT((pos = pos <= 0 ? mSize - 1 : pos) < mSize);
    const auto newPos = pos - other.mSize + 1;
    return (newPos >= 0 && newPos <= mSize)
               ? Compare(newPos, other.mSize, other.mData, 0, other.mSize) == 0
               : false;
}

template <typename T>
Bool BasicString<T>::EndsWith(ConstPointerType str, SizeType pos) const {
    OPX_ASSERT((pos = pos <= 0 ? mSize - 1 : pos) < mSize);
    const auto len = static_cast<SizeType>(CharTraitsType::Length(str));
    const auto newPos = pos - len + 1;
    return (newPos >= 0 && newPos <= mSize) ? Compare(newPos, len, str, 0, len) == 0 : false;
}

template <typename T>
void BasicString<T>::TrimRight() {
    SizeType endPos = mSize;
    while (endPos > 0) {
        if (!CharTraitsType::IsWhitespace(mData[endPos - 1])) {
            break;
        }
        --endPos;
    }
    RemoveAt(endPos, mSize - endPos);
}

template <typename T>
void BasicString<T>::TrimLeft() {
    SizeType pos = 0;
    while (pos < mSize) {
        if (!CharTraitsType::IsWhitespace(mData[pos])) {
            break;
        }
        ++pos;
    }
    RemoveAt(0, pos);
}

template <typename T>
typename BasicString<T>::SizeType BasicString<T>::RemoveAt(SizeType index, SizeType n) {
    OPX_ASSERT(n >= 0 && index >= 0 && index + n <= mSize);

    if (index == 0 && n == mSize) {
        Clear();
        return index;
    }

    mSize -= n;
    PointerType dest = mData + index;
    CharTraitsType::Copy(dest, dest + n, static_cast<SizeT>(mSize - index));
    mData[mSize] = kNullChar;
    return index;
}

template <typename T>
typename BasicString<T>::SizeType BasicString<T>::GetNewCapacity(SizeType numCharsToAdd) {
    OPX_ASSERT_MSG(numCharsToAdd < kMaxCapacity - mCapacity, "The size of Vector<T> is too large");
    if (mCapacity < kMaxGrowthCapacity) {
        const SizeType newCapacityA = mCapacity + numCharsToAdd;
        const auto newCapacityB =
            static_cast<SizeType>(static_cast<Float>(mCapacity) * kGrowthFactor);
        return OPX_MAX(newCapacityA, newCapacityB);
    }
    return mCapacity + numCharsToAdd;
}

template <typename T>
typename BasicString<T>::ReferenceType BasicString<T>::Push(ElementType c) {
    if (mSize == mCapacity) {
        PointerType oldData = mData;
        AllocateData(mSize + 1, GetNewCapacity());
        CharTraitsType::Copy(mData, oldData, static_cast<SizeT>(mSize));
        mData[mSize - 1] = c;
        Memory::Free(oldData);
    } else {
        mData[mSize] = c;
        mData[++mSize] = kNullChar;
    }
    return mData[mSize - 1];
}

template <typename T>
template <typename U, typename Fn, Fn FUNC>
BasicString<T>& BasicString<T>::AppendImpl(SizeType n, U value) {
    OPX_ASSERT(n >= 0);
    if (mCapacity - mSize < n) {
        const auto oldSize = mSize;
        auto oldData = mData;
        AllocateData(mSize + n, GetNewCapacity(n));
        CharTraitsType::Copy(mData, oldData, static_cast<SizeT>(oldSize));
        FUNC(mData + oldSize, value, static_cast<SizeT>(n));
        Memory::Free(oldData);
    } else {
        FUNC(mData + mSize, value, static_cast<SizeT>(n));
        mSize += n;
        mData[mSize] = kNullChar;
    }
    return *this;
}

template <typename T>
template <typename U, typename Fn, Fn FUNC>
typename BasicString<T>::SizeType BasicString<T>::InsertImpl(SizeType index, SizeType n, U value) {
    OPX_ASSERT(n >= 0 && index >= 0 && index <= mSize);

    if (mCapacity - mSize < n) {
        const auto oldSize = mSize;
        auto oldData = mData;
        AllocateData(mSize + n, GetNewCapacity(n));
        CharTraitsType::Copy(mData, oldData, static_cast<SizeT>(index));
        FUNC(mData + index, value, static_cast<SizeT>(n));
        CharTraitsType::Copy(mData + index + n, oldData + index,
                             static_cast<SizeT>(oldSize - index));
        Memory::Free(oldData);
    } else {
        PointerType dest = mData + index;
        CharTraitsType::Move(dest + n, dest, static_cast<SizeT>(mSize - index));
        FUNC(dest, value, static_cast<SizeT>(n));
        mSize += n;
        mData[mSize] = kNullChar;
    }

    return index;
}

template <typename T>
BasicString<T>& BasicString<T>::Repeat(SizeType mult) {
    OPX_ASSERT(mult >= 0);

    if (mult == 0) {
        Clear();
        return *this;
    }

    if (mult == 1) {
        return *this;
    }

    const SizeType size = mSize;
    const SizeType count = mSize * mult;
    if (mCapacity < count) {
        PointerType oldData = mData;
        AllocateData(count, GetNewCapacity(count - mSize));
        SizeType currentPos = 0;
        while (mult--) {
            CharTraitsType::Copy(mData + currentPos, oldData, static_cast<SizeT>(size));
            currentPos += size;
        }
        Memory::Free(oldData);
    } else {
        SizeType currentPos = 0;
        while (mult--) {
            CharTraitsType::Copy(mData + currentPos, mData, static_cast<SizeT>(size));
            currentPos += size;
        }
        mSize = count;
        mData[mSize] = kNullChar;
    }
    return *this;
}

template class BasicString<Char>;
template class BasicString<WChar>;
#if defined(OPX_HAS_CHAR8_T)
template class BasicString<Char8>;
#endif
template class BasicString<Char16>;
template class BasicString<Char32>;

template class Vector<BasicString<Char>>;
template class Vector<BasicString<WChar>>;
#if defined(OPX_HAS_CHAR8_T)
template class Vector<BasicString<Char8>>;
#endif
template class Vector<BasicString<Char16>>;
template class Vector<BasicString<Char32>>;

OPX_NAMESPACE_END