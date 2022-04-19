#pragma once

#include "common.h"

class ByteArray {
	static uint initialCapacity;

	uint size, capacity;
	byte* data;

  public:
	static void setInitialCapacity(uint initialCapacity);

	ByteArray();

	ByteArray(ByteArray const& array);
	ByteArray(ByteArray&& array);

	~ByteArray();

	ByteArray& operator=(ByteArray const& array);
	ByteArray& operator=(ByteArray&& array);

	byte* allocate(uint count);
	int reallocate();

	template <class T>
	ByteArray& push(T value);

	template <class T>
	T pop();

	void push(byte const* data, uint size);

	byte& operator[](int index);

	bool free(uint count);
	void release();

	bool enoughSpace(uint count) const;

	byte* data() const;
	uint size() const;
	uint capacity() const;

	void write(std::string const& fileName) const;
};

template <class T>
ByteArray& ByteArray::push(T value) {
	*(T*)allocate(sizeof(T)) = value;
	return *this;
}

template <>
inline ByteArray& ByteArray::push(ByteArray array) {
	push(array.data(), array.size());
	return *this;
}

template <class T>
T ByteArray::pop() {
	free(sizeof(T));
	return *(T*)(data + size);
}
