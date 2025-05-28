#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

void func(std::mutex& mtx, int& i) {
  mtx.lock();
  //  std::lock_guard<std::mutex> lock(mtx);
  int j = i + 10;
  for (; i < j; ++i) {
    std::cout << i << '\n';
  }
  mtx.unlock();
}

int main() {
  int i = 0;
  std::mutex mtx;
  std::thread t1(func, std::ref(mtx), std::ref(i));
  std::thread t2(func, std::ref(mtx), std::ref(i));
  t1.join();
  t2.join();
  //  std::condition_variable one;
  //  std::condition_variable zero;
}
