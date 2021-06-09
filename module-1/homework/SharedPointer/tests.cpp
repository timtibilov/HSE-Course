#include <algorithm>
#include <string>

#include "gtest/gtest.h"
#include "src/shared_ptr/shared_ptr.h"

// WeakPtr
TEST(WeakExpired, Test1) {
    WeakPtr<int32_t> w;
    {
        auto sp = makeShared<int32_t>(42);
        w = sp;
        ASSERT_FALSE(w.expired());
    }
}

TEST(WeakExpired, Test2) {
    WeakPtr<int32_t> w;
    {
        auto sp = makeShared<int32_t>(42);
        w = sp;
    }
    ASSERT_TRUE(w.expired());
}

TEST(WeakReset, Test3) {
    WeakPtr<int32_t> w;
    auto sp = makeShared<int32_t>(42);
    w = sp;
    w.reset();
    ASSERT_TRUE(w.expired());
}

TEST(WeakLock, Test1) {
    WeakPtr<int32_t> w;
    auto sp = makeShared<int32_t>(42);
    w = sp;
    ASSERT_TRUE(*w.lock() == 42);
}

TEST(WeakLock, Test2) {
    WeakPtr<int32_t> w;
    {
        auto sp = makeShared<int32_t>(42);
        w = sp;
    }
    ASSERT_FALSE(w.lock());
}

// SharedPtr
TEST(SharedMoveConstructor, Test1) {
    class Contrainer {};
    SharedPtr<Contrainer> s1 = makeShared<Contrainer>();
    Contrainer* raw_s1 = s1.get();
    SharedPtr<Contrainer> s2 = std::move(s1);
    ASSERT_TRUE(s1.useCount() == 0 && s1.get() == nullptr && s2.get() == raw_s1 &&
                s2.useCount() == 1);
}

TEST(SharedMoveAssignment, Test1) {
    class Contrainer {};
    SharedPtr<Contrainer> s1 = makeShared<Contrainer>();
    Contrainer* raw_s1 = s1.get();
    SharedPtr<Contrainer> s2;
    s2 = std::move(s1);
    ASSERT_TRUE(s1.useCount() == 0 && s1.get() == nullptr && s2.get() == raw_s1 &&
                s2.useCount() == 1);
}

TEST(SharedAssignment, Test1) {
    class Contrainer {};
    SharedPtr<Contrainer> s1 = makeShared<Contrainer>();
    SharedPtr<Contrainer> s2;
    SharedPtr<Contrainer> s3;
    s2 = s1;
    s3 = s2;

    ASSERT_TRUE(s1.useCount() == 3 && s2.useCount() == 3 && s3.useCount() == 3 &&
                s1.get() == s2.get() && s2.get() == s3.get());
}

TEST(SharedReset, Test1) {
    class Contrainer {};
    SharedPtr<Contrainer> s1 = makeShared<Contrainer>();
    s1.reset();
    ASSERT_FALSE(s1);
}

TEST(SharedReset, Test2) {
    class Contrainer {};
    SharedPtr<Contrainer> s1 = makeShared<Contrainer>();
    Contrainer* p = new Contrainer;
    s1.reset(p);

    ASSERT_TRUE(s1.get() == p && s1.useCount() == 1);
}

TEST(SharedReset, Test3) {
    class Contrainer {};

    SharedPtr<Contrainer> s1 = makeShared<Contrainer>();
    Contrainer* p = new Contrainer;
    s1.reset(p, std::default_delete<Contrainer>());

    ASSERT_TRUE(s1.get() == p && s1.useCount() == 1);
}

TEST(SharedUseCount, Test1) {
    class Contrainer {};

    SharedPtr<Contrainer> s1 = makeShared<Contrainer>();
    SharedPtr<Contrainer> s2 = s1;
    { SharedPtr<Contrainer> s3 = s2; }

    ASSERT_TRUE(s1.useCount() == 2 && s2.useCount() == 2 && s1.get() == s2.get());
}

TEST(SharedSwap, Test1) {
    SharedPtr<int> s1 = makeShared<int32_t>(1);
    SharedPtr<int> s2 = s1;
    SharedPtr<int> s3 = makeShared<int32_t>(2);
    auto raw_s1 = s1.get();
    auto raw_s3 = s3.get();
    s1.swap(s3);

    ASSERT_TRUE(s1.useCount() == 1 && s2.useCount() == 2 && s3.useCount() == 2 &&
                s2.get() == s3.get() && s2.get() == raw_s1 && s1.get() == raw_s3);
}

TEST(SharedIndirectionOperator, Test1) {
    SharedPtr<int> s1 = makeShared<int32_t>(1);
    ASSERT_TRUE(*s1 == 1);
}

TEST(SharedArrowOperator, Test1) {
    struct Contrainer {
        constexpr int Foo() const noexcept {
            return 1;
        }
    };

    SharedPtr<Contrainer> s1 = makeShared<Contrainer>();
    ASSERT_TRUE(s1->Foo() == 1);
}

TEST(SharedBoolOperator, Test1) {
    class Contrainer {};

    SharedPtr<Contrainer> s1 = makeShared<Contrainer>();
    ASSERT_TRUE(s1);
}

TEST(SharedBoolOperator, Test2) {
    class Contrainer {};

    SharedPtr<Contrainer> s1;
    ASSERT_FALSE(s1);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}