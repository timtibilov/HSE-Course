#include <algorithm>
#include <list>
#include <random>
#include <string>

#include "gtest/gtest.h"
#include "src/allocator/allocator.h"
#include "src/list/list.h"

TEST(CopyAssignment, Test) {
    task::list<std::string, CustomAllocator<std::string>> actual;
    task::list<std::string, CustomAllocator<std::string>> actual_copy;
    std::list<std::string, CustomAllocator<std::string>> expected;
    std::list<std::string, CustomAllocator<std::string>> expected_copy;

    for (std::size_t i = 0; i < 10; i++) {
        actual_copy.pushBack("hello");
        expected_copy.push_back("hello");
    }

    actual = actual_copy;
    expected = expected_copy;
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin(), expected.end()));
}

TEST(MoveAssignment, Test1) {
    task::list<std::string> l1;
    task::list<std::string> l2;
    std::list<std::string> l3;
    std::list<std::string> l4;

    l1.pushBack("hello");
    l2 = std::move(l1);
    l3.push_back("hello");
    l4 = std::move(l3);
    ASSERT_TRUE(std::equal(l1.begin(), l1.end(), l3.begin(), l3.end()));
    ASSERT_TRUE(std::equal(l2.begin(), l2.end(), l4.begin(), l4.end()));
}

TEST(Front, Test1) {
    task::list<std::string, CustomAllocator<std::string>> actual;
    std::list<std::string, CustomAllocator<std::string>> expected;

    actual.pushBack("hello");
    expected.push_back("hello");

    actual.pushBack("hello");
    expected.push_back("hello");

    actual.pushBack("hello");
    expected.push_back("hello");

    actual.clear();
    expected.clear();
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin(), expected.end()));
}

TEST(Clear, Test1) {
    task::list<std::string, CustomAllocator<std::string>> actual;
    std::list<std::string, CustomAllocator<std::string>> expected;

    for (std::size_t i = 0; i < 10; i++) {
        actual.pushBack("hello");
        expected.push_back("hello");
    }

    actual.clear();
    expected.clear();
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin(), expected.end()));
}

TEST(Swap, Test1) {
    task::list<std::string, CustomAllocator<std::string>> actual;
    std::list<std::string, CustomAllocator<std::string>> expected;

    for (std::size_t i = 0; i < 10; i++) {
        actual.pushBack("hello");
        expected.push_back("hello");
    }

    task::list<std::string, CustomAllocator<std::string>> actual2;
    std::list<std::string, CustomAllocator<std::string>> expected2;

    for (std::size_t i = 0; i < 10; i++) {
        actual2.pushBack("world");
        expected2.push_back("world");
    }

    actual.swap(actual2);
    expected.swap(expected2);
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin(), expected.end()));
    ASSERT_TRUE(std::equal(actual2.begin(), actual2.end(), expected2.begin(), expected2.end()));
}

TEST(PushBack, Test) {
    std::vector<std::string> actual_v(10, "hello");
    std::vector<std::string> expected_v(10, "hello");
    task::list<std::string, CustomAllocator<std::string>> actual;
    std::list<std::string, CustomAllocator<std::string>> expected;

    for (std::size_t i = 0; i < 10; i++) {
        actual.pushBack(std::move(actual_v[i]));
        expected.push_back(std::move(expected_v[i]));
    }
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin(), expected.end()));
}

TEST(EmplaceBack, Test1) {
    task::list<std::string, CustomAllocator<std::string>> actual;
    std::list<std::string, CustomAllocator<std::string>> expected;

    for (std::size_t i = 0; i < 10; i++) {
        actual.emplaceBack("hello");
        expected.emplace_back("hello");
    }
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin(), expected.end()));
}

TEST(PopBack, Test1) {
    task::list<std::string, CustomAllocator<std::string>> actual;
    std::list<std::string, CustomAllocator<std::string>> expected;

    for (std::size_t i = 0; i < 10; i++) {
        actual.emplaceBack("hello");
        expected.emplace_back("hello");
    }

    for (std::size_t i = 0; i < 5; i++) {
        actual.popBack();
        expected.pop_back();
    }
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin(), expected.end()));
}

TEST(PushFront, Test1) {
    std::vector<std::string> actual_v(10, "hello");
    std::vector<std::string> expected_v(10, "hello");
    task::list<std::string, CustomAllocator<std::string>> actual;
    std::list<std::string, CustomAllocator<std::string>> expected;

    for (std::size_t i = 0; i < 10; i++) {
        actual.pushFront(actual_v[i]);
        expected.push_front(expected_v[i]);
    }
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin(), expected.end()));
}

TEST(PushFront, Test2) {
    std::vector<std::string> actual_v(10, "hello");
    std::vector<std::string> expected_v(10, "hello");
    task::list<std::string, CustomAllocator<std::string>> actual;
    std::list<std::string, CustomAllocator<std::string>> expected;

    for (std::size_t i = 0; i < 10; i++) {
        actual.pushFront(std::move(actual_v[i]));
        expected.push_front(std::move(expected_v[i]));
    }
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin(), expected.end()));
}

TEST(EmplaceFront, Test1) {
    task::list<std::string, CustomAllocator<std::string>> actual;
    std::list<std::string, CustomAllocator<std::string>> expected;

    for (std::size_t i = 0; i < 10; i++) {
        actual.emplaceFront("hello");
        expected.emplace_front("hello");
    }
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin(), expected.end()));
}

TEST(PopFront, Test1) {
    task::list<std::string, CustomAllocator<std::string>> actual;
    std::list<std::string, CustomAllocator<std::string>> expected;

    for (std::size_t i = 0; i < 10; i++) {
        actual.emplaceBack("hello");
        expected.emplace_back("hello");
    }

    for (std::size_t i = 0; i < 5; i++) {
        actual.popFront();
        expected.pop_front();
    }
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin(), expected.end()));
}

TEST(Resize, Test1) {
    task::list<std::string, CustomAllocator<std::string>> actual;
    actual.resize(2);
    ASSERT_EQ(actual.size(), 2);
}

TEST(Remove, Test1) {
    task::list<std::string, CustomAllocator<std::string>> actual;
    actual.pushBack("hello");
    actual.pushBack("world");
    actual.pushBack("hello");
    actual.remove("hello");
    std::list<std::string, CustomAllocator<std::string>> expected;
    expected.push_back("world");
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin(), expected.end()));
}

TEST(Unique, Test1) {
    task::list<std::string, CustomAllocator<std::string>> actual;
    actual.pushBack("hello");
    actual.pushBack("hello");
    actual.pushBack("world");
    actual.pushBack("world");
    actual.unique();
    std::list<std::string, CustomAllocator<std::string>> expected;
    expected.push_back("hello");
    expected.push_back("world");
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin(), expected.end()));
}

TEST(Sort, Test1) {
    std::random_device random_device;
    std::mt19937 random_engine(random_device());
    std::uniform_int_distribution<int> distribution(1, 100);

    task::list<int> actual;
    std::list<int> expected;
    for (size_t i = 0; i < 100; ++i) {
        int value = distribution(random_engine);
        actual.pushBack(value);
        expected.push_back(value);
    }
    actual.sort();
    expected.sort();
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin(), expected.end()));
}

TEST(Mixed, Test1) {
    task::list<std::string, CustomAllocator<std::string>> actual;
    std::list<std::string, CustomAllocator<std::string>> expected;

    for (std::size_t i = 0; i < 5; i++) {
        actual.pushBack("hello");
        expected.push_back("hello");
    }

    for (std::size_t i = 0; i < 3; i++) {
        actual.popFront();
        expected.pop_front();
    }

    for (std::size_t i = 0; i < 5; i++) {
        actual.pushFront("world");
        expected.push_front("world");
    }

    for (std::size_t i = 0; i < 3; i++) {
        actual.popBack();
        expected.pop_back();
    }
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin(), expected.end()));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}