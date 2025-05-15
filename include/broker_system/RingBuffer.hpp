#include <iostream>
#include <vector>

#define ERROR_RINGBUF_SIZE "Capacity must be greater than 0"
template <typename T>
class RingBuffer {
	public:
		RingBuffer(size_t capacity);
		bool push(const T& msg);
		bool pop(T& msg);
		bool full() const;
		bool empty() const;
		void show() const;
		size_t size() const;
		size_t capacity() const;
		size_t available() const;

	private:
		std::vector<T> buffer_;
		size_t front_ = 0;
		size_t back_ = 0;
		size_t count_ = 0;
		size_t capacity_;
};
