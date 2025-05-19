#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <iostream>
#include <vector>

#define ERROR_RINGBUF_SIZE "Capacity must be greater than 0"
template <typename T>
class RingBuffer {
 public:
  explicit RingBuffer(size_t capacity);
  bool push(const T& msg);
  bool pop(T& msg);
  bool full() const;
  bool empty() const;
  void show() const;
  size_t size() const;
  size_t capacity() const;
  size_t available() const;

	class Iterator {
		public:
			using iterator_category = std::forward_iterator_tag;
			using value_type = T;
			using pointer = T*;
			using reference = T&;
			using difference_type = std::ptrdiff_t;

			Iterator(std::vector<T>& buffer, size_t pos, size_t count, size_t capacity);

			reference operator*();
			pointer operator->();
			Iterator& operator++();
			Iterator operator++(int);
			bool operator==(const Iterator& other) const;
			bool operator!=(const Iterator& other) const;


		private:
			std::vector<T>& buffer_;
			size_t pos_;
			size_t count_;
			size_t capacity_;

	};

	class ConstIterator {
		public:
			using iterator_category = std::forward_iterator_tag;
			using value_type = T;
			using pointer = const T*;
			using reference = const T&;
			using difference_type = std::ptrdiff_t;

			ConstIterator(const std::vector<T>& buffer, size_t pos, size_t count, size_t capacity);

			reference operator*() const;
			pointer operator->() const;
			ConstIterator& operator++();
			ConstIterator operator++(int);
			bool operator==(const ConstIterator& other) const;
			bool operator!=(const ConstIterator& other) const;


		private:
			const std::vector<T>& buffer_;
			size_t pos_;
			size_t count_;
			size_t capacity_;
	};

	Iterator begin();
	Iterator end();
	ConstIterator cbegin() const;
	ConstIterator cend() const;

 private:
  std::vector<T> buffer_;
  size_t front_ = 0;
  size_t back_ = 0;
  size_t count_ = 0;
  size_t capacity_;
};

//template class RingBuffer<int>;

// RingBuffer
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
	return Iterator(buffer_, front_, count_, capacity_);
};

template <typename T>
RingBuffer<T>::Iterator RingBuffer<T>::end() {
	return Iterator(buffer_, back_, 0, capacity_);
};

template <typename T>
RingBuffer<T>::ConstIterator RingBuffer<T>::cbegin() const {
	return ConstIterator(buffer_, front_, count_, capacity_);
};

template <typename T>
RingBuffer<T>::ConstIterator RingBuffer<T>::cend() const {
	return ConstIterator(buffer_, back_, 0, capacity_);
};

//Iterator
template <typename T>
RingBuffer<T>::Iterator::Iterator(std::vector<T>& buffer, size_t pos, size_t count, size_t capacity) : buffer_(buffer), pos_(pos), count_(count), capacity_(capacity) {}

template <typename T>
RingBuffer<T>::Iterator::reference RingBuffer<T>::Iterator::operator*() {
  return buffer_[pos_];
}

template <typename T>
RingBuffer<T>::Iterator::pointer RingBuffer<T>::Iterator::operator->() {
	return &buffer_[pos_];
}

template <typename T>
RingBuffer<T>::Iterator& RingBuffer<T>::Iterator::operator++() {
	if (count_ > 0) {
		pos_ = (pos_ + 1) % capacity_;
		--count_;
	}
	return *this;
}

template <typename T>
RingBuffer<T>::Iterator RingBuffer<T>::Iterator::operator++(int) {
	Iterator tmp = *this;
	++(*this);
	return tmp;
}

template <typename T>
bool RingBuffer<T>::Iterator::operator==(const Iterator& other) const {
	return &buffer_ == &other.buffer_ && count_ == other.count_ && pos_ == other.pos_;
}

template <typename T>
bool RingBuffer<T>::Iterator::operator!=(const Iterator& other) const {
  return !(*this == other);
}

//ConstIterator
template <typename T>
RingBuffer<T>::ConstIterator::ConstIterator(const std::vector<T>& buffer, size_t pos, size_t count, size_t capacity) : buffer_(buffer), pos_(pos), count_(count), capacity_(capacity) {}

template <typename T>
RingBuffer<T>::ConstIterator::reference RingBuffer<T>::ConstIterator::operator*() const {
  return buffer_[pos_];
}

template <typename T>
RingBuffer<T>::ConstIterator::pointer RingBuffer<T>::ConstIterator::operator->() const {
	return &buffer_[pos_];
}

template <typename T>
RingBuffer<T>::ConstIterator& RingBuffer<T>::ConstIterator::operator++() {
	if (count_ > 0) {
		pos_ = (pos_ + 1) % capacity_;
		--count_;
	}
	return *this;
}

template <typename T>
RingBuffer<T>::ConstIterator RingBuffer<T>::ConstIterator::operator++(int) {
	ConstIterator tmp = *this;
	++(*this);
	return tmp;
}

template <typename T>
bool RingBuffer<T>::ConstIterator::operator==(const ConstIterator& other) const {
	return &buffer_ == &other.buffer_ && count_ == other.count_ && pos_ == other.pos_;
}

template <typename T>
bool RingBuffer<T>::ConstIterator::operator!=(const ConstIterator& other) const {
  return !(*this == other);
}

#endif

