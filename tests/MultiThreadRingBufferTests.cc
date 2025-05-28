#include <broker_system/RingBuffer.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <atomic>
#include <numeric>
#include <random>
#include <sstream>

std::mutex cout_mtx;

int gen_random_int(int min, int max) {
  std::mt19937 gen(std::random_device{}());
  std::uniform_int_distribution<> dist(min, max);
  return dist(gen);
}

// void rbuf_push(RingBuffer<int>& rbuf, std::atomic<int>& push_sum, size_t n) {
//   for (int i = 0; i < n; ++i) {
//     int random = gen_random_int(1, 100);
//     rbuf.push(random);
//     push_sum += random;
//   }
// }
//
// void rbuf_pop(RingBuffer<int>& rbuf, std::atomic<int>& pop_sum, size_t n) {
//   for (int i = 0; i < n; ++i) {
//     int msg;
//     rbuf.pop(msg);
//     pop_sum += msg;
//   }
// }

TEST(MultiThread, WaitReader) {
  std::atomic<int> push_sum(0), pop_sum(0);
  size_t n = 1100;
  RingBuffer<int> rbuf(n - 834);

  auto producer = [&]() {
    for (int i = 0; i < n; ++i) {
      int random = gen_random_int(1, 100);
      rbuf.push(random);
      push_sum += random;
    }
  };

  auto consumer = [&]() {
    for (int i = 0; i < n; ++i) {
      int msg;
      rbuf.pop(msg);
      pop_sum += msg;
    }
  };

  std::thread t1(producer);
  std::thread t2(consumer);

  t1.join();
  t2.join();
  ASSERT_EQ(push_sum, pop_sum);
}

TEST(MultiThread, TryPushTryPopConcurrent) {
  RingBuffer<int> rbuf(10);
  std::atomic<int> push_sum(0);
  std::atomic<int> pop_sum(0);
  const int total = 1000;

  auto producer = [&]() {
    for (int i = 0; i < total; ++i) {
      int val = gen_random_int(1, 10);
      while (!rbuf.try_push(val)) {
        std::this_thread::yield();
      }
      push_sum += val;
    }
  };

  auto consumer = [&]() {
    for (int i = 0; i < total; ++i) {
      int val;
      while (!rbuf.try_pop(val)) {
        std::this_thread::yield();
      }
      pop_sum += val;
    }
  };

  std::thread t1(producer);
  std::thread t2(consumer);

  t1.join();
  t2.join();

  ASSERT_EQ(push_sum.load(), pop_sum.load());
}

TEST(MultiThread, ConcurrentStateChecks) {
  RingBuffer<int> rbuf(100);
  std::atomic<bool> done{false};

  std::thread writer([&]() {
    for (int i = 0; i < 100; ++i) {
      rbuf.push(i);
    }
    done = true;
  });

  std::thread reader([&]() {
    int val;
    while (!done || !rbuf.empty()) {
      rbuf.pop(val);
      std::this_thread::sleep_for(std::chrono::microseconds(1));
    }
  });

  std::thread state_checker1([&]() {
    while (!done || !rbuf.empty()) {
      auto [size, avail, cap] = rbuf.snapshot();
      EXPECT_LE(size, cap);
      EXPECT_LE(avail, cap);
      EXPECT_EQ(size + avail, cap);
    }
  });

  std::thread state_checker2([&]() {
    while (!done || !rbuf.empty()) {
      auto [size, avail, cap] = rbuf.snapshot();
      EXPECT_LE(size, cap);
      EXPECT_LE(avail, cap);
      EXPECT_EQ(size + avail, cap);
    }
  });

  writer.join();
  reader.join();
  state_checker1.join();
  state_checker2.join();
}
