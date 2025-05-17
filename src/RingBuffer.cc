#include <broker_system/RingBuffer.h>

template <typename T>
RingBuffer<T>::RingBuffer(size_t capacity) {
  if (capacity < 1) {
    throw std::invalid_argument(ERROR_RINGBUF_SIZE);
  }
  capacity_ = capacity + 1;
  buffer_.resize(capacity_);
}

template <typename T>
bool RingBuffer<T>::push(const T& msg) {
  if (full()) {
    return false;
  }
  buffer_[back_] = msg;
  ++count_;
  back_ = (back_ + 1) % capacity_;
  return true;
}

template <typename T>
bool RingBuffer<T>::pop(T& msg) {
  if (empty()) {
    return false;
  }
  msg = buffer_[front_];
  --count_;
  front_ = (front_ + 1) % capacity_;
  return true;
}

template <typename T>
size_t RingBuffer<T>::capacity() const {
  return capacity_ - 1;
}

template <typename T>
size_t RingBuffer<T>::size() const {
  return count_;
}

template <typename T>
size_t RingBuffer<T>::available() const {
  return (capacity_ - 1) - count_;
}

template <typename T>
bool RingBuffer<T>::full() const {
  return (back_ + 1) % capacity_ == front_;
}

template <typename T>
bool RingBuffer<T>::empty() const {
  return front_ == back_;
}

template <typename T>
void RingBuffer<T>::show() const {
  for (size_t i = front_; i != back_; i = (i + 1) % capacity_) {
    std::cout << buffer_[i] << ' ';
  }
  std::cout << '\n';
}

template <typename T>
RingBuffer<T>::Iterator RingBuffer<T>::begin() {
	Iterator(buffer_, front_, count_, capacity_);
};

template <typename T>
RingBuffer<T>::Iterator RingBuffer<T>::end() {
	Iterator(buffer_, back_, 0, capacity_);
};
