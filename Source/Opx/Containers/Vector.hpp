#pragma once

#include <initializer_list>

#include "../Assert.hpp"
#include "../Memory/Memory.hpp"
#include "../Algorithm/Equal.hpp"
#include "../Algorithm/ForEach.hpp"
#include "../Utility/NumericLimits.hpp"

OPX_NAMESPACE_BEGIN

namespace Internal {
    template <typename T, Bool USE_MOVE = IsMoveConstructible_V<T>>
    OPX_INLINE void MoveOrCopyConstructItem(T* dest, TypeChooser_T<USE_MOVE, T*, const T*> source) {
        if constexpr (USE_MOVE) {
            new (dest) T(Move(*source));
        } else {
            new (dest) T(*source);
        }
    }

    template <typename T, typename SizeType, Bool USE_MOVE = IsMoveConstructible_V<T>>
    OPX_INLINE void MoveOrCopyConstructItems(T* dest,
                                             TypeChooser_T<USE_MOVE, T*, const T*> source,
                                             SizeType num) {
        if constexpr (USE_MOVE) {
            Memory::MoveConstructItems<T, SizeType>(dest, source, num);
        } else {
            Memory::CopyConstructItems<T, SizeType>(dest, source, num);
        }
    }

    template <typename T, typename SizeType, Bool USE_MOVE = IsMoveAssignable_V<T>>
    OPX_INLINE void MoveOrCopyAssignItems(T* dest,
                                          TypeChooser_T<USE_MOVE, T*, const T*> source,
                                          SizeType num) {
        if constexpr (USE_MOVE) {
            Memory::MoveAssignItems<T, SizeType>(dest, source, num);
        } else {
            Memory::CopyAssignItems<T, SizeType>(dest, source, num);
        }
    }

    template <typename T, typename SizeType, Bool USE_MOVE = IsMoveAssignable_V<T>>
    OPX_INLINE void MoveOrCopyBackwardAssignItems(T* dest,
                                                  TypeChooser_T<USE_MOVE, T*, const T*> source,
                                                  SizeType num) {
        if constexpr (USE_MOVE) {
            Memory::MoveBackwardAssignItems<T, SizeType>(dest, source, num);
        } else {
            Memory::CopyBackwardAssignItems<T, SizeType>(dest, source, num);
        }
    }
}  // namespace Internal

template <typename T>
class Vector {
public:
    using SizeType = Int64;

    using ElementType = T;
    using ReferenceType = T&;
    using PointerType = T*;
    using ConstReferenceType = const T&;
    using ConstPointerType = const T*;

    using IteratorType = T*;
    using ConstIteratorType = const T*;

    static OPX_CONSTEXPR Float kGrowthFactor = 2.f;
    static OPX_CONSTEXPR SizeType kInvalidIndex = OPX_INVALID_INDEX;
    static OPX_CONSTEXPR SizeType kMaxCapacity =
        static_cast<SizeType>(NumericLimits<SizeType>::kMax / sizeof(ElementType));
    static OPX_CONSTEXPR SizeType kMaxGrowthCapacity =
        static_cast<SizeType>(static_cast<Float>(kMaxCapacity) / kGrowthFactor);

public:
    Vector() noexcept : mData(nullptr), mSize(0), mCapacity(0) {}

    explicit Vector(SizeType num) { Init<EInitMethod::DefaultConstruct>(num, num, nullptr); }

    Vector(SizeType num, ConstReferenceType value) {
        Init<EInitMethod::FillConstruct>(num, num, &value);
    }

    Vector(SizeType num, ConstPointerType pointer) {
        Init<EInitMethod::CopyConstruct>(num, num, pointer);
    }

    Vector(std::initializer_list<ElementType> list) {
        auto size = static_cast<SizeType>(list.size());
        Init<EInitMethod::CopyConstruct>(size, size, list.begin());
    }

    Vector(const Vector& other) {
        Init<EInitMethod::CopyConstruct>(other.mSize, other.mSize, other.mData);
    }

    Vector(Vector&& other) noexcept
        : mData(other.mData), mSize(other.mSize), mCapacity(other.mCapacity) {
        other.mData = nullptr;
        other.mSize = 0;
        other.mCapacity = 0;
    }

    ~Vector() { Release(); }

    void Release() {
        if (mData) {
            ReleaseInternal();
            mData = nullptr, mSize = 0, mCapacity = 0;
        }
    }

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

    void Assign(SizeType sourceSize, ConstPointerType source) {
        if (sourceSize > mCapacity) {
            Reallocate<false>(sourceSize, sourceSize, source);
        } else {
            Bool needsShrinking = mSize > sourceSize;
            if (needsShrinking) {
                Memory::DestructItems<ElementType, SizeType>(mData + sourceSize,
                                                             sourceSize - mSize);
            }
            SizeType count = needsShrinking ? sourceSize : mSize;
            Memory::CopyAssignItems<ElementType, SizeType>(mData, source, count);
            if (!needsShrinking) {
                Memory::CopyConstructItems<ElementType, SizeType>(mData + count, source + count,
                                                                  sourceSize - mSize);
            }
            mSize = sourceSize;
        }
    }

    OPX_INLINE Vector& operator=(std::initializer_list<ElementType> list) {
        Assign(static_cast<SizeType>(list.size()), list.begin());
        return *this;
    }

    OPX_INLINE Vector& operator=(const Vector& other) {
        if (this != &other) {
            Assign(other.mSize, other.mData);
        }
        return *this;
    }

    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            ReleaseInternal();
            mData = other.mData, mSize = other.mSize, mCapacity = other.mCapacity;
            other.mData = nullptr, other.mSize = 0, other.mCapacity = 0;
        }
        return *this;
    }

    OPX_INLINE void Clear() {
        Memory::DestructItems<ElementType, SizeType>(mData, mSize);
        mSize = 0;
    }

    OPX_INLINE void Reserve(SizeType newCapacity) {
        if (newCapacity > mCapacity) {
            Reallocate<true>(newCapacity, mSize, mData);
        }
    }

    OPX_INLINE void Shrink() {
        if (mCapacity != mSize) {
            Reallocate<true>(mSize, mSize, mData);
        }
    }

    OPX_INLINE void Resize(SizeType newSize) {
        ResizeImpl(newSize, [&]() {
            Memory::DefaultConstructItems<ElementType, SizeType>(mData + mSize, newSize - mSize);
        });
    }

    OPX_INLINE void Resize(SizeType newSize, ConstReferenceType value) {
        ResizeImpl(newSize, [&]() {
            Memory::FillConstructItems<ElementType, SizeType>(mData + mSize, value,
                                                              newSize - mSize);
        });
    }

    OPX_INLINE friend Bool operator==(const Vector<T>& lhs, const Vector<T>& rhs) {
        return lhs.mSize == rhs.mSize &&
               Algorithm::Equal(lhs.mData, lhs.mData + lhs.mSize, rhs.mData);
    }

    OPX_INLINE friend Bool operator!=(const Vector<T>& lhs, const Vector<T>& rhs) {
        return lhs.mSize != rhs.mSize ||
               !Algorithm::Equal(lhs.mData, lhs.mData + lhs.mSize, rhs.mData);
    }

    template <typename... ArgTypes>
    ReferenceType Push(ArgTypes&&... args) {
        if (mSize == mCapacity) {
            auto newData = BeginReallocate(GetNewCapacity());
            new (newData + mSize) ElementType(Forward<ArgTypes>(args)...);
            Internal::MoveOrCopyConstructItems<ElementType, SizeType>(newData, mData, mSize);
            EndReallocate(newData);
        } else {
            new (mData + mSize) ElementType(Forward<ArgTypes>(args)...);
        }
        mSize += 1;
        return mData[mSize - 1];
    }

    SizeType PushDefaulted(SizeType count = 1) {
        OPX_ASSERT(count >= 0);
        if (mCapacity - mSize <= count) {
            Reallocate<IsMoveConstructible_V<ElementType>>(GetNewCapacity(count), mSize, mData);
        }
        SizeType index = mSize;
        Memory::DefaultConstructItems<ElementType, SizeType>(mData + index, count);
        mSize += count;
        return index;
    }

    SizeType RemoveAt(SizeType index, SizeType count = 1) {
        OPX_ASSERT(index >= 0 && count >= 0 && index + count <= mSize);
        mSize -= count;
        Internal::MoveOrCopyAssignItems<ElementType, SizeType>(mData + index, mData + index + count,
                                                               mSize - index);
        Memory::DestructItems<ElementType, SizeType>(mData + mSize, count);
        return index;
    }

    ElementType Pop() {
        ElementType item(Move(mData[mSize - 1]));
        RemoveAt(mSize - 1);
        return item;
    }

    OPX_INLINE void Append(SizeType count, ConstReferenceType item) {
        AppendImpl<ConstReferenceType, decltype(Memory::FillConstructItems<ElementType, SizeType>),
                   Memory::FillConstructItems<ElementType, SizeType>>(count, item);
    }

    OPX_INLINE void Append(SizeType count, ConstPointerType items) {
        AppendImpl<ConstPointerType, decltype(Memory::CopyConstructItems<ElementType, SizeType>),
                   Memory::CopyConstructItems<ElementType, SizeType>>(count, items);
    }

    OPX_INLINE void Append(const Vector& other) {
        AppendImpl<ConstPointerType, decltype(Memory::CopyConstructItems<ElementType, SizeType>),
                   Memory::CopyConstructItems<ElementType, SizeType>>(other.mSize, other.mData);
    }
    OPX_INLINE void Append(std::initializer_list<ElementType> list) {
        AppendImpl<ConstPointerType, decltype(Memory::CopyConstructItems<ElementType, SizeType>),
                   Memory::CopyConstructItems<ElementType, SizeType>>(
            static_cast<SizeType>(list.size()), list.begin());
    }

    OPX_INLINE Vector& operator+=(const Vector& other) {
        AppendImpl<ConstPointerType, decltype(Memory::CopyConstructItems<ElementType, SizeType>),
                   Memory::CopyConstructItems<ElementType, SizeType>>(other.mSize, other.mData);
        return *this;
    }

    OPX_INLINE Vector& operator+=(std::initializer_list<ElementType> list) {
        AppendImpl<ConstPointerType, decltype(Memory::CopyConstructItems<ElementType, SizeType>),
                   Memory::CopyConstructItems<ElementType, SizeType>>(
            static_cast<SizeType>(list.size()), list.begin());
        return *this;
    }

    template <typename... ArgTypes>
    ReferenceType EmplaceAt(SizeType index, ArgTypes&&... args) {
        OPX_ASSERT(index >= 0 && index <= mSize);
        if (index == mSize) {
            return Push(Forward<ArgTypes>(args)...);
        }

        PointerType out = nullptr;
        if (mSize == mCapacity) {
            auto newData = BeginReallocate(GetNewCapacity());
            new (out = newData + index) ElementType(Forward<ArgTypes>(args)...);
            Internal::MoveOrCopyConstructItems<ElementType, SizeType>(newData, mData, index);
            Internal::MoveOrCopyConstructItems<ElementType, SizeType>(newData + index + 1,
                                                                      mData + index, mSize - index);
            EndReallocate(newData);
        } else {
            PointerType dest = mData + mSize;
            Internal::MoveOrCopyConstructItem<ElementType>(dest, dest - 1);
            auto count = mSize - index - 1;
            dest = mData + (index + count + 1);
            Internal::MoveOrCopyBackwardAssignItems<ElementType, SizeType>(dest, dest - 1, count);

            dest = mData + index;
            dest->~ElementType();
            new (out = dest) ElementType(Forward<ArgTypes>(args)...);
        }

        mSize += 1;
        return *out;
    }

    OPX_INLINE ReferenceType InsertDefaulted(SizeType index) { return EmplaceAt(index); }

    OPX_INLINE SizeType Insert(SizeType index, SizeType num, ConstPointerType items) {
        return InsertImpl<ConstPointerType>(index, num, items,
                                            Memory::CopyConstructItems<ElementType, SizeType>,
                                            Memory::CopyAssignItems<ElementType, SizeType>);
    }

    OPX_INLINE SizeType Insert(SizeType index, SizeType num, ConstReferenceType value) {
        return InsertImpl<ConstReferenceType>(index, num, value,
                                              Memory::FillConstructItems<ElementType, SizeType>,
                                              Memory::FillAssignItems<ElementType, SizeType>);
    }

    OPX_INLINE SizeType Insert(SizeType index, const Vector& vec) {
        return InsertImpl<ConstPointerType>(index, vec.mSize, vec.mData,
                                            Memory::CopyConstructItems<ElementType, SizeType>,
                                            Memory::CopyAssignItems<ElementType, SizeType>);
    }
    OPX_INLINE SizeType Insert(SizeType index, std::initializer_list<ElementType> list) {
        return InsertImpl<ConstPointerType>(index, static_cast<SizeType>(list.size()), list.begin(),
                                            Memory::CopyConstructItems<ElementType, SizeType>,
                                            Memory::CopyAssignItems<ElementType, SizeType>);
    }

    template <typename Predicate>
    void Filter(Vector& output, Predicate pred) {
        for (SizeType i = 0; i < mSize; ++i) {
            if (pred(mData[i])) {
                output.Push(mData[i]);
            }
        }
    }

    template <typename ModifierFunc>
    OPX_NODISCARD OPX_INLINE Vector Map(ModifierFunc fn) {
        Vector output(*this);
        Algorithm::ForEach(output.mData, output.mData + output.mSize, fn);
        return output;
    }

    template <typename ModifierFunc>
    OPX_INLINE void ForEach(ModifierFunc fn) {
        Algorithm::ForEach(mData, mData + mSize, fn);
    }

    OPX_INLINE Vector Slice(SizeType start, SizeType num = -1) {
        OPX_ASSERT(start >= 0 && start < mSize);
        return Vector((num < 0 || num >= mSize - start) ? mSize - start : num, mData + start);
    }

    void Reverse() {
        for (SizeType i = 0, j = mSize - 1,
                      count = static_cast<SizeType>(static_cast<Float>(mSize) * .5f);
             i < count; ++i, --j) {
            ElementType Temp = mData[i];
            mData[i] = mData[j];
            mData[j] = Temp;
        }
    }

private:
    explicit Vector(SizeType capacity, Bool) { Init<EInitMethod::None>(0, capacity, nullptr); }

    template <typename LhsVector, typename RhsVector>
    static Vector ConcatenateVectorsImpl(LhsVector lhs, RhsVector rhs) {
        OPX_ASSERT(lhs.mSize <= kMaxCapacity && rhs.mSize <= kMaxCapacity - lhs.mSize);
        Vector output(lhs.mSize + rhs.mSize, true);
        Internal::MoveOrCopyConstructItems<ElementType, SizeType, IsRvalueReference_V<LhsVector>>(
            output.mData, lhs.mData, lhs.mSize);
        Internal::MoveOrCopyConstructItems<ElementType, SizeType, IsRvalueReference_V<RhsVector>>(
            output.mData + lhs.mSize, rhs.mData, rhs.mSize);
        output.mSize = lhs.mSize + rhs.mSize;
        return output;
    }

public:
    OPX_NODISCARD OPX_INLINE friend Vector operator+(const Vector& lhs, const Vector& rhs) {
        return ConcatenateVectorsImpl<const Vector&, const Vector&>(lhs, rhs);
    }

    OPX_NODISCARD OPX_INLINE friend Vector operator+(Vector&& lhs, Vector&& rhs) {
        return ConcatenateVectorsImpl<Vector&&, Vector&&>(Move(lhs), Move(rhs));
    }

    OPX_NODISCARD OPX_INLINE friend Vector operator+(Vector&& lhs, const Vector& rhs) {
        return ConcatenateVectorsImpl<Vector&&, const Vector&>(Move(lhs), rhs);
    }

    OPX_NODISCARD OPX_INLINE friend Vector operator+(const Vector& lhs, Vector&& rhs) {
        return ConcatenateVectorsImpl<const Vector&, Vector&&>(lhs, Move(rhs));
    }

private:
    enum class EInitMethod : UInt8 {
        None,
        FillConstruct,
        CopyConstruct,
        DefaultConstruct,
    };

    template <EInitMethod METHOD>
    void Init(SizeType size, SizeType capacity, ConstPointerType source) {
        OPX_ASSERT(size >= 0 && capacity >= 0 && capacity <= kMaxCapacity);
        mSize = size;
        mCapacity = capacity;
        auto data = static_cast<PointerType>(
            Memory::Allocate(sizeof(ElementType) * static_cast<SizeT>(mCapacity)));

        if constexpr (METHOD == EInitMethod::FillConstruct) {
            Memory::FillConstructItems<ElementType, SizeType>(data, *source, mSize);
        } else if constexpr (METHOD == EInitMethod::CopyConstruct) {
            if (source) {
                Memory::CopyConstructItems<ElementType, SizeType>(data, source, mSize);
            }
        } else if constexpr (METHOD == EInitMethod::DefaultConstruct) {
            Memory::DefaultConstructItems<ElementType, SizeType>(data, mSize);
        } else {
        }  // EInitMethod::None

        mData = data;
    }

    template <Bool USE_MOVE>
    void Reallocate(SizeType newCapacity,
                    SizeType sourceSize,
                    TypeChooser_T<USE_MOVE, PointerType, ConstPointerType> source) {
        OPX_ASSERT(newCapacity >= 0 && newCapacity <= kMaxCapacity && sourceSize >= 0);
        auto data = static_cast<PointerType>(
            Memory::Allocate(sizeof(ElementType) * static_cast<SizeT>(newCapacity)));
        if (sourceSize > 0 && source) {
            Internal::MoveOrCopyConstructItems<ElementType, SizeType, USE_MOVE>(data, source,
                                                                                sourceSize);
        }
        ReleaseInternal();
        mData = data, mCapacity = newCapacity, mSize = sourceSize;
    }

    PointerType BeginReallocate(SizeType newCapacity) {
        mCapacity = newCapacity;
        return static_cast<PointerType>(
            Memory::Allocate(sizeof(ElementType) * static_cast<SizeT>(newCapacity)));
    }

    void EndReallocate(PointerType newData) {
        ReleaseInternal();
        mData = newData;
    }

    void ReleaseInternal() {
        Memory::DestructItems<ElementType, SizeType>(mData, mSize);
        Memory::Free(mData);
    }

    SizeType GetNewCapacity(SizeType numItemsToAdd = 1) {
        if (mCapacity > 0) {
            if (numItemsToAdd < kMaxCapacity - mCapacity) {
                if (mCapacity < kMaxGrowthCapacity) {
                    const SizeType newCapacityA = mCapacity + numItemsToAdd;
                    const SizeType newCapacityB =
                        static_cast<SizeType>(static_cast<Float>(mCapacity) * kGrowthFactor);
                    return OPX_MAX(newCapacityA, newCapacityB);
                }
                return mCapacity + numItemsToAdd;
            } else {
                OPX_ASSERT_MSG(false, "The size of Vector<T> is too large");
            }
        }
        return OPX_MAX(numItemsToAdd, 8);
    }

    template <typename ValueType, typename Fn, Fn FUNC>
    void AppendImpl(SizeType count, ValueType value) {
        OPX_ASSERT(count >= 0);
        if (mCapacity - mSize <= count) {
            auto newData = BeginReallocate(GetNewCapacity(count));
            FUNC(newData + mSize, value, count);
            Internal::MoveOrCopyConstructItems<ElementType, SizeType>(newData, mData, mSize);
            EndReallocate(newData);
        } else {
            FUNC(mData + mSize, value, count);
        }
        mSize += count;
    }

    template <typename ValueType, typename ConstructFn, typename AssignFn>
    SizeType InsertImpl(SizeType index,
                        SizeType num,
                        ValueType value,
                        ConstructFn constructFn,
                        AssignFn assignFn) {
        OPX_ASSERT(index >= 0 && index <= mSize && num >= 0);

        if (mCapacity - mSize <= num) {
            auto newData = BeginReallocate(GetNewCapacity(num));
            Internal::MoveOrCopyConstructItems<ElementType, SizeType>(newData, mData, index);
            constructFn(newData + index, value, num);
            Internal::MoveOrCopyConstructItems<ElementType, SizeType>(newData + index + num,
                                                                      mData + index, mSize - index);
            EndReallocate(newData);
        } else {
            if (index + num >= mSize) {
                auto source = mData + index;
                auto count = mSize - index;
                Internal::MoveOrCopyConstructItems<ElementType, SizeType>(source + num, source,
                                                                          count);
                assignFn(mData + index, value, count);
                if constexpr (IsSame_V<ValueType, ConstPointerType>) {
                    constructFn(mData + mSize, value + count, num - count);
                } else {
                    constructFn(mData + mSize, value, num - count);
                }
            } else {
                auto dest = mData + mSize;
                Internal::MoveOrCopyConstructItems<ElementType, SizeType>(dest, dest - num, num);
                auto count = mSize - index - num;
                auto source = mData + index + count;
                Internal::MoveOrCopyBackwardAssignItems<ElementType, SizeType>(source + num, source,
                                                                               count);
                assignFn(mData + index, value, num);
            }
        }

        mSize += num;
        return index;
    }

    template <typename Fn>
    void ResizeImpl(SizeType newSize, Fn func) {
        if (newSize == mSize) {
            return;
        }

        if (newSize <= mSize) {
            Memory::DestructItems<ElementType, SizeType>(mData + newSize, mSize - newSize);
        } else {
            if (newSize > mCapacity) {
                Reallocate<true>(newSize, mSize, mData);
            }
            func();
        }
        mSize = newSize;
    }

    PointerType mData;
    SizeType mSize;
    SizeType mCapacity;
};

OPX_NAMESPACE_END