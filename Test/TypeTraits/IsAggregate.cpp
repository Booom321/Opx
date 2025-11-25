#include "Framework/Framework.hpp"

#include <Opx/TypeTraits.hpp>

struct A {
    int x;
};
struct B {
    int x;

private:
    int y;
};
struct C {
    C(int) {}
};
struct D {
    int x;
    int y;
};
union U {
    int a;
    float b;
};
class E {};

TEST_CASE(TypeTraits, IsAggregate) {
    TEST_EXPECT_TRUE(Opx::IsAggregate_V<A> && Opx::IsAggregate_V<D> && Opx::IsAggregate_V<U> && Opx::IsAggregate_V<E> && !Opx::IsAggregate_V<B> &&
                     !Opx::IsAggregate_V<C> && !Opx::IsAggregate_V<int> && !Opx::IsAggregate_V<int*> && !Opx::IsAggregate_V<void>);
}