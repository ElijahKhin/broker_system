#include <gtest/gtest.h>

#include <broker_system/RingBuffer.h>
#include <sstream>
#include <chrono>

static inline void getFilledBuffer(RingBuffer<int>& rbuf, size_t n) {
  for (int i = 0; i < n; ++i) rbuf.push(i);
}

TEST(SingleThread, ConstructorException) {
  try {
    RingBuffer<int> rbuf(0);
  } catch (std::invalid_argument& e) {
    ASSERT_EQ(std::string(ERROR_RINGBUF_SIZE), e.what());
  }
}

TEST(SingleThread, ConstructorNoSize) {
  RingBuffer<int> rbuf(std::nullopt);
  getFilledBuffer(rbuf, 3);
  std::vector<int> tst = {0, 1, 2};
  for (auto i : tst) {
    int p;
    rbuf.pop(p);
    ASSERT_EQ(i, p);
  }
}

TEST(SingleThread, ConstructorNormal) {
  size_t n = 5;
  RingBuffer<int> rbuf(n);
  getFilledBuffer(rbuf, n);
  std::vector<int> tst = {0, 1, 2, 3, 4};
  for (auto i : tst) {
    int p;
    rbuf.pop(p);
    ASSERT_EQ(i, p);
  }
}

TEST(SingleThread, PushPop) {
  RingBuffer<int> rbuf(5);
  for (int i = 0; i < 5; ++i) {
    rbuf.push(i);
  }
  for (int i = 0; i < 5; ++i) {
    int msg;
    rbuf.pop(msg);
    ASSERT_EQ(msg, i);
  }
}


TEST(SingleThread, Capacity) {
  RingBuffer<int> rbuf(3);
  for (int i = 0; i < 3; ++i) {
    rbuf.push(i);
    ASSERT_EQ(3, rbuf.capacity());
  }
}

TEST(SingleThread, Size) {
  RingBuffer<int> rbuf(3);
  for (int i = 0; i < 3; ++i) {
    rbuf.push(i);
    ASSERT_EQ(i + 1, rbuf.size());
  }
}

TEST(SingleThread, Available) {
  RingBuffer<int> rbuf(3);
  for (int i = 0, j = 3; i < 3; ++i, --j) {
    ASSERT_EQ(j, rbuf.available());
    rbuf.push(i);
  }
}

TEST(SingleThread, Show) {
  RingBuffer<int> rbuf(3);
  for (int i = 0; i < 3; ++i) {
    rbuf.push(i);
  }
  std::ostringstream oss;
  std::streambuf* orig_cout = std::cout.rdbuf();
  std::cout.rdbuf(oss.rdbuf());

  rbuf.show();
  std::cout.rdbuf(orig_cout);
  ASSERT_EQ(oss.str(), "0 1 2 \n");
}

TEST(SingleThread, TryPush) {
  RingBuffer<int> rbuf(std::nullopt);
  ASSERT_TRUE(rbuf.try_push(0));
  ASSERT_TRUE(rbuf.try_push(0));
  ASSERT_TRUE(rbuf.try_push(0));
  ASSERT_FALSE(rbuf.try_push(0));
}

TEST(SingleThread, TryPop) {
  int msg;
  RingBuffer<int> rbuf(std::nullopt);
  ASSERT_FALSE(rbuf.try_pop(msg));
  ASSERT_TRUE(rbuf.try_push(0));
  ASSERT_TRUE(rbuf.try_push(0));
  ASSERT_TRUE(rbuf.try_push(0));
  ASSERT_TRUE(rbuf.try_pop(msg));
  ASSERT_TRUE(msg == 0);
}

TEST(SingleThread, WrapAround) {
  RingBuffer<int> rbuf(3);

  rbuf.push(1);
  rbuf.push(2);
  rbuf.push(3);

  ASSERT_TRUE(rbuf.full());

  int out;
  rbuf.pop(out);
  ASSERT_EQ(out, 1);
  rbuf.pop(out);
  ASSERT_EQ(out, 2);

  rbuf.push(4);
  rbuf.push(5);

  rbuf.pop(out);
  ASSERT_EQ(out, 3);
  rbuf.pop(out);
  ASSERT_EQ(out, 4);
  rbuf.pop(out);
  ASSERT_EQ(out, 5);

  ASSERT_TRUE(rbuf.empty());
}

int main(int argc, char** argv) {
//  std::ios::sync_with_stdio(false);
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
