#include <gtest/gtest.h>
#include <algorithm>
#include <sstream>
#include <numeric>

#include <broker_system/RingBuffer.h>

static inline void getFilledBuffer(RingBuffer<int>& rbuf, size_t n) {
  for (int i = 0; i < n; ++i) {
    rbuf.push(i);
  }; 
}


TEST(ConstIterator, Constructor) {
  RingBuffer<int> rbuf(10);
  getFilledBuffer(rbuf, 10);

  auto cit = rbuf.cbegin();
  ASSERT_EQ(*cit, 0);
}

TEST(ConstIterator, BeginEnd) {
  RingBuffer<int> rbuf(10);
  getFilledBuffer(rbuf, 10);

  int j = 0;
  for (auto it = rbuf.cbegin(); it != rbuf.cend(); ++it) {
    ASSERT_EQ(*it, j++);
  }
}

TEST(ConstIterator, ReferenceAccess) {
  RingBuffer<int> rbuf(5);
  getFilledBuffer(rbuf, 5);

  auto it = rbuf.cbegin();
  ASSERT_EQ(it.operator->()[0], 0);
}

TEST(ConstIterator, lhsIncrement) {
  RingBuffer<int> rbuf(5);
  getFilledBuffer(rbuf, 5);

  auto it = rbuf.cbegin();
  ++it;
  ASSERT_EQ(*it, 1);
}

TEST(ConstIterator, rhsIncrement) {
  RingBuffer<int> rbuf(5);
  getFilledBuffer(rbuf, 5);

  auto it = rbuf.cbegin();
  ASSERT_EQ(*(it++), 0);
  ASSERT_EQ(*it, 1);
}

TEST(ConstIterator, EqualNotEqual) {
  RingBuffer<int> rbuf(5);
  getFilledBuffer(rbuf, 5);

  auto it1 = rbuf.cbegin();
  auto it2 = rbuf.cbegin();

  ASSERT_TRUE(it1 == it2);
  ++it1;
  ASSERT_FALSE(it1 == it2);
  ASSERT_TRUE(it1 != it2);
}

TEST(ConstIterator, EmptyBuffer) {
  RingBuffer<int> rbuf(5);
  ASSERT_TRUE(rbuf.cbegin() == rbuf.cend());
}

TEST(ConstIterator, WrapAround) {
  RingBuffer<int> rbuf(5);
  for (int i = 0; i < 7; ++i) {
    int tmp;
    if (rbuf.full()) rbuf.pop(tmp);
    rbuf.push(i);
  }

  std::vector<int> expected = {2, 3, 4, 5, 6};
  std::vector<int> actual;
  for (auto it = rbuf.cbegin(); it != rbuf.cend(); ++it) {
    actual.push_back(*it);
  }

  ASSERT_EQ(actual, expected);
}

TEST(ConstIterator, CannotModify) {
  RingBuffer<int> rbuf(3);
  getFilledBuffer(rbuf, 3);

  std::vector<int> expected = {0, 1, 2};
  std::vector<int> actual;
  for (auto it = rbuf.cbegin(); it != rbuf.cend(); ++it) {
    actual.push_back(*it);
  }
  ASSERT_EQ(actual, expected);
}

TEST(ConstIterator, EndReached) {
  RingBuffer<int> rbuf(3);
  getFilledBuffer(rbuf, 3);

  auto it = rbuf.cbegin();
  int steps = 0;
  while (it != rbuf.cend()) {
    ++it;
    ++steps;
  }
  ASSERT_EQ(steps, 3);
}

TEST(ConstIterator, STL_compatible) {
  RingBuffer<int> rbuf(4);
  getFilledBuffer(rbuf, 4);

  int sum = std::accumulate(rbuf.cbegin(), rbuf.cend(), 0);
  ASSERT_EQ(sum, 6);
}

