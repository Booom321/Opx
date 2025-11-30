#include "Framework/Framework.hpp"

#include <Opx/Containers/Vector.hpp>

template <typename T>
Bool ExpectTrue(const Opx::Vector<T>& vec,
                Opx::Int64 expectedSize,
                std::initializer_list<T> expectedValues) {
    if (vec.GetSize() != expectedSize || vec.GetCapacity() < expectedSize) {
        return false;
    }

    const T* expectedVec = expectedValues.begin();
    for (Opx::Int64 i = 0, size = vec.GetSize(); i < size; ++i) {
        if (!(vec[i] == expectedVec[i])) {
            return false;
        }
    }
    return true;
}

#define TEST_VECTOR(vec, expectedSize, ...) \
    TEST_EXPECT_TRUE(ExpectTrue(vec, expectedSize, {__VA_ARGS__}))

class NonTrivial {
public:
    NonTrivial() { value = 0; }
    NonTrivial(int val) { value = val; }
    NonTrivial(int a, int b) { value = a + b; }
    NonTrivial(const NonTrivial& other) { value = other.value; }
    NonTrivial(NonTrivial&& other) noexcept { value = other.value; }
    ~NonTrivial() { value = 0; }

    NonTrivial& operator=(const NonTrivial& other) {
        value = other.value;
        return *this;
    }
    NonTrivial& operator=(NonTrivial&& other) noexcept {
        value = other.value;
        return *this;
    }

    int value;
};

Bool operator==(const NonTrivial& lhs, const NonTrivial& rhs) {
    return lhs.value == rhs.value;
}

Bool operator!=(const NonTrivial& lhs, const NonTrivial& rhs) {
    return lhs.value != rhs.value;
}

static_assert(!Opx::IsTriviallyConstructible_V<NonTrivial>);
static_assert(!Opx::IsTriviallyDestructible_V<NonTrivial>);
static_assert(!Opx::IsTriviallyCopyable_V<NonTrivial>);

TEST_CASE(Containers, Vector) {
    using IntVector = Opx::Vector<Int32>;

    TEST_COMMENT("Constructor");
    {
        IntVector v1{};
        TEST_EXPECT_TRUE(v1.IsEmpty());
        TEST_EXPECT_EQ(v1.GetSize(), 0);
        TEST_EXPECT_EQ(v1.GetCapacity(), 0);

        IntVector v2(3);
        TEST_VECTOR(v2, 3, 0, 0, 0);

        IntVector v3(3, 2);
        TEST_VECTOR(v3, 3, 2, 2, 2);

        Int32 arr[7];
        arr[0] = 0, arr[1] = 1, arr[2] = 2, arr[3] = 3;
        IntVector v4(3, arr);
        TEST_VECTOR(v4, 3, 0, 1, 2);

        IntVector v5{1, 2, 3, 4};
        TEST_VECTOR(v5, 4, 1, 2, 3, 4);

        IntVector v6(v5);
        TEST_VECTOR(v6, 4, 1, 2, 3, 4);

        IntVector v7(Move(v5));
        TEST_VECTOR(v7, 4, 1, 2, 3, 4);
        TEST_EXPECT_EQ(v5.GetCapacity(), 0);
        TEST_EXPECT_EQ(v5.GetSize(), 0);
        TEST_EXPECT_EQ(v5.GetData(), nullptr);

        IntVector v8({});
        TEST_EXPECT_TRUE(v8.IsEmpty());
        TEST_EXPECT_EQ(v8.GetSize(), 0);
        TEST_EXPECT_EQ(v8.GetCapacity(), 0);
    }

    TEST_COMMENT("Element access");
    {
        IntVector v{1, 2, 3, 4};

        TEST_ASSERT_NEQ(v.GetData(), nullptr);
        TEST_EXPECT_EQ(v[0], 1);
        TEST_EXPECT_EQ(v[1], 2);
        TEST_EXPECT_EQ(v[2], 3);
        TEST_EXPECT_EQ(v[3], 4);
        TEST_EXPECT_EQ(v.At(0), 1);
        TEST_EXPECT_EQ(v.At(1), 2);
        TEST_EXPECT_EQ(v.At(2), 3);
        TEST_EXPECT_EQ(v.At(3), 4);
        TEST_EXPECT_EQ(v.First(), 1);
        TEST_EXPECT_EQ(v.Last(), 4);

        v[0] = 0;
        TEST_VECTOR(v, 4, 0, 2, 3, 4);

        v.At(1) = 1;
        TEST_VECTOR(v, 4, 0, 1, 3, 4);

        v.First() = 1;
        TEST_VECTOR(v, 4, 1, 1, 3, 4);

        v.Last() = 3;
        TEST_VECTOR(v, 4, 1, 1, 3, 3);
    }

    TEST_COMMENT("Iterator");
    {
        IntVector v{1, 2, 3, 4};

        IntVector v1(v);
        Int64 index = 0;
        for (int x : v1) {
            TEST_EXPECT_EQ(x, v[index++]);
        }

        const IntVector v2(v);
        index = 0;
        for (const int x : v2) {
            TEST_EXPECT_EQ(x, v[index++]);
        }
    }

    TEST_COMMENT("Assign");
    {
        IntVector v{};

        IntVector v1{1, 2, 3, 4};
        TEST_EXPECT_EQ(v1[0], 1);
        TEST_EXPECT_EQ(v1[1], 2);
        TEST_EXPECT_EQ(v1[2], 3);
        TEST_EXPECT_EQ(v1[3], 4);

        IntVector v2{};

        IntVector v3{1, 1, 2, 2, 3, 3};

        v = v2;
        TEST_EXPECT_TRUE(v.IsEmpty());

        v = v1;
        TEST_VECTOR(v, 4, 1, 2, 3, 4);

        v = Move(v3);
        TEST_VECTOR(v, 6, 1, 1, 2, 2, 3, 3);
        TEST_EXPECT_EQ(v3.GetCapacity(), 0);
        TEST_EXPECT_EQ(v3.GetSize(), 0);
        TEST_EXPECT_EQ(v3.GetData(), nullptr);

        v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
        TEST_VECTOR(v, 12, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12);

        v = {1, 2};
        TEST_VECTOR(v, 2, 1, 2);

        v = {};
        TEST_EXPECT_TRUE(v.IsEmpty());
    }

    TEST_COMMENT("Clear, Reserve, Resize, Shrink");
    {
        IntVector v{1, 2, 3};

        v.Resize(6);
        TEST_VECTOR(v, 6, 1, 2, 3, 0, 0, 0);

        int x = 1;
        v.Resize(10, x);
        TEST_VECTOR(v, 10, 1, 2, 3, 0, 0, 0, 1, 1, 1, 1);

        v.Resize(3);
        TEST_VECTOR(v, 3, 1, 2, 3);

        v.Reserve(13);
        TEST_VECTOR(v, 3, 1, 2, 3);
        TEST_EXPECT_EQ(v.GetCapacity(), 13);

        v.Reserve(2);
        TEST_VECTOR(v, 3, 1, 2, 3);
        TEST_EXPECT_EQ(v.GetCapacity(), 13);

        v.Shrink();
        TEST_VECTOR(v, 3, 1, 2, 3);
        TEST_EXPECT_EQ(v.GetCapacity(), v.GetSize());

        v.Clear();
        TEST_EXPECT_TRUE(v.IsEmpty());
    }

    TEST_COMMENT("== / !=");
    {
        IntVector v1{1, 2, 3, 4};
        IntVector v2{-1, -2, -3, -4};
        IntVector v3{1, 2, 3, 4};

        TEST_EXPECT_EQ(v1, v3);
        TEST_EXPECT_NEQ(v1, v2);
    }

    TEST_COMMENT("Push, PushDefaulted");
    {
        IntVector v{};
        v.Push(1);
        v.Push(1) += 1;
        TEST_VECTOR(v, 2, 1, 2);

        v.PushDefaulted(3);
        TEST_VECTOR(v, 5, 1, 2, 0, 0, 0);
    }

    TEST_COMMENT("RemoveAt, Pop");
    {
        IntVector v{1, 2, 3, 4, 5, 6, 7, 8, 9};

        v.RemoveAt(1, 4);
        TEST_VECTOR(v, 5, 1, 6, 7, 8, 9);

        int x = v.Pop();
        TEST_VECTOR(v, 4, 1, 6, 7, 8);
        TEST_EXPECT_EQ(x, 9);
    }

    TEST_COMMENT("Append");
    {
        IntVector v{};

        IntVector v1{1, 2, 3, 4};
        IntVector v2{2, 2, 2, 2, 2};

        v.Append(3, 1);
        TEST_VECTOR(v, 3, 1, 1, 1);

        v.Append({1, 2, 3});
        TEST_VECTOR(v, 6, 1, 1, 1, 1, 2, 3);

        v.Append(v1);
        TEST_VECTOR(v, 10, 1, 1, 1, 1, 2, 3, 1, 2, 3, 4);

        v.Append(2, v2.GetData());
        TEST_VECTOR(v, 12, 1, 1, 1, 1, 2, 3, 1, 2, 3, 4, 2, 2);
    }

    TEST_COMMENT("+=");
    {
        IntVector v{};

        IntVector v1{1, 2, 3, 4};

        v += {1, 2, 3};
        TEST_VECTOR(v, 3, 1, 2, 3);
        v += {};
        TEST_VECTOR(v, 3, 1, 2, 3);
        v += v1;
        TEST_VECTOR(v, 7, 1, 2, 3, 1, 2, 3, 4);
    }

    TEST_COMMENT("EmplaceAt, InsertDefaulted");
    {
        IntVector v{1, 2, 3};

        v.EmplaceAt(1, 1);
        TEST_VECTOR(v, 4, 1, 1, 2, 3);

        v.EmplaceAt(4, 4);
        TEST_VECTOR(v, 5, 1, 1, 2, 3, 4);

        v.Clear();

        v = {1, 2};
        v.InsertDefaulted(0);
        TEST_VECTOR(v, 3, 0, 1, 2);

        v.InsertDefaulted(2) = 5;
        TEST_VECTOR(v, 4, 0, 1, 5, 2);
    }

    TEST_COMMENT("Insert");
    {
        IntVector v{1, 2, 3};

        IntVector v1{1, 2, 3, 4};

        v.Insert(1, {2, 2, 2});
        TEST_VECTOR(v, 6, 1, 2, 2, 2, 2, 3);
        v.Insert(4, v1);
        TEST_VECTOR(v, 10, 1, 2, 2, 2, 1, 2, 3, 4, 2, 3);

        v = {1, 2};

        v.Insert(1, 3, 1);
        TEST_VECTOR(v, 5, 1, 1, 1, 1, 2);

        v.Insert(3, 2, 2);
        TEST_VECTOR(v, 7, 1, 1, 1, 2, 2, 1, 2);
    }

    TEST_COMMENT("Filter");
    {
        IntVector v{1, 2, 2, 3, 4, 6, 8, 7};
        IntVector res{};
        v.Filter(res, [](int x) { return x % 2 == 0; });
        TEST_VECTOR(res, 5, 2, 2, 4, 6, 8);
    }

    TEST_COMMENT("Map, ForEach");
    {
        IntVector v{1, 2, 2, 3, 4, 6, 8, 7};

        IntVector mapResult{v.Map([](int& x) { x = 1; })};
        IntVector forEachResult{v};
        forEachResult.ForEach([](int& x) { x += 1; });

        TEST_VECTOR(mapResult, 8, 1, 1, 1, 1, 1, 1, 1, 1);
        TEST_VECTOR(forEachResult, 8, 2, 3, 3, 4, 5, 7, 9, 8);
    }

    TEST_COMMENT("Slice");
    {
        IntVector v{1, 2, 2, 3, 4, 6, 8, 7};

        IntVector v1{v.Slice(3)};
        IntVector v2{v.Slice(3, 3)};
        IntVector v3{v.Slice(3, -3)};
        IntVector v4{v.Slice(3, 5)};
        IntVector v5{v.Slice(3, 6)};

        TEST_VECTOR(v1, 5, 3, 4, 6, 8, 7);
        TEST_VECTOR(v2, 3, 3, 4, 6);
        TEST_VECTOR(v3, 5, 3, 4, 6, 8, 7);
        TEST_VECTOR(v4, 5, 3, 4, 6, 8, 7);
        TEST_VECTOR(v5, 5, 3, 4, 6, 8, 7);
    }

    TEST_COMMENT("Reverse");
    {
        IntVector v1{1, 2, 2, 3, 4, 6, 8, 7};
        IntVector v2{};
        IntVector v3{1, 2};
        IntVector v4({1});
        IntVector v5({1, 2, 3});

        v1.Reverse();
        v2.Reverse();
        v3.Reverse();
        v4.Reverse();
        v5.Reverse();

        TEST_VECTOR(v1, 8, 7, 8, 6, 4, 3, 2, 2, 1);
        TEST_EXPECT_TRUE(v2.IsEmpty());
        TEST_VECTOR(v3, 2, 2, 1);
        TEST_VECTOR(v4, 1, 1);
        TEST_VECTOR(v5, 3, 3, 2, 1);
    }

    TEST_COMMENT("operator+");
    {
        IntVector a{1, 2, 3};
        IntVector b{1, 2};
        IntVector c{};

        IntVector v1 = a + b;
        IntVector v2 = a + c;
        IntVector v3 = b + c + a;
        IntVector v4 = a + IntVector{1, 1, 1};
        IntVector v5 = IntVector{2, 3, 4} + b;
        IntVector v6 = IntVector{2, 3, 4} + IntVector{1, 2};

        TEST_VECTOR(v1, 5, 1, 2, 3, 1, 2);
        TEST_VECTOR(v2, 3, 1, 2, 3);
        TEST_VECTOR(v3, 5, 1, 2, 1, 2, 3);
        TEST_VECTOR(v4, 6, 1, 2, 3, 1, 1, 1);
        TEST_VECTOR(v6, 5, 2, 3, 4, 1, 2);
    }
}

TEST_CASE(Containers, VectorWithNonTrivial) {
    using NT = NonTrivial;
    using NTVector = Opx::Vector<NonTrivial>;

    TEST_COMMENT("Constructor");
    {
        NTVector v1{};
        TEST_EXPECT_TRUE(v1.IsEmpty());
        TEST_EXPECT_EQ(v1.GetSize(), 0);
        TEST_EXPECT_EQ(v1.GetCapacity(), 0);

        NTVector v2(3);
        TEST_VECTOR(v2, 3, {}, {}, {});

        NTVector v3(2, NT(1, 2));
        TEST_VECTOR(v3, 2, {3}, {3}, {3});

        NT arr[5];
        arr[0] = {0}, arr[1] = {1}, arr[2] = {2}, arr[3] = {3};
        NTVector v4(3, arr);
        TEST_VECTOR(v4, 3, {0}, {1}, {2});

        NTVector v5{{1}, {2}, {3}, {4}};
        TEST_VECTOR(v5, 4, {1}, {2}, {3}, {4});

        NTVector v6(v5);
        TEST_VECTOR(v6, 4, {1}, {2}, {3}, {4});

        NTVector v7(Move(v5));
        TEST_VECTOR(v7, 4, {1}, {2}, {3}, {4});
        TEST_EXPECT_EQ(v5.GetCapacity(), 0);
        TEST_EXPECT_EQ(v5.GetSize(), 0);
        TEST_EXPECT_EQ(v5.GetData(), nullptr);

        NTVector v8({});
        TEST_EXPECT_TRUE(v8.IsEmpty());
        TEST_EXPECT_EQ(v8.GetSize(), 0);
        TEST_EXPECT_EQ(v8.GetCapacity(), 0);
    }

    TEST_COMMENT("Element access");
    {
        NTVector v{{1}, {2}, {3}, {4}};

        TEST_ASSERT_NEQ(v.GetData(), nullptr);
        TEST_EXPECT_EQ(v[0], NT{1});
        TEST_EXPECT_EQ(v[1], NT{2});
        TEST_EXPECT_EQ(v[2], NT{3});
        TEST_EXPECT_EQ(v[3], NT{4});
        TEST_EXPECT_EQ(v.At(0), NT{1});
        TEST_EXPECT_EQ(v.At(1), NT{2});
        TEST_EXPECT_EQ(v.At(2), NT{3});
        TEST_EXPECT_EQ(v.At(3), NT{4});
        TEST_EXPECT_EQ(v.First(), NT{1});
        TEST_EXPECT_EQ(v.Last(), NT{4});

        v[0] = {0};
        TEST_VECTOR(v, 4, {0}, {2}, {3}, {4});

        v.At(1) = {1};
        TEST_VECTOR(v, 4, {0}, {1}, {3}, {4});

        v.First() = {1};
        TEST_VECTOR(v, 4, {1}, {1}, {3}, {4});

        v.Last() = {3};
        TEST_VECTOR(v, 4, {1}, {1}, {3}, {3});
    }

    TEST_COMMENT("Iterator");
    {
        NTVector v{{1}, {2}, {3}, {4}};

        Int64 index = 0;
        NTVector v1(v);
        for (NT& x : v1) {
            TEST_EXPECT_EQ(x, v[index++]);
        }

        index = 0;
        const NTVector v2(v);
        for (const NT& x : v2) {
            TEST_EXPECT_EQ(x, v[index++]);
        }
    }

    TEST_COMMENT("Assign");
    {
        NTVector v{};

        NTVector v1{{1}, {2}, {3}, {4}};
        TEST_EXPECT_EQ(v1[0], NT{1});
        TEST_EXPECT_EQ(v1[1], NT{2});
        TEST_EXPECT_EQ(v1[2], NT{3});
        TEST_EXPECT_EQ(v1[3], NT{4});

        NTVector v2{};

        NTVector v3{{1}, {1}, {2}, {2}, {3}, {3}};

        v = v2;
        TEST_EXPECT_TRUE(v.IsEmpty());

        v = v1;
        TEST_VECTOR(v, 4, {1}, {2}, {3}, {4});

        v = Move(v3);
        TEST_VECTOR(v, 6, {1}, {1}, {2}, {2}, {3}, {3});
        TEST_EXPECT_EQ(v3.GetCapacity(), 0);
        TEST_EXPECT_EQ(v3.GetSize(), 0);
        TEST_EXPECT_EQ(v3.GetData(), nullptr);

        v = {{1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9}, {10}, {11}, {12}};
        TEST_VECTOR(v, 12, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9}, {10}, {11}, {12});

        v = {{1}, {2}};
        TEST_VECTOR(v, 2, {1}, {2});

        v = {};
        TEST_EXPECT_TRUE(v.IsEmpty());
    }

    TEST_COMMENT("Clear, Reserve, Resize, Shrink");
    {
        NTVector v{{1}, {2}, {3}};

        v.Resize(6);
        TEST_VECTOR(v, 6, {1}, {2}, {3}, {0}, {0}, {0});

        NT x{1};
        v.Resize(10, x);
        TEST_VECTOR(v, 10, {1}, {2}, {3}, {0}, {0}, {0}, {1}, {1}, {1}, {1});

        v.Resize(3);
        TEST_VECTOR(v, 3, {1}, {2}, {3});

        v.Reserve(13);
        TEST_VECTOR(v, 3, {1}, {2}, {3});
        TEST_EXPECT_EQ(v.GetCapacity(), 13);

        v.Reserve(2);
        TEST_VECTOR(v, 3, {1}, {2}, {3});
        TEST_EXPECT_EQ(v.GetCapacity(), 13);

        v.Shrink();
        TEST_VECTOR(v, 3, {1}, {2}, {3});
        TEST_EXPECT_EQ(v.GetCapacity(), v.GetSize());

        v.Clear();
        TEST_EXPECT_TRUE(v.IsEmpty());
    }

    TEST_COMMENT("== / !=");
    {
        NTVector v1{{1}, {2}, {3}, {4}};
        NTVector v2{{-1}, {-2}, {-3}, {-4}};
        NTVector v3{{1}, {2}, {3}, {4}};

        TEST_EXPECT_EQ(v1, v3);
        TEST_EXPECT_NEQ(v1, v2);
    }

    TEST_COMMENT("Push, PushDefaulted");
    {
        NTVector v{};
        v.Push(1, 2);
        v.Push(1, 2).value += 2;
        TEST_VECTOR(v, 2, {3}, {5});

        v.PushDefaulted(3);
        TEST_VECTOR(v, 5, {3}, {5}, {0}, {0}, {0});
    }

    TEST_COMMENT("RemoveAt, Pop");
    {
        NTVector v{{1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9}};

        v.RemoveAt(1, 4);
        TEST_VECTOR(v, 5, {1}, {6}, {7}, {8}, {9});

        NT x = v.Pop();
        TEST_VECTOR(v, 4, {1}, {6}, {7}, {8});
        TEST_EXPECT_EQ(x, NT{9});
    }

    TEST_COMMENT("Append");
    {
        NTVector v{};

        NTVector v1{{1}, {2}, {3}, {4}};
        NTVector v2{{2}, {2}, {2}, {2}, {2}};

        v.Append(3, {1});
        TEST_VECTOR(v, 3, {1}, {1}, {1});

        v.Append({{1}, {2}, {3}});
        TEST_VECTOR(v, 6, {1}, {1}, {1}, {1}, {2}, {3});

        v.Append(v1);
        TEST_VECTOR(v, 10, {1}, {1}, {1}, {1}, {2}, {3}, {1}, {2}, {3}, {4});

        v.Append(2, v2.GetData());
        TEST_VECTOR(v, 12, {1}, {1}, {1}, {1}, {2}, {3}, {1}, {2}, {3}, {4}, {2}, {2});
    }

    TEST_COMMENT("+=");
    {
        NTVector v{};

        NTVector v1{{1}, {2}, {3}, {4}};

        v += {{1}, {2}, {3}};
        TEST_VECTOR(v, 3, {1}, {2}, {3});
        v += {};
        TEST_VECTOR(v, 3, {1}, {2}, {3});
        v += v1;
        TEST_VECTOR(v, 7, {1}, {2}, {3}, {1}, {2}, {3}, {4});
    }

    TEST_COMMENT("EmplaceAt, InsertDefaulted");
    {
        NTVector v{{1}, {2}, {3}};

        v.EmplaceAt(1, 1, 2);
        TEST_VECTOR(v, 4, {1}, {3}, {2}, {3});

        v.EmplaceAt(4, 4).value += 5;
        TEST_VECTOR(v, 5, {1}, {3}, {2}, {3}, {9});

        v.Clear();

        v = {{1}, {2}};
        v.InsertDefaulted(0);
        TEST_VECTOR(v, 3, {0}, {1}, {2});

        v.InsertDefaulted(2).value = 5;
        TEST_VECTOR(v, 4, {0}, {1}, {5}, {2});
    }

    TEST_COMMENT("Insert");
    {
        NTVector v{{1}, {2}, {3}};

        NTVector v1{{1}, {2}, {3}, {4}};

        v.Insert(1, {{2}, {2}, {2}});
        TEST_VECTOR(v, 6, {1}, {2}, {2}, {2}, {2}, {3});
        v.Insert(4, v1);
        TEST_VECTOR(v, 10, {1}, {2}, {2}, {2}, {1}, {2}, {3}, {4}, {2}, {3});

        v = {{1}, {2}};

        v.Insert(1, 3, {1});
        TEST_VECTOR(v, 5, {1}, {1}, {1}, {1}, {2});

        v.Insert(3, 2, {2});
        TEST_VECTOR(v, 7, {1}, {1}, {1}, {2}, {2}, {1}, {2});
    }

    TEST_COMMENT("Filter");
    {
        NTVector v{{1}, {2}, {2}, {3}, {4}, {6}, {8}, {7}};
        NTVector res{};
        v.Filter(res, [](const NT& x) { return x.value % 2 == 0; });
        TEST_VECTOR(res, 5, {2}, {2}, {4}, {6}, {8});
    }

    TEST_COMMENT("Map, ForEach");
    {
        NTVector v{{1}, {2}, {2}, {3}, {4}, {6}, {8}, {7}};

        NTVector mapResult{v.Map([](NT& x) { x.value = 1; })};
        NTVector forEachResult{v};
        forEachResult.ForEach([](NT& x) { x.value += 1; });

        TEST_VECTOR(mapResult, 8, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1});
        TEST_VECTOR(forEachResult, 8, {2}, {3}, {3}, {4}, {5}, {7}, {9}, {8});
    }

    TEST_COMMENT("Slice");
    {
        NTVector v{{1}, {2}, {2}, {3}, {4}, {6}, {8}, {7}};

        NTVector v1{v.Slice(3)};
        NTVector v2{v.Slice(3, 3)};
        NTVector v3{v.Slice(3, -3)};
        NTVector v4{v.Slice(3, 5)};
        NTVector v5{v.Slice(3, 6)};

        TEST_VECTOR(v1, 5, {3}, {4}, {6}, {8}, {7});
        TEST_VECTOR(v2, 3, {3}, {4}, {6});
        TEST_VECTOR(v3, 5, {3}, {4}, {6}, {8}, {7});
        TEST_VECTOR(v4, 5, {3}, {4}, {6}, {8}, {7});
        TEST_VECTOR(v5, 5, {3}, {4}, {6}, {8}, {7});
    }

    TEST_COMMENT("Reverse");
    {
        NTVector v1{{1}, {2}, {2}, {3}, {4}, {6}, {8}, {7}};
        NTVector v2{};
        NTVector v3{{1}, {2}};
        NTVector v4{{1}};
        NTVector v5{{1}, {2}, {3}};

        v1.Reverse();
        v2.Reverse();
        v3.Reverse();
        v4.Reverse();
        v5.Reverse();

        TEST_VECTOR(v1, 8, {7}, {8}, {6}, {4}, {3}, {2}, {2}, {1});
        TEST_EXPECT_TRUE(v2.IsEmpty());
        TEST_VECTOR(v3, 2, {2}, {1});
        TEST_VECTOR(v4, 1, {1});
        TEST_VECTOR(v5, 3, {3}, {2}, {1});
    }

    TEST_COMMENT("operator+");
    {
        NTVector a{{1}, {2}, {3}};
        NTVector b{{1}, {2}};
        NTVector c{};

        NTVector v1 = a + b;
        NTVector v2 = a + c;
        NTVector v3 = b + c + a;
        NTVector v4 = a + NTVector{{1}, {1}, {1}};
        NTVector v5 = NTVector{{2}, {3}, {4}} + b;
        NTVector v6 = NTVector{{2}, {3}, {4}} + NTVector{{1}, {2}};

        TEST_VECTOR(v1, 5, {1}, {2}, {3}, {1}, {2});
        TEST_VECTOR(v2, 3, {1}, {2}, {3});
        TEST_VECTOR(v3, 5, {1}, {2}, {1}, {2}, {3});
        TEST_VECTOR(v4, 6, {1}, {2}, {3}, {1}, {1}, {1});
        TEST_VECTOR(v6, 5, {2}, {3}, {4}, {1}, {2});
    }
}