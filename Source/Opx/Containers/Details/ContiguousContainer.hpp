#pragma once

#include "../../Assert.hpp"
#include "../../DataTypes.hpp"
#include "../../Memory/Memory.hpp"
#include "../../Utility/NumericLimits.hpp"

OPX_NAMESPACE_BEGIN

namespace Details {
    template <typename T>
    class ContiguousContainer {
    public:
        using SizeType = Int64;

        using ElementType = T;
        using ReferenceType = T&;
        using ConstReferenceType = const T&;
        using PointerType = T*;
        using ConstPointerType = const T*;

        using IteratorType = T*;
        using ConstIteratorType = const T*;

        static OPX_CONSTEXPR Float kGrowthFactor = 2.f;
        static OPX_CONSTEXPR SizeType kInvalidIndex = OPX_INVALID_INDEX;
        static OPX_CONSTEXPR SizeType kMaxCapacity =
            static_cast<SizeType>(NumericLimits<SizeType>::kMax / sizeof(ElementType));
        static OPX_CONSTEXPR SizeType kMaxGrowthCapacity =
            static_cast<SizeType>(static_cast<Float>(kMaxCapacity) / kGrowthFactor);

        ContiguousContainer() noexcept = default;

        OPX_INLINE PointerType GetData() noexcept { return mData; }
        OPX_INLINE ConstPointerType GetData() const noexcept { return mData; }
        OPX_INLINE SizeType GetSize() const noexcept { return mSize; }
        OPX_INLINE SizeType GetCapacity() const noexcept { return mCapacity; }
        OPX_INLINE Bool IsEmpty() const noexcept { return mSize == 0; }

        OPX_INLINE ReferenceType operator[](SizeType index) {
            OPX_ASSERT(index >= 0 && index < mSize);
            return mData[index];
        }

        OPX_INLINE ConstReferenceType operator[](SizeType index) const {
            OPX_ASSERT(index >= 0 && index < mSize);
            return mData[index];
        }

        OPX_INLINE ReferenceType At(SizeType index) {
            OPX_ASSERT(index >= 0 && index < mSize);
            return mData[index];
        }

        OPX_INLINE ConstReferenceType At(SizeType index) const {
            OPX_ASSERT(index >= 0 && index < mSize);
            return mData[index];
        }

        OPX_INLINE ReferenceType First() {
            OPX_ASSERT(mSize > 0);
            return mData[0];
        }

        OPX_INLINE ConstReferenceType First() const {
            OPX_ASSERT(mSize > 0);
            return mData[0];
        }

        OPX_INLINE ReferenceType Last() {
            OPX_ASSERT(mSize > 0);
            return mData[mSize - 1];
        }

        OPX_INLINE ConstReferenceType Last() const {
            OPX_ASSERT(mSize > 0);
            return mData[mSize - 1];
        }

        OPX_INLINE IteratorType begin() { return IteratorType(mData); }
        OPX_INLINE ConstIteratorType begin() const { return ConstIteratorType(mData); }
        OPX_INLINE IteratorType end() { return IteratorType(mData + mSize); }
        OPX_INLINE ConstIteratorType end() const { return ConstIteratorType(mData + mSize); }

        void Reverse() {
            for (SizeType i = 0, j = mSize - 1,
                          count = static_cast<SizeType>(static_cast<Float>(mSize) * .5f);
                 i < count; ++i, --j) {
                ElementType Temp = mData[i];
                mData[i] = mData[j];
                mData[j] = Temp;
            }
        }

    protected:
        PointerType mData;
        SizeType mSize;
        SizeType mCapacity;
    };
}  // namespace Details

OPX_NAMESPACE_END