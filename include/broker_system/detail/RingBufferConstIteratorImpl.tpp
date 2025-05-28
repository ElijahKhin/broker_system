template <typename T>
RingBuffer<T>::ConstIterator::ConstIterator(const std::vector<T>& buffer,
                                            size_t pos, size_t count,
                                            size_t capacity)
    : buffer_(buffer), pos_(pos), count_(count), capacity_(capacity) {}

template <typename T>
RingBuffer<T>::ConstIterator::reference
RingBuffer<T>::ConstIterator::operator*() const {
  return buffer_[pos_];
}

template <typename T>
RingBuffer<T>::ConstIterator::pointer RingBuffer<T>::ConstIterator::operator->()
    const {
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
bool RingBuffer<T>::ConstIterator::operator==(
    const ConstIterator& other) const {
  return &buffer_ == &other.buffer_ && count_ == other.count_ &&
         pos_ == other.pos_;
}

template <typename T>
bool RingBuffer<T>::ConstIterator::operator!=(
    const ConstIterator& other) const {
  return !(*this == other);
}
