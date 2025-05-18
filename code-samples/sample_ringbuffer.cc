#include <broker_system/RingBuffer.h>

int main() {
  RingBuffer<int> rbuf(100);
  for (int i = 0; i < 4; ++i) {
    if (!rbuf.push(i)) {
      std::cout << "full: " << i << '\n';
      break;
    }
  }
  std::cout << rbuf.size() << ' ' << rbuf.capacity() << '\n';
}
