#pragma once

#include "../Containers/Details/ContiguousContainer.hpp"
#include "../Utility/NumericLimits.hpp"
#include "../Memory/Memory.hpp"
#include "../Assert.hpp"

#include "CharTraits.hpp"

#include <initializer_list>

OPX_NAMESPACE_BEGIN

template <typename T>
class String : public Details::ContiguousContainer<T> {
public:
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

    String() { AllocateData(0, kInitialCapacity); }

    template <SizeT N>
    String(const ElementType (*arr)[N]) {
        Init(arr, N);
    }

    String(std::initializer_list<ElementType> list) { Init(list.begin(), list.size()); }

    String(ConstPointerType str) { Init(str, CharTraitsType::Length(str)); }

    String(ConstPointerType str, SizeType n) { Init(str, n); }

    String(SizeType n, ElementType c) { Init(n, c); }

    String(const String& other) { Init(other.mData, other.mSize); }

    String(String&& other) noexcept {
        mData = other.mData;
        mSize = other.mSize;
        mCapacity = other.mCapacity;
        other.Init();
    }

    String(NullPtrT) = delete;

    ~String() {}

    void Release() {
        if (mData) {
            Memory::Free(mData);
            mData = nullptr;
            mSize = 0;
            mCapacity = 0;
        }
    }

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

private:
    void AllocateData(SizeType size, SizeType capacity) {
        mSize = size;
        mCapacity = capacity;
        mData = static_cast<PointerType>(Memory::Allocate((mCapacity + 1) * sizeof(ElementType)));
        mData[size] = kNullChar;
    }

    void Init(ConstPointerType str, SizeType n) {
        OPX_ASSERT(n >= 0);
        AllocateData(n, OPX_MAX(n, kInitialCapacity));
        CharTraitsType::Copy(mData, str, n);
    }

    void Init(SizeType n, ElementType c) {
        OPX_ASSERT(n >= 0);
        AllocateData(n, OPX_MAX(n, kInitialCapacity));
        CharTraitsType::Fill(mData, c, n);
    }
};

OPX_NAMESPACE_END