template <typename T>
RingBuffer<T>::RingBuffer(std::optional<size_t> capacity) {
  if (capacity == std::nullopt) {
    capacity_ = kBufSizeLockMode + 1;  // 3 entries + 1 shadow-cell
  } else if (capacity < 1) {
    throw std::invalid_argument(ERROR_RINGBUF_SIZE);
  } else if (capacity) {
    capacity_ = *capacity + 1;
  }
  buffer_.resize(capacity_);
}

template <typename T>
void RingBuffer<T>::push(const T& msg) {
  std::unique_lock<std::mutex> lock(mtx_);
  not_full_.wait(lock, [this]() { return count_ < capacity_ - 1; });

  buffer_[back_] = msg;
  ++count_;
  back_ = (back_ + 1) % capacity_;

  not_empty_.notify_one();
}

template <typename T>
bool RingBuffer<T>::try_push(const T& msg) {
  std::scoped_lock<std::mutex> lock(mtx_);
  if (count_ < capacity_ - 1) {
    buffer_[back_] = msg;
    ++count_;
    back_ = (back_ + 1) % capacity_;
    not_empty_.notify_one();
    return true;
  }
  return false;
}

template <typename T>
void RingBuffer<T>::pop(T& msg) {
  std::unique_lock<std::mutex> lock(mtx_);
  not_empty_.wait(lock, [this]() { return count_ != 0; });

  msg = buffer_[front_];
  --count_;
  front_ = (front_ + 1) % capacity_;

  not_full_.notify_one();
}

template <typename T>
bool RingBuffer<T>::try_pop(T& msg) {
  std::scoped_lock<std::mutex> lock(mtx_);
  if (count_) {
    msg = buffer_[front_];
    --count_;
    front_ = (front_ + 1) % capacity_;
    not_full_.notify_one();
    return true;
  }
  return false;
}

template <typename T>
size_t RingBuffer<T>::capacity() const {
  std::scoped_lock<std::mutex> lock(mtx_);
  return capacity_ - 1;
}

template <typename T>
size_t RingBuffer<T>::size() const {
  std::scoped_lock<std::mutex> lock(mtx_);
  return count_;
}

template <typename T>
size_t RingBuffer<T>::available() const {
  std::scoped_lock<std::mutex> lock(mtx_);
  return (capacity_ - 1) - count_;
}

template <typename T>
bool RingBuffer<T>::full() const {
  std::scoped_lock<std::mutex> lock(mtx_);
  return (back_ + 1) % capacity_ == front_;
}

template <typename T>
bool RingBuffer<T>::empty() const {
  std::scoped_lock<std::mutex> lock(mtx_);
  return front_ == back_;
}

template <typename T>
void RingBuffer<T>::show() const {
  std::scoped_lock<std::mutex> lock(mtx_);
  for (size_t i = front_; i != back_; i = (i + 1) % capacity_) {
    std::cout << buffer_[i] << ' ';
  }
  std::cout << '\n';
}

template <typename T>
std::tuple<size_t, size_t, size_t> RingBuffer<T>::snapshot() const {
  std::scoped_lock<std::mutex> lock(mtx_);
  return {count_, (capacity_ - 1) - count_, capacity_ - 1};
}

template <typename T>
RingBuffer<T>::Iterator RingBuffer<T>::begin() {
  std::scoped_lock<std::mutex> lock(mtx_);
  return Iterator(buffer_, front_, count_, capacity_);
};

template <typename T>
RingBuffer<T>::Iterator RingBuffer<T>::end() {
  std::scoped_lock<std::mutex> lock(mtx_);
  return Iterator(buffer_, back_, 0, capacity_);
};

template <typename T>
RingBuffer<T>::ConstIterator RingBuffer<T>::cbegin() const {
  std::scoped_lock<std::mutex> lock(mtx_);
  return ConstIterator(buffer_, front_, count_, capacity_);
};

template <typename T>
RingBuffer<T>::ConstIterator RingBuffer<T>::cend() const {
  std::scoped_lock<std::mutex> lock(mtx_);
  return ConstIterator(buffer_, back_, 0, capacity_);
};
