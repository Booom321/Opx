#include "Framework/Framework.hpp"

#include <Opx/TypeTraits.hpp>

#include <atomic>

int Func();
struct O {
    void operator()() {}
};
struct A {
    static int Foo();
    int Fun() const&;
};
template <typename>
struct PMTraits {};
template <typename T, typename U>
struct PMTraits<U T::*> {
    using MemberType = U;
};

void Func2() noexcept {}
int Func3(int) { return 1; }
double Func4(int, float) noexcept { return 0.0; }

struct Fun {
    long operator()(int) { return 2; }
    int N() const noexcept { return 3; }
};

void Foo() {}

// clang-format off
TEST_CASE(TypeTraits, TypeTraits) {
    using namespace Opx;

    TEST_EXPECT_TRUE(
        IsSame_V<RemoveCV_T<int>, int> && 
        IsSame_V<RemoveCV_T<const int>, int> && 
        IsSame_V<RemoveCV_T<volatile int>, int> &&
        IsSame_V<RemoveCV_T<const volatile int>, int> && 
        IsSame_V<RemoveCV_T<const int*>, const int*> &&
        IsSame_V<RemoveCV_T<int* const>, int*> && 
        IsSame_V<RemoveCV_T<int* volatile>, int*> &&
        IsSame_V<RemoveCV_T<int* const volatile>, int*>
    );

    TEST_EXPECT_TRUE(
        IsSame_V<AddCV_T<int>, const volatile int> &&
        IsSame_V<AddCV_T<const int>, const volatile int> &&
        IsSame_V<AddCV_T<volatile int>, const volatile int> &&
        IsSame_V<AddCV_T<const volatile int>, const volatile int> &&
        IsSame_V<AddCV_T<int*>, int* const volatile> &&
        IsSame_V<AddCV_T<const int*>, const int* const volatile> &&
        IsSame_V<AddCV_T<int* const>, int* const volatile> &&
        IsSame_V<AddCV_T<int* volatile>, int* const volatile>
    );

    TEST_EXPECT_TRUE(
        IsSame_V<RemoveReference_T<int>, int> &&
        IsSame_V<RemoveReference_T<int&>, int> &&
        IsSame_V<RemoveReference_T<int&&>, int> &&
        IsSame_V<RemoveReference_T<const int&>, const int> &&
        IsSame_V<RemoveReference_T<int*&>, int*> &&
        IsSame_V<RemoveReference_T<int*&&>, int*> &&
        IsSame_V<RemoveReference_T<int*>, int*>
    );

    TEST_EXPECT_TRUE(
        IsSame_V<RemovePointer_T<int>, int> &&
        IsSame_V<RemovePointer_T<int*>, int> &&
        IsSame_V<RemovePointer_T<int**>, int*> &&
        IsSame_V<RemovePointer_T<const int*>, const int> &&
        IsSame_V<RemovePointer_T<int* const>, int> &&
        IsSame_V<RemovePointer_T<int* volatile>, int> &&
        IsSame_V<RemovePointer_T<int* const volatile>, int> &&
        IsSame_V<RemovePointer_T<int(*)()>, int()> &&
        IsSame_V<RemovePointer_T<void(*)(int)>, void(int)>
    );

    TEST_EXPECT_TRUE(
        IsSame_V<AddPointer_T<int>, int*> &&
        IsSame_V<AddPointer_T<const int>, const int*> &&
        IsSame_V<AddPointer_T<int&>, int*> &&
        IsSame_V<AddPointer_T<int&&>, int*> &&
        IsSame_V<AddPointer_T<int*>, int**> &&
        IsSame_V<AddPointer_T<void(int)>, void(*)(int)> &&
        IsSame_V<AddPointer_T<int()>, int(*)()>
    );

    TEST_EXPECT_TRUE(
        IsSame_V<AddLvalueReference_T<int>, int&> &&
        IsSame_V<AddLvalueReference_T<const int>, const int&> &&
        IsSame_V<AddLvalueReference_T<int&>, int&> &&
        IsSame_V<AddLvalueReference_T<int&&>, int&> &&
        IsSame_V<AddLvalueReference_T<int*>, int*&> &&
        IsSame_V<AddLvalueReference_T<const int*>, const int*&> &&
        IsSame_V<AddLvalueReference_T<int*&>, int*&> &&
        IsSame_V<AddLvalueReference_T<int*&&>, int*&>
    );

    TEST_EXPECT_TRUE(
        IsSame_V<AddRvalueReference_T<int>, int&&> &&
        IsSame_V<AddRvalueReference_T<const int>, const int&&> &&
        IsSame_V<AddRvalueReference_T<int&>, int&> &&
        IsSame_V<AddRvalueReference_T<int&&>, int&&> &&
        IsSame_V<AddRvalueReference_T<int*>, int*&&> &&
        IsSame_V<AddRvalueReference_T<const int*>, const int*&&> &&
        IsSame_V<AddRvalueReference_T<int*&>, int*&> &&
        IsSame_V<AddRvalueReference_T<int*&&>, int*&&>
    );

    
    TEST_EXPECT_TRUE(
        Conjunction_V<TrueType, TrueType> &&
        !Conjunction_V<TrueType, FalseType> &&
        !Conjunction_V<FalseType, TrueType> &&
        !Conjunction_V<FalseType, FalseType> &&
        Conjunction_V<TrueType, TrueType, TrueType> &&
        !Conjunction_V<TrueType, TrueType, FalseType>
    );

    TEST_EXPECT_TRUE(
        !Disjunction_V<FalseType, FalseType> &&
        Disjunction_V<TrueType, FalseType> &&
        Disjunction_V<FalseType, TrueType> &&
        Disjunction_V<TrueType, TrueType> &&
        !Disjunction_V<FalseType, FalseType, FalseType> &&
        Disjunction_V<FalseType, TrueType, FalseType>
    );
    
    TEST_EXPECT_TRUE(
        Negation_V<FalseType> &&
        !Negation_V<TrueType>
    );

    TEST_EXPECT_TRUE(
        IsAnyOf_V<int, bool, int, float> &&
        IsAnyOf_V<int, int> &&
        !IsAnyOf_V<int, bool, char, double> &&
        !IsAnyOf_V<int>
    );

    TEST_EXPECT_TRUE(
        IsConst_V<const int> &&
        IsConst_V<const long> &&
        !IsConst_V<int> &&
        !IsConst_V<long> &&
        !IsConst_V<int*> &&
        !IsConst_V<int&>
    );

    TEST_EXPECT_TRUE(
        IsReference_V<int&> &&
        IsReference_V<int&&> &&
        IsReference_V<const int&> &&
        !IsReference_V<int> &&
        !IsReference_V<int*> &&
        !IsReference_V<void>
    );

    TEST_EXPECT_TRUE(
        IsVoid_V<void> &&
        IsVoid_V<const void> &&
        IsVoid_V<volatile void> &&
        IsVoid_V<const volatile void> &&
        IsVoid_V<std::void_t<>> &&
        !IsVoid_V<void*> &&
        !IsVoid_V<int> &&
        !IsVoid_V<decltype(Foo)>
    );

    TEST_EXPECT_TRUE(
        IsNullPtr_V<NullPtr> &&
        !IsNullPtr_V<std::nullptr_t*> &&
        !IsNullPtr_V<int*> &&
        !IsNullPtr_V<void*> &&
        !IsNullPtr_V<int> &&
        !IsNullPtr_V<float>
    );

    TEST_EXPECT_TRUE(
        IsIntegral_V<int> &&
        IsIntegral_V<unsigned int> &&
        IsIntegral_V<long> &&
        IsIntegral_V<unsigned long> &&
        IsIntegral_V<short> &&
        IsIntegral_V<unsigned short> &&
        IsIntegral_V<char> &&
        IsIntegral_V<unsigned char> &&
        !IsIntegral_V<float> &&
        !IsIntegral_V<double> &&
        !IsIntegral_V<int*> &&
        !IsIntegral_V<void>
    );

    TEST_EXPECT_TRUE(
        IsFloatingPoint_V<float> &&
        IsFloatingPoint_V<double> &&
        IsFloatingPoint_V<long double> &&
        !IsFloatingPoint_V<int> &&
        !IsFloatingPoint_V<unsigned int> &&
        !IsFloatingPoint_V<int*> &&
        !IsFloatingPoint_V<void>
    );

    TEST_EXPECT_TRUE(
        IsArray_V<int[]> &&
        IsArray_V<int[5]> &&
        IsArray_V<const int[3]> &&
        !IsArray_V<int> &&
        !IsArray_V<int*> &&
        !IsArray_V<void>
    );

    enum EColor { Red, Green, Blue };
    enum class EDirection { North, South, East, West };
    TEST_EXPECT_TRUE(
        IsEnum_V<EColor> &&
        IsEnum_V<EDirection> &&
        !IsEnum_V<int> &&
        !IsEnum_V<float> &&
        !IsEnum_V<int*> &&
        !IsEnum_V<void>
    );

    union MyUnion {
        int i;
        float f;
    };

    struct MyStruct {
        int a;
        float b;
    };
    TEST_EXPECT_TRUE(
        IsUnion_V<MyUnion> &&
        !IsUnion_V<MyStruct> &&
        !IsUnion_V<int> &&
        !IsUnion_V<int*> &&
        !IsUnion_V<void>
    );
    
    class MyClass {};
    TEST_EXPECT_TRUE(
        IsClass_V<MyStruct> &&
        IsClass_V<MyClass> &&
        !IsClass_V<MyUnion> &&
        !IsClass_V<int> &&
        !IsClass_V<int*> &&
        !IsClass_V<void>
    );

    TEST_EXPECT_TRUE(
        IsFunction_V<O()> && 
        IsFunction_V<int(int)> && 
        IsFunction_V<decltype(Func)> && 
        IsFunction_V<decltype(A::Foo)> && 
        IsFunction_V<PMTraits<decltype(&A::Fun)>::MemberType> && 
        !IsFunction_V<A> && 
        !IsFunction_V<int> && 
        !IsFunction_V<decltype(&A::Fun)>
    );

    struct StructA {
        int mData;
        void Func() {}
        int Compute(int) { return 0; }
    };
    TEST_EXPECT_TRUE(
        IsMemberObjectPointer_V<decltype(&StructA::mData)> &&
        !IsMemberObjectPointer_V<decltype(&StructA::Func)> &&
        !IsMemberObjectPointer_V<int> &&
        !IsMemberObjectPointer_V<int*> &&
        !IsMemberObjectPointer_V<void>
    );
    TEST_EXPECT_TRUE(
        IsMemberFunctionPointer_V<decltype(&StructA::Func)> &&
        IsMemberFunctionPointer_V<decltype(&StructA::Compute)> &&
        !IsMemberFunctionPointer_V<decltype(&StructA::mData)> &&
        !IsMemberFunctionPointer_V<int> &&
        !IsMemberFunctionPointer_V<int*>
    );

    TEST_EXPECT_TRUE(
        IsLvalueReference_V<A> == false &&
        IsLvalueReference_V<A&> == true &&
        IsLvalueReference_V<A&&> == false &&
        IsLvalueReference_V<int> == false &&
        IsLvalueReference_V<int&> == true &&
        IsLvalueReference_V<int&&> == false
    );
    TEST_EXPECT_TRUE(
        IsRvalueReference_V<A> == false and
        IsRvalueReference_V<A&> == false and
        IsRvalueReference_V<A&&> != false and
        IsRvalueReference_V<char> == false and
        IsRvalueReference_V<char&> == false and
        IsRvalueReference_V<char&&> != false
    );

    int StructA::*MemberDataPtr = &StructA::mData;
    void (StructA::*MemberFuncPtr)() = &StructA::Func;
    TEST_EXPECT_TRUE(
        IsPointer_V<A*> &&
        IsPointer_V<A const* volatile> &&
        IsPointer_V<void*> &&
        IsPointer_V<int*> &&
        IsPointer_V<int**> &&
        IsPointer_V<void (*)()> &&
        !IsPointer_V<A> &&
        !IsPointer_V<A&> &&
        !IsPointer_V<int> &&
        !IsPointer_V<decltype(MemberDataPtr)> &&
        !IsPointer_V<decltype(MemberFuncPtr)> &&
        !IsPointer_V<int[10]> &&
        !IsPointer_V<NullPtrT>
    );

    TEST_EXPECT_TRUE(
        IsMemberPointer_V<decltype(&StructA::mData)> &&
        IsMemberPointer_V<decltype(&StructA::Func)> &&
        !IsMemberPointer_V<int> &&
        !IsMemberPointer_V<int*>
    );

    TEST_EXPECT_TRUE(
        IsFundamental_V<int> == true &&
        IsFundamental_V<int&> == false &&
        IsFundamental_V<int*> == false &&
        IsFundamental_V<void> == true &&
        IsFundamental_V<void*> == false &&
        IsFundamental_V<float> == true &&
        IsFundamental_V<float&> == false &&
        IsFundamental_V<float*> == false &&
        IsFundamental_V<std::nullptr_t> == true &&
        IsFundamental_V<std::is_fundamental<int>> == false &&
        IsFundamental_V<A> == false &&
        IsFundamental_V<std::is_fundamental<A>::value_type>
    );

    enum class B : int { e };
    TEST_EXPECT_TRUE(
        IsArithmetic_V<bool>             &&
        IsArithmetic_V<char>             &&
        IsArithmetic_V<char const>       &&
        IsArithmetic_V<int>              &&
        IsArithmetic_V<int const>        &&
        IsArithmetic_V<float>            &&
        IsArithmetic_V<float const>      &&
        IsArithmetic_V<std::size_t>      &&
        !IsArithmetic_V<char&>           &&
        !IsArithmetic_V<char*>           &&
        !IsArithmetic_V<int&>            &&
        !IsArithmetic_V<int*>            &&
        !IsArithmetic_V<float&>          &&
        !IsArithmetic_V<float*>          &&
        !IsArithmetic_V<A>               &&
        !IsArithmetic_V<B>               &&
        !IsArithmetic_V<decltype(B::e)>  &&
        !IsArithmetic_V<std::byte>       &&
        !IsArithmetic_V<std::atomic_int> 
    );

    TEST_EXPECT_TRUE(
        IsScalar_V<int> &&
        IsScalar_V<const int> &&
        IsScalar_V<float> &&
        IsScalar_V<bool> &&
        IsScalar_V<int*> &&
        IsScalar_V<void*> &&
        IsScalar_V<std::nullptr_t> &&
        !IsScalar_V<int&> &&
        !IsScalar_V<int[]> &&
        !IsScalar_V<void> &&
        !IsScalar_V<A>
    );


    TEST_EXPECT_TRUE(
        !IsObject_V<void> &&
        IsObject_V<int> &&
        !IsObject_V<int&> &&
        IsObject_V<int*> &&
        !IsObject_V<int*&> &&
        IsObject_V<A> &&
        !IsObject_V<A&> &&
        IsObject_V<A*> &&
        !IsObject_V<int()> &&
        IsObject_V<int(*)()> &&
        !IsObject_V<int(&)()>
    );

    struct S {};
    union U { int a; float b; };
    enum E { A, B, C };

    TEST_EXPECT_TRUE(
        IsCompound_V<S> &&
        IsCompound_V<U> &&
        IsCompound_V<E*> &&
        IsCompound_V<int*> &&
        IsCompound_V<int&> &&
        IsCompound_V<int[]> &&
        !IsCompound_V<int> &&
        !IsCompound_V<void> &&
        !IsCompound_V<const int> &&
        !IsCompound_V<std::nullptr_t>
    );

    TEST_EXPECT_TRUE(
        !IsVolatile_V<int> &&
        IsVolatile_V<volatile int> &&
        IsVolatile_V<volatile const int> &&
        IsVolatile_V<volatile std::valarray<float>> &&
        !IsVolatile_V<std::valarray<volatile float>>
    );

    TEST_EXPECT_TRUE(IsBoundedArray_V<float> == false);
    TEST_EXPECT_TRUE(IsBoundedArray_V<int> == false);
    TEST_EXPECT_TRUE(IsBoundedArray_V<int[]> == false);
    TEST_EXPECT_TRUE(IsBoundedArray_V<int[3]>);

    TEST_EXPECT_TRUE(
        IsUnboundedArray_V<float> == false &&
        IsUnboundedArray_V<int> == false &&
        IsUnboundedArray_V<int[]> == true &&
        IsUnboundedArray_V<int[3]> == false
    );

    TEST_EXPECT_TRUE(
        IsSame_V<MakeUnsigned_T<char>, unsigned char> &&
        IsSame_V<MakeUnsigned_T<int>, unsigned int> &&
        IsSame_V<MakeUnsigned_T<volatile long>, volatile unsigned long>
    );

    enum class EUnsignedEnum : unsigned short {};
    TEST_EXPECT_TRUE(
        IsSame_V<MakeSigned_T<unsigned char>, signed char> &&
        IsSame_V<MakeSigned_T<unsigned int>, signed int> &&
        IsSame_V<MakeSigned_T<volatile unsigned long>, volatile signed long> &&
        IsSame_V<MakeSigned_T<EUnsignedEnum>, signed short>
    );

    enum E1 {};
    enum class E2 {};
    enum class E3 : unsigned short {};
    enum class E4 : int {};
    TEST_EXPECT_TRUE(IsSame_V<UnderlyingType_T<E1>, unsigned int>);
    TEST_EXPECT_TRUE(IsSame_V<UnderlyingType_T<E2>, int>);
    TEST_EXPECT_TRUE(IsSame_V<UnderlyingType_T<E3>, unsigned short>);
    TEST_EXPECT_TRUE(IsSame_V<UnderlyingType_T<E4>, int>);

    TEST_EXPECT_TRUE(
        IsSame_V<RemoveExtent_T<int>, int> &&
        IsSame_V<RemoveExtent_T<int[]>, int> &&
        IsSame_V<RemoveExtent_T<int[5]>, int> &&
        IsSame_V<RemoveExtent_T<const int[3]>, const int> &&
        IsSame_V<RemoveExtent_T<int[][4]>, int[4]> &&
        IsSame_V<RemoveExtent_T<int[2][4]>, int[4]> &&
        IsSame_V<RemoveExtent_T<int*>, int*> &&
        IsSame_V<RemoveExtent_T<void>, void>
    );

    TEST_EXPECT_TRUE(
        IsSame_V<RemoveAllExtents_T<int>, int> &&
        IsSame_V<RemoveAllExtents_T<int[]>, int> &&
        IsSame_V<RemoveAllExtents_T<int[5]>, int> &&
        IsSame_V<RemoveAllExtents_T<const int[3]>, const int> &&
        IsSame_V<RemoveAllExtents_T<int[][4]>, int> &&
        IsSame_V<RemoveAllExtents_T<int[2][4]>, int> &&
        IsSame_V<RemoveAllExtents_T<int*>, int*> &&
        IsSame_V<RemoveAllExtents_T<void>, void>
    );
 
    TEST_EXPECT_TRUE(
        IsSame_V<Decay_T<int>, int> &&
        !IsSame_V<Decay_T<int>, float> &&
        IsSame_V<Decay_T<int&>, int> &&
        IsSame_V<Decay_T<int&&>, int> &&
        IsSame_V<Decay_T<const int&>, int> &&
        IsSame_V<Decay_T<int[2]>, int*> &&
        !IsSame_V<Decay_T<int[4][2]>, int*> &&
        !IsSame_V<Decay_T<int[4][2]>, int**> &&
        IsSame_V<Decay_T<int[4][2]>, int(*)[2]> &&
        IsSame_V<Decay_T<int(int)>, int(*)(int)>
    );

    TEST_EXPECT_TRUE(IsSame_V<RemoveCVRef_T<int>, int>);
    TEST_EXPECT_TRUE(IsSame_V<RemoveCVRef_T<int&>, int>);
    TEST_EXPECT_TRUE(IsSame_V<RemoveCVRef_T<int&&>, int>);
    TEST_EXPECT_TRUE(IsSame_V<RemoveCVRef_T<const int&>, int>);
    TEST_EXPECT_TRUE(IsSame_V<RemoveCVRef_T<const int[2]>, int[2]>);
    TEST_EXPECT_TRUE(IsSame_V<RemoveCVRef_T<const int(&)[2]>, int[2]>);
    TEST_EXPECT_TRUE(IsSame_V<RemoveCVRef_T<int(int)>, int(int)>);

    TEST_EXPECT_TRUE(
        IsSame_V<EnableIf_T<true, int>, int> &&
        IsSame_V<EnableIf_T<true, void>, void> &&
        IsSame_V<EnableIf_T<true, const int>, const int> &&
        IsSame_V<EnableIf_T<true>, void>
    );

    TEST_EXPECT_TRUE(
        IsInvocable_V<decltype(Func2)> &&
        IsInvocable_V<decltype(Func3), int> &&
        IsInvocable_V<decltype(Func4), int, float> &&
        IsInvocable_V<Fun, int> &&
        !IsInvocable_V<decltype(Func3)> &&

        IsInvocableR_V<int, decltype(Func3), int> &&
        IsInvocableR_V<double, decltype(Func4), int, float> &&
        IsInvocableR_V<long, Fun, int> &&
        IsInvocableR_V<void, decltype(Func3), int> &&

        IsNothrowInvocable_V<decltype(Func4), int, float> &&
        IsNothrowInvocable_V<decltype(&Fun::N), const Fun&> &&
        IsNothrowInvocable_V<decltype(Func2)> &&
        !IsNothrowInvocable_V<decltype(Func3), int> &&

        IsNothrowInvocableR_V<double, decltype(Func4), int, float> &&
        IsNothrowInvocableR_V<int, decltype(&Fun::N), const Fun&> &&
        !IsNothrowInvocableR_V<int, decltype(Func3), int>
    );
}
// clang-format on