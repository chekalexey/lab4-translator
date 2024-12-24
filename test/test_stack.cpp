#include <gtest.h>
#include <vector>
#include "stack.h"

TEST(MyStack, CanPushElementInStack) {
	MyStack<int, std::vector> s;
	ASSERT_NO_THROW(s.push(1));
}

TEST(MyStack, EmptyOnEmptyStack) {
	MyStack<int, std::vector> s;
	EXPECT_EQ(true,s.empty());
}

TEST(MyStack, EmptyOnStackAfterPush) {
	MyStack<int, std::vector> s;
	s.push(1);
	EXPECT_EQ(false, s.empty());
}

TEST(MyStack, CanGetSize) {
	MyStack<int, std::vector> s;
	EXPECT_EQ(0,s.size());
}

TEST(MyStack, CanGetSizeAfterPush) {
	MyStack<int, std::vector> s;
	s.push(1);
	s.push(2);
	EXPECT_EQ(2,s.size());
}

TEST(MyStack, CanGetSizeAfterPushAndPop) {
	MyStack<int, std::vector> s;
	s.push(1);
	s.push(2);
	s.pop();
	EXPECT_EQ(1,s.size());
}

TEST(MyStack, CanEraseElement) {
	MyStack<int, std::vector> s;
	s.push(1);
	s.push(2);
	ASSERT_NO_THROW(s.pop());
}

TEST(MyStack, ThrowsWhenPopInEmptyStack) {
	MyStack<int, std::vector> s;
	ASSERT_ANY_THROW(s.pop());
}

TEST(MyStack, ThrowsWhenTopInEmptyStack) {
	MyStack<int, std::vector> s;
	ASSERT_ANY_THROW(s.top());
}

TEST(MyStack, GetTopElement) {
	MyStack<int, std::vector> s;
	s.push(1);
	s.push(2);
	EXPECT_EQ(2,s.top());
}

TEST(MyStack, GetTopElementAfterPushAndPop) {
	MyStack<int, std::vector> s;
	s.push(1);
	s.push(2);
	s.pop();
	s.push(3);
	EXPECT_EQ(3,s.top());
}
