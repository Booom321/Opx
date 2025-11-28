#pragma once

#include "../DataTypes.hpp"
#include "../Memory/Memory.hpp"
#include "../Algorithm/Equal.hpp"
#include "../Utility/NumericLimits.hpp"

OPX_NAMESPACE_BEGIN

template <typename T, Int64 NUM>
class Array {
public:
    using SizeType = Int64;

    using ElementType = T;
    using ReferenceType = T&;
    using PointerType = T*;
    using ConstReferenceType = const T&;
    using ConstPointerType = const T*;

    using IteratorType = T*;
    using ConstIteratorType = const T*;

    OPX_CONSTEXPR Array() = default;
    OPX_CONSTEXPR Array(const Array&) = default;
    OPX_CONSTEXPR Array(Array&&) noexcept = default;
    Array& operator=(const Array&) = default;
    Array& operator=(Array&&) noexcept = default;
    ~Array() = default;

    OPX_CONSTEXPR Bool IsEmpty() const noexcept { return NUM == 0; }
    OPX_CONSTEXPR SizeType GetSize() const noexcept { return NUM; }
    OPX_CONSTEXPR PointerType GetData() noexcept { return &mData[0]; }
    OPX_CONSTEXPR ConstPointerType GetData() const noexcept { return &mData[0]; }

    OPX_CONSTEXPR ReferenceType operator[](SizeType index) { return mData[index]; }
    OPX_CONSTEXPR ConstReferenceType operator[](SizeType index) const { return mData[index]; }
    OPX_CONSTEXPR ReferenceType At(SizeType index) { return mData[index]; }
    OPX_CONSTEXPR ConstReferenceType At(SizeType index) const { return mData[index]; }

    OPX_CONSTEXPR ReferenceType First() { return mData[0]; }
    OPX_CONSTEXPR ConstReferenceType First() const { return mData[0]; }
    OPX_CONSTEXPR ReferenceType Last() { return mData[NUM - 1]; }
    OPX_CONSTEXPR ConstReferenceType Last() const { return mData[NUM - 1]; }

    OPX_CONSTEXPR IteratorType begin() { return IteratorType(&mData[0]); }
    OPX_CONSTEXPR ConstIteratorType begin() const { return ConstIteratorType(&mData[0]); }
    OPX_CONSTEXPR IteratorType end() { return IteratorType(&mData[NUM]); }
    OPX_CONSTEXPR ConstIteratorType end() const { return ConstIteratorType(&mData[NUM]); }

    T mData[static_cast<SizeT>(NUM)];
};

template <typename T>
class Array<T, 0> {
public:
    using SizeType = Int64;

    using ElementType = T;
    using ReferenceType = T&;
    using PointerType = T*;
    using ConstReferenceType = const T&;
    using ConstPointerType = const T*;

    using IteratorType = T*;
    using ConstIteratorType = const T*;

    OPX_CONSTEXPR Array() = default;
    OPX_CONSTEXPR Array(const Array&) = default;
    OPX_CONSTEXPR Array(Array&&) noexcept = default;
    Array& operator=(const Array&) = default;
    Array& operator=(Array&&) noexcept = default;
    ~Array() = default;

    OPX_CONSTEXPR Bool IsEmpty() const noexcept { return true; }
    OPX_CONSTEXPR SizeType GetSize() const noexcept { return 0; }
    OPX_CONSTEXPR PointerType GetData() noexcept { return nullptr; }
    OPX_CONSTEXPR ConstPointerType GetData() const noexcept { return nullptr; }

    OPX_CONSTEXPR ReferenceType operator[](SizeType index) { return GetData()[index]; }
    OPX_CONSTEXPR ConstReferenceType operator[](SizeType index) const { return GetData()[index]; }
    OPX_CONSTEXPR ReferenceType At(SizeType index) { return GetData()[index]; }
    OPX_CONSTEXPR ConstReferenceType At(SizeType index) const { return GetData()[index]; }

    OPX_CONSTEXPR ReferenceType First() { return *GetData(); }
    OPX_CONSTEXPR ConstReferenceType First() const { return *GetData(); }
    OPX_CONSTEXPR ReferenceType Last() { return *GetData(); }
    OPX_CONSTEXPR ConstReferenceType Last() const { return *GetData(); }

    OPX_CONSTEXPR IteratorType begin() { return IteratorType(nullptr); }
    OPX_CONSTEXPR ConstIteratorType begin() const { return ConstIteratorType(nullptr); }
    OPX_CONSTEXPR IteratorType end() { return IteratorType(nullptr); }
    OPX_CONSTEXPR ConstIteratorType end() const { return ConstIteratorType(nullptr); }
};

template <typename T, Int64 NUM>
OPX_CONSTEXPR Bool operator==(const Array<T, NUM>& lhs, const Array<T, NUM>& rhs) {
    if constexpr (NUM == 0) {
        return true;
    } else {
        return Algorithm::Equal(lhs.mData, lhs.mData + NUM, rhs.mData);
    }
}

template <typename T, Int64 NUM>
OPX_CONSTEXPR Bool operator!=(const Array<T, NUM>& lhs, const Array<T, NUM>& rhs) {
    if constexpr (NUM == 0) {
        return false;
    } else {
        return !Algorithm::Equal(lhs.mData, lhs.mData + NUM, rhs.mData);
    }
}

namespace Internal {
    template <typename T, Bool LHS_USE_MOVE, Bool RHS_USE_MOVE, Int64 LHS_NUM, Int64 RHS_NUM>
    OPX_CONSTEXPR Array<T, LHS_NUM + RHS_NUM> ConcatenateArraysImpl(TypeChooser_T<LHS_USE_MOVE, T*, const T*> lhsData,
                                                                    TypeChooser_T<RHS_USE_MOVE, T*, const T*> rhsData) {
        static_assert(LHS_NUM <= NumericLimits<Int64>::kMax);
        static_assert(RHS_NUM <= NumericLimits<Int64>::kMax);
        static_assert(LHS_NUM <= NumericLimits<Int64>::kMax - RHS_NUM);

        Array<T, LHS_NUM + RHS_NUM> result{};
        T* destData = &result.mData[0];

        if constexpr (LHS_NUM > 0) {
            for (Int64 i = 0; i < LHS_NUM; ++i, ++destData) {
                if constexpr (LHS_USE_MOVE) {
                    *destData = Move(lhsData[i]);
                } else {
                    *destData = lhsData[i];
                }
            }
        }

        if constexpr (RHS_NUM > 0) {
            for (Int64 i = 0; i < RHS_NUM; ++i, ++destData) {
                if constexpr (RHS_USE_MOVE) {
                    *destData = Move(rhsData[i]);
                } else {
                    *destData = rhsData[i];
                }
            }
        }

        return result;
    }
}  // namespace Internal

template <typename T, Int64 LHS_NUM, Int64 RHS_NUM>
OPX_CONSTEXPR Array<T, LHS_NUM + RHS_NUM> operator+(const Array<T, LHS_NUM>& lhs, const Array<T, RHS_NUM>& rhs) {
    return Internal::ConcatenateArraysImpl<T, false, false, LHS_NUM, RHS_NUM>(&lhs.mData[0], &rhs.mData[0]);
}

template <typename T, Int64 LHS_NUM, Int64 RHS_NUM>
OPX_CONSTEXPR Array<T, LHS_NUM + RHS_NUM> operator+(Array<T, LHS_NUM>&& lhs, Array<T, RHS_NUM>&& rhs) {
    return Internal::ConcatenateArraysImpl<T, true, true, LHS_NUM, RHS_NUM>(&lhs.mData[0], &rhs.mData[0]);
}

template <typename T, Int64 LHS_NUM, Int64 RHS_NUM>
OPX_CONSTEXPR Array<T, LHS_NUM + RHS_NUM> operator+(const Array<T, LHS_NUM>& lhs, Array<T, RHS_NUM>&& rhs) {
    return Internal::ConcatenateArraysImpl<T, false, true, LHS_NUM, RHS_NUM>(&lhs.mData[0], &rhs.mData[0]);
}

template <typename T, Int64 LHS_NUM, Int64 RHS_NUM>
OPX_CONSTEXPR Array<T, LHS_NUM + RHS_NUM> operator+(Array<T, LHS_NUM>&& lhs, const Array<T, RHS_NUM>& rhs) {
    return Internal::ConcatenateArraysImpl<T, true, false, LHS_NUM, RHS_NUM>(&lhs.mData[0], &rhs.mData[0]);
}

OPX_NAMESPACE_END