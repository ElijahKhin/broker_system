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
			bool operator==(const Iterator& other);


		private:
			std::vector<T> buffer_;
			size_t pos_;
			size_t count_;
			size_t capacity_;

	};

	Iterator begin();
	Iterator end();

 private:
  std::vector<T> buffer_;
  size_t front_ = 0;
  size_t back_ = 0;
  size_t count_ = 0;
  size_t capacity_;
};

#endif

