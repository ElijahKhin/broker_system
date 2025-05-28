#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <optional>
#include <thread>
#include <tuple>
#include <vector>

#define ERROR_RINGBUF_SIZE "Capacity must be greater than 0"

constexpr size_t kBufSizeLockMode = 3;

template <typename T>
class RingBuffer {
 public:
  explicit RingBuffer(std::optional<size_t> capacity);

  void push(const T& msg);
  void pop(T& msg);
  bool try_push(const T& msg);
  bool try_pop(T& msg);
  bool full() const;
  bool empty() const;
  void show() const;
  size_t size() const;
  size_t capacity() const;
  size_t available() const;
  std::tuple<size_t, size_t, size_t> snapshot() const;

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

    ConstIterator(const std::vector<T>& buffer, size_t pos, size_t count,
                  size_t capacity);

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
  mutable std::mutex mtx_;
  std::condition_variable not_full_;
  std::condition_variable not_empty_;
  std::vector<T> buffer_;
  size_t front_ = 0;
  size_t back_ = 0;
  size_t count_ = 0;
  size_t capacity_;
};

#include "detail/RingBufferConstIteratorImpl.tpp"
#include "detail/RingBufferImpl.tpp"
#include "detail/RingBufferIteratorImpl.tpp"

#endif
