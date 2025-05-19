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

TEST(Iterator, Constructor) {
  int n = 10;
  RingBuffer<int> rbuf(n);
  getFilledBuffer(rbuf, n);

  ASSERT_TRUE(*(rbuf.begin()) == 0);
}

TEST(Iterator, BeginEnd) {
  int n = 10;
  RingBuffer<int> rbuf(n);
  getFilledBuffer(rbuf, n);

  ASSERT_TRUE(*(rbuf.begin()) == 0);
  int j = 0;
  for (auto i : rbuf) {
    ASSERT_TRUE(i == j++);
  }
}

TEST(Iterator, Reference) {
  int n = 10;
  RingBuffer<int> rbuf(n);
  getFilledBuffer(rbuf, n);
  RingBuffer<int>::Iterator It = rbuf.begin();

  ASSERT_TRUE(*(It.operator->()) == 0);
}

TEST(Iterator, lhsIncrement) {
  int n = 10;
  RingBuffer<int> rbuf(n);
  getFilledBuffer(rbuf, n);
  RingBuffer<int>::Iterator It = rbuf.begin();
  ++It;
  ASSERT_TRUE(*It == 1);
}

TEST(Iterator, rhsIncrement) {
  int n = 10;
  RingBuffer<int> rbuf(n);
  getFilledBuffer(rbuf, n);
  RingBuffer<int>::Iterator It = rbuf.begin();
  ASSERT_TRUE(*(It++) == 0);
  ASSERT_TRUE(*It == 1);
}

TEST(Iterator, Equal) {
  int n = 10;
  RingBuffer<int> rbuf(n);
  getFilledBuffer(rbuf, n);
  RingBuffer<int>::Iterator It1 = rbuf.begin();
  RingBuffer<int>::Iterator It2 = rbuf.begin();
  bool tst1 = (It1 == It2);
  bool tst2 = (++It1 == It2);
  ASSERT_TRUE(tst1);
  ASSERT_FALSE(tst2);
}

TEST(Iterator, NotEqual) {
  int n = 10;
  RingBuffer<int> rbuf(n);
  getFilledBuffer(rbuf, n);
  RingBuffer<int>::Iterator It1 = rbuf.begin();
  RingBuffer<int>::Iterator It2 = rbuf.begin();
  bool tst1 = (It1 != It2);
  bool tst2 = (++It1 != It2);
  ASSERT_FALSE(tst1);
  ASSERT_TRUE(tst2);
}

TEST(Iterator, EmptyBuffer) {
  RingBuffer<int> rbuf(10);
  ASSERT_TRUE(rbuf.begin() == rbuf.end());
}

TEST(Iterator, WrapAround) {
  RingBuffer<int> rbuf(5);
  for (int i = 0; i < 7; ++i) {
    int tmp;
    if (rbuf.full()) rbuf.pop(tmp);
    rbuf.push(i);
  }

  std::vector<int> expected = {2, 3, 4, 5, 6};
  std::vector<int> actual;
  for (auto i : rbuf) actual.push_back(i);

  ASSERT_EQ(actual, expected);
}

TEST(Iterator, ModifyThroughIterator) {
  RingBuffer<int> rbuf(3);
  getFilledBuffer(rbuf, 3);

  for (auto& i : rbuf) {
    i *= 2;
  }
  std::vector<int> expected = {0, 2, 4};
  std::vector<int> actual;
  for (auto i : rbuf) actual.push_back(i);
  ASSERT_EQ(actual, expected);
}

TEST(Iterator, EndReached) {
  RingBuffer<int> rbuf(3);
  getFilledBuffer(rbuf, 3);

  auto it = rbuf.begin();
  int steps = 0;
  while (it != rbuf.end()) {
    ++it;
    ++steps;
  }
  ASSERT_EQ(steps, 3);
}

TEST(Iterator, STL_compatible) {
  RingBuffer<int> rbuf(4);
  getFilledBuffer(rbuf, 4);

  int sum = std::accumulate(rbuf.begin(), rbuf.end(), 0);
  ASSERT_EQ(sum, 6);
}
