#pragma once

#include <type_traits>

#include "DataTypes.hpp"

OPX_NAMESPACE_BEGIN

template <typename T, T VALUE>
struct IntegralConstant {
    using ValueType = T;
    static OPX_CONSTEXPR T value = VALUE;
};

template <Bool VALUE>
struct BoolConstant : IntegralConstant<Bool, VALUE> {};

using TrueType = BoolConstant<true>;
using FalseType = BoolConstant<false>;

template <Bool VALUE, typename TrueType, typename FalseType>
struct TypeChooser {
    using type = TrueType;
};
template <typename TrueType, typename FalseType>
struct TypeChooser<false, TrueType, FalseType> {
    using type = FalseType;
};
template <Bool VALUE, typename TrueType, typename FalseType>
using TypeChooser_T = typename TypeChooser<VALUE, TrueType, FalseType>::type;

template <typename... Rest>
struct Conjunction : TrueType {};
template <typename T>
struct Conjunction<T> : T {};
template <typename T, typename... Rest>
struct Conjunction<T, Rest...> : TypeChooser_T<Bool(T::value), Conjunction<Rest...>, T> {};
template <typename... Rest>
OPX_CONSTEXPR Bool Conjunction_V = Conjunction<Rest...>::value;

template <typename... Rest>
struct Disjunction : FalseType {};
template <typename T>
struct Disjunction<T> : T {};
template <typename T, typename... Rest>
struct Disjunction<T, Rest...> : TypeChooser_T<Bool(T::value), T, Disjunction<Rest...>> {};
template <typename... Rest>
OPX_CONSTEXPR Bool Disjunction_V = Disjunction<Rest...>::value;

template <typename T>
struct Negation : BoolConstant<!Bool(T::value)> {};
template <typename T>
OPX_CONSTEXPR Bool Negation_V = Negation<T>::value;

template <typename T>
struct RemoveConst {
    using type = T;
};
template <typename T>
struct RemoveConst<const T> {
    using type = T;
};
template <typename T>
using RemoveConst_T = typename RemoveConst<T>::type;

template <typename T>
struct RemoveVolatile {
    using type = T;
};
template <typename T>
struct RemoveVolatile<volatile T> {
    using type = T;
};
template <typename T>
using RemoveVolatile_T = typename RemoveVolatile<T>::type;

template <typename T>
struct RemoveCV : RemoveConst<RemoveVolatile_T<T>> {};
template <typename T>
using RemoveCV_T = typename RemoveCV<T>::type;

template <typename T>
struct AddConst {
    using type = const T;
};
template <typename T>
using AddConst_T = typename AddConst<T>::type;

template <typename T>
struct AddVolatile {
    using type = volatile T;
};
template <typename T>
using AddVolatile_T = typename AddVolatile<T>::type;

template <typename T>
struct AddCV : AddConst<AddVolatile_T<T>> {};
template <typename T>
using AddCV_T = typename AddCV<T>::type;

template <typename T>
struct RemoveReference {
    using type = T;
};
template <typename T>
struct RemoveReference<T&> {
    using type = T;
};
template <typename T>
struct RemoveReference<T&&> {
    using type = T;
};
template <typename T>
using RemoveReference_T = typename RemoveReference<T>::type;

template <typename T>
struct RemovePointer {
    using type = T;
};
template <typename T>
struct RemovePointer<T*> {
    using type = T;
};
template <typename T>
struct RemovePointer<T* const> {
    using type = T;
};
template <typename T>
struct RemovePointer<T* volatile> {
    using type = T;
};
template <typename T>
struct RemovePointer<T* const volatile> {
    using type = T;
};
template <typename T>
using RemovePointer_T = typename RemovePointer<T>::type;

template <typename T>
struct TypeIdentity {
    using type = T;
};
template <typename T>
using TypeIdentity_T = typename TypeIdentity<T>::type;

namespace Details {
    template <typename T>
    auto TryAddPointer(Int32) -> TypeIdentity<RemoveReference_T<T>*>;
    template <typename T>
    auto TryAddPointer(...) -> TypeIdentity<T>;

    template <typename T>
    auto TryAddLvalueReference(int) -> TypeIdentity<T&>;
    template <typename T>
    auto TryAddLvalueReference(...) -> TypeIdentity<T>;

    template <typename T>
    auto TryAddRvalueReference(int) -> TypeIdentity<T&&>;
    template <typename T>
    auto TryAddRvalueReference(...) -> TypeIdentity<T>;

    template <typename B>
    TrueType TestPtrConv(const volatile B*);
    template <typename>
    FalseType TestPtrConv(const volatile void*);

    template <typename T>
    struct IsMemberPointerImpl : FalseType {};
    template <typename T, typename U>
    struct IsMemberPointerImpl<T U::*> : TrueType {};
}  // namespace Details

template <typename T>
struct AddPointer : decltype(Details::TryAddPointer<T>(0)) {};
template <typename T>
using AddPointer_T = typename AddPointer<T>::type;

template <typename T>
struct AddLvalueReference : decltype(Details::TryAddLvalueReference<T>(0)) {};
template <typename T>
using AddLvalueReference_T = typename AddLvalueReference<T>::type;

template <typename T>
struct AddRvalueReference : decltype(Details::TryAddRvalueReference<T>(0)) {};
template <typename T>
using AddRvalueReference_T = typename AddRvalueReference<T>::type;

template <typename T>
struct IsConst : FalseType {};
template <typename T>
struct IsConst<const T> : TrueType {};
template <typename T>
OPX_CONSTEXPR Bool IsConst_V = IsConst<T>::value;

template <typename T>
struct IsReference : FalseType {};
template <typename T>
struct IsReference<T&> : TrueType {};
template <typename T>
struct IsReference<T&&> : TrueType {};
template <typename T>
OPX_CONSTEXPR Bool IsReference_V = IsReference<T>::value;

template <typename T>
struct IsMemberPointer : Details::IsMemberPointerImpl<RemoveCV_T<T>> {};
template <typename T>
OPX_CONSTEXPR Bool IsMemberPointer_V = IsMemberPointer<T>::value;

template <typename T, typename U>
struct IsSame : FalseType {};
template <typename T>
struct IsSame<T, T> : TrueType {};
template <typename T, typename U>
OPX_CONSTEXPR Bool IsSame_V = IsSame<T, U>::value;

template <typename T, typename... Rest>
struct IsAnyOf : Disjunction<IsSame<T, Rest>...> {};
template <typename T, typename... Rest>
OPX_CONSTEXPR Bool IsAnyOf_V = IsAnyOf<T, Rest...>::value;

template <typename T>
struct IsVoid : IsSame<void, RemoveCV_T<T>> {};
template <typename T>
OPX_CONSTEXPR Bool IsVoid_V = IsVoid<T>::value;

template <typename T>
struct IsNullPtr : IsSame<NullPtrT, RemoveCV_T<T>> {};
template <typename T>
OPX_CONSTEXPR Bool IsNullPtr_V = IsNullPtr<T>::value;

template <typename T>
struct IsIntegral : IsAnyOf<RemoveCV_T<T>, bool, char, wchar_t,
#if defined(__cpp_char8_t)
                            char8_t,
#endif
                            char16_t, char32_t, unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long, signed char, short,
                            int, long, long long> {
};
template <typename T>
OPX_CONSTEXPR Bool IsIntegral_V = IsIntegral<T>::value;

template <typename T>
struct IsFloatingPoint : IsAnyOf<RemoveCV_T<T>, float, double, long double> {};
template <typename T>
OPX_CONSTEXPR Bool IsFloatingPoint_V = IsFloatingPoint<T>::value;

template <typename T>
struct IsArray : FalseType {};
template <typename T>
struct IsArray<T[]> : TrueType {};
template <typename T, SizeT N>
struct IsArray<T[N]> : TrueType {};
template <typename T>
OPX_CONSTEXPR Bool IsArray_V = IsArray<T>::value;

template <typename T>
struct IsEnum : BoolConstant<std::is_enum_v<T>> {};
template <typename T>
OPX_CONSTEXPR Bool IsEnum_V = IsEnum<T>::value;

template <typename T>
struct IsUnion : BoolConstant<std::is_union_v<T>> {};
template <typename T>
OPX_CONSTEXPR Bool IsUnion_V = IsUnion<T>::value;

template <typename T>
struct IsFunction : BoolConstant<!IsConst_V<const T> && !IsReference_V<T>> {};
template <typename T>
OPX_CONSTEXPR Bool IsFunction_V = IsFunction<T>::value;

namespace Details {
    template <typename T>
    BoolConstant<!IsUnion_V<T>> TestIsClass(int T::*);
    template <typename>
    FalseType TestIsClass(...);

    template <typename T>
    struct IsMemberFunctionPtrImpl : FalseType {};
    template <typename T, typename U>
    struct IsMemberFunctionPtrImpl<T U::*> : IsFunction<T> {};
}  // namespace Details

template <typename T>
struct IsClass : decltype(Details::TestIsClass<T>(nullptr)) {};
template <typename T>
OPX_CONSTEXPR Bool IsClass_V = IsClass<T>::value;

template <typename T>
struct IsPointer : FalseType {};
template <typename T>
struct IsPointer<T*> : TrueType {};
template <typename T>
struct IsPointer<T* const> : TrueType {};
template <typename T>
struct IsPointer<T* volatile> : TrueType {};
template <typename T>
struct IsPointer<T* const volatile> : TrueType {};
template <typename T>
OPX_CONSTEXPR Bool IsPointer_V = IsPointer<T>::value;

template <typename T>
struct IsLvalueReference : FalseType {};
template <typename T>
struct IsLvalueReference<T&> : TrueType {};
template <typename T>
OPX_CONSTEXPR Bool IsLvalueReference_V = IsLvalueReference<T>::value;

template <typename T>
struct IsRvalueReference : FalseType {};
template <typename T>
struct IsRvalueReference<T&&> : TrueType {};
template <typename T>
OPX_CONSTEXPR Bool IsRvalueReference_V = IsRvalueReference<T>::value;

template <typename T>
struct IsMemberFunctionPointer : Details::IsMemberFunctionPtrImpl<typename std::remove_cv<T>::type> {};
template <typename T>
OPX_CONSTEXPR Bool IsMemberFunctionPointer_V = IsMemberFunctionPointer<T>::value;

template <typename T>
struct IsMemberObjectPointer : BoolConstant<IsMemberPointer_V<T> && !IsMemberFunctionPointer_V<T>> {};
template <typename T>
OPX_CONSTEXPR Bool IsMemberObjectPointer_V = IsMemberObjectPointer<T>::value;

template <typename T>
struct IsArithmetic : BoolConstant<IsIntegral_V<T> || IsFloatingPoint_V<T>> {};
template <typename T>
OPX_CONSTEXPR Bool IsArithmetic_V = IsArithmetic<T>::value;

template <typename T>
struct IsScalar : BoolConstant<IsArithmetic_V<T> || IsEnum_V<T> || IsPointer_V<T> || IsMemberPointer_V<T> || IsNullPtr_V<T>> {};
template <typename T>
OPX_CONSTEXPR Bool IsScalar_V = IsScalar<T>::value;

template <typename T>
struct IsFundamental : BoolConstant<IsArithmetic_V<T> || IsVoid_V<T> || IsNullPtr_V<T>> {};
template <typename T>
OPX_CONSTEXPR Bool IsFundamental_V = IsFundamental<T>::value;

template <typename T>
struct IsObject : BoolConstant<IsScalar_V<T> || IsArray_V<T> || IsUnion_V<T> || IsClass_V<T>> {};
template <typename T>
OPX_CONSTEXPR Bool IsObject_V = IsObject<T>::value;

template <typename T>
struct IsCompound : BoolConstant<!IsFundamental_V<T>> {};
template <typename T>
OPX_CONSTEXPR Bool IsCompound_V = IsCompound<T>::value;

template <typename T>
struct IsVolatile : FalseType {};
template <typename T>
struct IsVolatile<volatile T> : TrueType {};
template <typename T>
OPX_CONSTEXPR Bool IsVolatile_V = IsVolatile<T>::value;

template <typename T>
struct IsTriviallyCopyable : BoolConstant<std::is_trivially_copyable_v<T>> {};
template <typename T>
OPX_CONSTEXPR Bool IsTriviallyCopyable_V = IsTriviallyCopyable<T>::value;

template <typename T>
struct IsStandardLayout : BoolConstant<std::is_standard_layout_v<T>> {};
template <typename T>
OPX_CONSTEXPR Bool IsStandardLayout_V = IsStandardLayout<T>::value;

template <typename T>
struct IsEmpty : BoolConstant<std::is_empty_v<T>> {};
template <typename T>
OPX_CONSTEXPR Bool IsEmpty_V = IsEmpty<T>::value;

namespace Details {
    template <typename T>
    TrueType TestIsPolymorphic(decltype(dynamic_cast<const volatile void*>(static_cast<T*>(nullptr))));
    template <typename T>
    FalseType TestIsPolymorphic(...);

    template <typename T, Bool = IsArithmetic_V<T>>
    struct IsSignedImpl : BoolConstant<T(-1) < T(0)> {};
    template <typename T>
    struct IsSignedImpl<T, false> : FalseType {};

    template <typename T, Bool = IsArithmetic_V<T>>
    struct IsUnsignedImpl : BoolConstant<T(0) < T(-1)> {};
    template <typename T>
    struct IsUnsignedImpl<T, false> : FalseType {};

    template <typename T>
    TrueType TestPtrConv(const volatile T*);
    template <typename>
    FalseType TestPtrConv(const volatile void*);

    template <typename T, typename U>
    auto TestIsBaseOf(int) -> decltype(TestPtrConv<T>(static_cast<U*>(nullptr)));
    template <typename, typename>
    auto TestIsBaseOf(...) -> TrueType;
}  // namespace Details

template <typename T>
struct IsPolymorphic : decltype(Details::TestIsPolymorphic<T>(nullptr)) {};
template <typename T>
OPX_CONSTEXPR Bool IsPolymorphic_V = IsPolymorphic<T>::value;

template <typename T>
struct IsAbstract : BoolConstant<std::is_abstract_v<T>> {};
template <typename T>
OPX_CONSTEXPR Bool IsAbstract_V = IsAbstract<T>::value;

template <typename T>
struct IsFinal : BoolConstant<std::is_final_v<T>> {};
template <typename T>
OPX_CONSTEXPR Bool IsFinal_V = IsFinal<T>::value;

template <typename T>
struct IsAggregate : BoolConstant<std::is_aggregate_v<T>> {};
template <typename T>
OPX_CONSTEXPR Bool IsAggregate_V = IsAggregate<T>::value;

template <typename T>
struct IsSigned : Details::IsSignedImpl<T> {};
template <typename T>
OPX_CONSTEXPR Bool IsSigned_V = IsSigned<T>::value;

template <typename T>
struct IsUnsigned : Details::IsUnsignedImpl<T> {};
template <typename T>
OPX_CONSTEXPR Bool IsUnsigned_V = IsUnsigned<T>::value;

template <typename T>
struct IsBoundedArray : FalseType {};
template <typename T, SizeT N>
struct IsBoundedArray<T[N]> : TrueType {};
template <typename T>
OPX_CONSTEXPR Bool IsBoundedArray_V = IsBoundedArray<T>::value;

template <typename T>
struct IsUnboundedArray : FalseType {};
template <typename T>
struct IsUnboundedArray<T[]> : TrueType {};
template <typename T>
OPX_CONSTEXPR Bool IsUnboundedArray_V = IsUnboundedArray<T>::value;

template <typename Base, typename Derived>
struct IsBaseOf : BoolConstant<IsClass_V<Base> && IsClass_V<Derived>&& decltype(Details::TestIsBaseOf<Base, Derived>(0))::value> {};
template <typename Base, typename Derived>
OPX_CONSTEXPR Bool IsBaseOf_V = IsBaseOf<Base, Derived>::value;

template <typename T>
struct AlignmentOf : IntegralConstant<SizeT, alignof(T)> {};
template <typename T>
OPX_CONSTEXPR SizeT AlignmentOf_V = AlignmentOf<T>::value;

template <typename T>
struct Rank : public IntegralConstant<SizeT, 0> {};
template <typename T>
struct Rank<T[]> : public IntegralConstant<SizeT, Rank<T>::value + 1> {};
template <typename T, SizeT N>
struct Rank<T[N]> : public IntegralConstant<SizeT, Rank<T>::value + 1> {};
template <typename T>
OPX_CONSTEXPR SizeT Rank_V = Rank<T>::value;

template <typename T, unsigned N = 0>
struct Extent : IntegralConstant<SizeT, 0> {};
template <typename T>
struct Extent<T[], 0> : IntegralConstant<SizeT, 0> {};
template <typename T, unsigned N>
struct Extent<T[], N> : Extent<T, N - 1> {};
template <typename T, SizeT I>
struct Extent<T[I], 0> : IntegralConstant<SizeT, I> {};
template <typename T, SizeT I, unsigned N>
struct Extent<T[I], N> : Extent<T, N - 1> {};
template <typename T, unsigned N = 0>
OPX_CONSTEXPR SizeT Extent_V = Extent<T, N>::value;

template <typename T>
struct UnderlyingType {
    using type = std::underlying_type_t<T>;
};
template <typename T>
using UnderlyingType_T = typename UnderlyingType<T>::type;

template <typename T>
struct MakeUnsigned {
    using type = std::make_unsigned_t<T>;
};
template <typename T>
using MakeUnsigned_T = typename MakeUnsigned<T>::type;

template <typename T>
struct MakeSigned {
    using type = std::make_signed_t<T>;
};
template <typename T>
using MakeSigned_T = typename MakeSigned<T>::type;

template <typename T>
struct RemoveExtent {
    using type = T;
};
template <typename T>
struct RemoveExtent<T[]> {
    using type = T;
};
template <typename T, SizeT N>
struct RemoveExtent<T[N]> {
    using type = T;
};
template <typename T>
using RemoveExtent_T = typename RemoveExtent<T>::type;

template <typename T>
struct RemoveAllExtents {
    typedef T type;
};
template <typename T>
struct RemoveAllExtents<T[]> {
    typedef typename RemoveAllExtents<T>::type type;
};
template <typename T, SizeT N>
struct RemoveAllExtents<T[N]> {
    typedef typename RemoveAllExtents<T>::type type;
};
template <typename T>
using RemoveAllExtents_T = typename RemoveAllExtents<T>::type;

template <typename T>
struct Decay {
private:
    using U = RemoveReference_T<T>;

public:
    using type = TypeChooser_T<IsArray_V<U>, AddPointer_T<RemoveExtent_T<U>>, TypeChooser_T<IsFunction_V<U>, AddPointer_T<U>, RemoveCV_T<U>>>;
};
template <typename T>
using Decay_T = typename Decay<T>::type;

template <typename T>
struct RemoveCVRef {
    using type = RemoveCV_T<RemoveReference_T<T>>;
};
template <typename T>
using RemoveCVRef_T = typename RemoveCVRef<T>::type;

template <Bool VALUE, typename T = void>
struct EnableIf {};
template <typename T>
struct EnableIf<true, T> {
    using type = T;
};
template <Bool VALUE, typename T = void>
using EnableIf_T = typename EnableIf<VALUE, T>::type;

template <typename F, typename... ArgTypes>
struct InvokeResult {
    using type = std::invoke_result_t<F, ArgTypes...>;
};
template <typename F, typename... ArgTypes>
using InvokeResult_T = typename InvokeResult<F, ArgTypes...>::type;

template <typename F, typename... ArgTypes>
struct IsInvocable : BoolConstant<std::is_invocable_v<F, ArgTypes...>> {};
template <typename R, typename F, typename... ArgTypes>
struct IsInvocableR : BoolConstant<std::is_invocable_r_v<R, F, ArgTypes...>> {};
template <typename F, typename... ArgTypes>
struct IsNothrowInvocable : BoolConstant<std::is_nothrow_invocable_v<F, ArgTypes...>> {};
template <typename R, typename F, typename... ArgTypes>
struct IsNothrowInvocableR : BoolConstant<std::is_nothrow_invocable_r_v<R, F, ArgTypes...>> {};

template <typename F, typename... ArgTypes>
OPX_CONSTEXPR Bool IsInvocable_V = IsInvocable<F, ArgTypes...>::value;
template <typename R, typename F, typename... ArgTypes>
OPX_CONSTEXPR Bool IsInvocableR_V = IsInvocableR<R, F, ArgTypes...>::value;
template <typename F, typename... ArgTypes>
OPX_CONSTEXPR Bool IsNothrowInvocable_V = IsNothrowInvocable<F, ArgTypes...>::value;
template <typename R, typename F, typename... ArgTypes>
OPX_CONSTEXPR Bool IsNothrowInvocableR_V = IsNothrowInvocableR<R, F, ArgTypes...>::value;

template <typename T>
struct HasVirtualDestructor : BoolConstant<std::has_virtual_destructor_v<T>> {};
template <typename T>
OPX_CONSTEXPR Bool HasVirtualDestructor_V = HasVirtualDestructor<T>::value;

template <typename T, typename... Args>
struct IsConstructible : BoolConstant<std::is_constructible_v<T, Args...>> {};
template <typename T, typename... Args>
OPX_CONSTEXPR Bool IsConstructible_V = IsConstructible<T, Args...>::value;

template <typename T, typename... Args>
struct IsTriviallyConstructible : BoolConstant<std::is_trivially_constructible_v<T, Args...>> {};
template <typename T, typename... Args>
OPX_CONSTEXPR Bool IsTriviallyConstructible_V = IsTriviallyConstructible<T, Args...>::value;

template <typename T, typename... Args>
struct IsNothrowConstructible : BoolConstant<std::is_nothrow_constructible_v<T, Args...>> {};
template <typename T, typename... Args>
OPX_CONSTEXPR Bool IsNothrowConstructible_V = IsNothrowConstructible<T, Args...>::value;

template <typename T>
struct IsDefaultConstructible : BoolConstant<std::is_default_constructible_v<T>> {};
template <typename T>
OPX_CONSTEXPR Bool IsDefaultConstructible_V = IsDefaultConstructible<T>::value;

template <typename T>
struct IsTriviallyDefaultConstructible : BoolConstant<std::is_trivially_default_constructible_v<T>> {};
template <typename T>
OPX_CONSTEXPR Bool IsTriviallyDefaultConstructible_V = IsTriviallyDefaultConstructible<T>::value;

template <typename T>
struct IsNothrowDefaultConstructible : BoolConstant<std::is_nothrow_default_constructible_v<T>> {};
template <typename T>
OPX_CONSTEXPR Bool IsNothrowDefaultConstructible_V = IsNothrowDefaultConstructible<T>::value;

template <typename T>
struct IsCopyConstructible : BoolConstant<std::is_copy_constructible_v<T>> {};
template <typename T>
OPX_CONSTEXPR Bool IsCopyConstructible_V = IsCopyConstructible<T>::value;

template <typename T>
struct IsTriviallyCopyConstructible : BoolConstant<std::is_trivially_copy_constructible_v<T>> {};
template <typename T>
OPX_CONSTEXPR Bool IsTriviallyCopyConstructible_V = IsTriviallyCopyConstructible<T>::value;

template <typename T>
struct IsNothrowCopyConstructible : BoolConstant<std::is_nothrow_copy_constructible_v<T>> {};
template <typename T>
OPX_CONSTEXPR Bool IsNothrowCopyConstructible_V = IsNothrowCopyConstructible<T>::value;

template <typename T>
struct IsMoveConstructible : BoolConstant<std::is_move_constructible_v<T>> {};
template <typename T>
OPX_CONSTEXPR Bool IsMoveConstructible_V = IsMoveConstructible<T>::value;

template <typename T>
struct IsTriviallyMoveConstructible : BoolConstant<std::is_trivially_move_constructible_v<T>> {};
template <typename T>
OPX_CONSTEXPR Bool IsTriviallyMoveConstructible_V = IsTriviallyMoveConstructible<T>::value;

template <typename T>
struct IsNothrowMoveConstructible : BoolConstant<std::is_nothrow_move_constructible_v<T>> {};
template <typename T>
OPX_CONSTEXPR Bool IsNothrowMoveConstructible_V = IsNothrowMoveConstructible<T>::value;

template <typename T, typename U>
struct IsAssignable : BoolConstant<std::is_assignable_v<T, U>> {};
template <typename T, typename U>
OPX_CONSTEXPR Bool IsAssignable_V = IsAssignable<T, U>::value;

template <typename T, typename U>
struct IsTriviallyAssignable : BoolConstant<std::is_trivially_assignable_v<T, U>> {};
template <typename T, typename U>
OPX_CONSTEXPR Bool IsTriviallyAssignable_V = IsTriviallyAssignable<T, U>::value;

template <typename T, typename U>
struct IsNothrowAssignable : BoolConstant<std::is_nothrow_assignable_v<T, U>> {};
template <typename T, typename U>
OPX_CONSTEXPR Bool IsNothrowAssignable_V = IsNothrowAssignable<T, U>::value;

template <typename T>
struct IsCopyAssignable : BoolConstant<std::is_copy_assignable_v<T>> {};
template <typename T>
OPX_CONSTEXPR Bool IsCopyAssignable_V = IsCopyAssignable<T>::value;

template <typename T>
struct IsTriviallyCopyAssignable : BoolConstant<std::is_trivially_copy_assignable_v<T>> {};
template <typename T>
OPX_CONSTEXPR Bool IsTriviallyCopyAssignable_V = IsTriviallyCopyAssignable<T>::value;

template <typename T>
struct IsNothrowCopyAssignable : BoolConstant<std::is_nothrow_copy_assignable_v<T>> {};
template <typename T>
OPX_CONSTEXPR Bool IsNothrowCopyAssignable_V = IsNothrowCopyAssignable<T>::value;

template <typename T>
struct IsMoveAssignable : BoolConstant<std::is_move_assignable_v<T>> {};
template <typename T>
OPX_CONSTEXPR Bool IsMoveAssignable_V = IsMoveAssignable<T>::value;

template <typename T>
struct IsTriviallyMoveAssignable : BoolConstant<std::is_trivially_move_assignable_v<T>> {};
template <typename T>
OPX_CONSTEXPR Bool IsTriviallyMoveAssignable_V = IsTriviallyMoveAssignable<T>::value;

template <typename T>
struct IsNothrowMoveAssignable : BoolConstant<std::is_nothrow_move_assignable_v<T>> {};
template <typename T>
OPX_CONSTEXPR Bool IsNothrowMoveAssignable_V = IsNothrowMoveAssignable<T>::value;

template <typename T>
struct IsDestructible : BoolConstant<std::is_destructible_v<T>> {};
template <typename T>
OPX_CONSTEXPR Bool IsDestructible_V = IsDestructible<T>::value;

template <typename T>
struct IsTriviallyDestructible : BoolConstant<std::is_trivially_destructible_v<T>> {};
template <typename T>
OPX_CONSTEXPR Bool IsTriviallyDestructible_V = IsTriviallyDestructible<T>::value;

template <typename T>
struct IsNothrowDestructible : BoolConstant<std::is_nothrow_destructible_v<T>> {};
template <typename T>
OPX_CONSTEXPR Bool IsNothrowDestructible_V = IsNothrowDestructible<T>::value;

OPX_NAMESPACE_END