#include <broker_system/RingBuffer.h>

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
bool RingBuffer<T>::Iterator::operator==(const Iterator& other) {
	return count_ == other.count_ && pos_ == other.pos_;
}

template class RingBuffer<int>;
