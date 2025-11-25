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

OPX_NAMESPACE_END