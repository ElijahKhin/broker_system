#include "RingBuffer.hpp"

int main() {
  RingBuffer<int> rbuf(0);
  for (int i = 0; i < 4; ++i) {
    if (!rbuf.push(i)) {
      std::cout << "full: " << i << '\n';
      break;
    }
  }
  std::cout << rbuf.size() << ' ' << rbuf.capacity() << '\n';
}
