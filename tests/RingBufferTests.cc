#include <gtest/gtest.h>
#include <sstream>
#include "../src/RingBuffer/RingBuffer.hpp"

static inline void getFilledBuffer(RingBuffer<int>& rbuf, size_t n) {
	for (int i = 0; i < n; ++i) 
		rbuf.push(i);
}

TEST(SingleThread, ConstructorException) {
  try {
		RingBuffer<int> rbuf(0);
  } catch (std::invalid_argument& e) {
    ASSERT_EQ(std::string(ERROR_RINGBUF_SIZE), e.what());
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

TEST(SingleThread, Push) {
	RingBuffer<int> rbuf(5);
	for (int i = 0; i < 5; ++i) {
		ASSERT_TRUE(rbuf.push(i));
	}
	ASSERT_FALSE(rbuf.push(5));
}

TEST(SingleThread, Pop) {
	RingBuffer<int> rbuf(5);
	int p;
	ASSERT_FALSE(rbuf.pop(p));
	rbuf.push(1);
	ASSERT_TRUE(rbuf.pop(p));
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
		ASSERT_EQ(i+1, rbuf.size());
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

TEST(SingleThread, WrapAround) {
	RingBuffer<int> rbuf(3);
	
	ASSERT_TRUE(rbuf.push(1));
	ASSERT_TRUE(rbuf.push(2));
	ASSERT_TRUE(rbuf.push(3));

	ASSERT_TRUE(rbuf.full());

	ASSERT_FALSE(rbuf.push(4));
	
	int out;
	ASSERT_TRUE(rbuf.pop(out));
	ASSERT_EQ(out, 1);
	ASSERT_TRUE(rbuf.pop(out));
	ASSERT_EQ(out, 2);
	
	ASSERT_TRUE(rbuf.push(4));
	ASSERT_TRUE(rbuf.push(5));
	ASSERT_FALSE(rbuf.push(6));
	
	ASSERT_TRUE(rbuf.pop(out));
	ASSERT_EQ(out, 3);
	ASSERT_TRUE(rbuf.pop(out));
	ASSERT_EQ(out, 4);
	ASSERT_TRUE(rbuf.pop(out));
	ASSERT_EQ(out, 5);
	
	ASSERT_TRUE(rbuf.empty());
}


int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
