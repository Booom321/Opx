#include <Framework/Framework.hpp>

#include <Opx/Utility/Singleton.hpp>

static const Char* message = "";

class Base : public Opx::Singleton<Base> {
public:
    Base() = default;
    virtual ~Base() = default;

    virtual void Foo() = 0;
};

class Derived : public Base {
public:
    Derived(const char* text) : Base() { message = text; }

    virtual ~Derived() { message = "Derived deleted"; }

    void Foo() override { message = "Foo"; }
};

TEST_CASE(Utility, Singleton) {
    Base::Create<Derived>("Derived created");
    TEST_EXPECT_STR_EQ(message, "Derived created");

    Base::Get()->Foo();
    TEST_EXPECT_STR_EQ(message, "Foo");

    message = "";
    auto instance = Base::Get<Derived>();
    TEST_EXPECT_TRUE(Opx::IsSame_V<decltype(instance), Derived*>);
    instance->Foo();
    TEST_EXPECT_STR_EQ(message, "Foo");

    Base::Delete();
    TEST_EXPECT_STR_EQ(message, "Derived deleted");
}