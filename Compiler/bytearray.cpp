#include "bytearray.h"

#include <fstream>
#include <memory>

uint ByteArray::initialCapacity = 1;

void ByteArray::setInitialCapacity(uint initialCapacity) {
	ByteArray::initialCapacity = initialCapacity;
}

ByteArray::ByteArray()
	: size(0)
	, capacity(0)
	, data(0) {}

ByteArray::ByteArray(const ByteArray& array)
	: data(0) {
	*this = array;
}

ByteArray::ByteArray(ByteArray&& array)
	: data(0) {
	*this = std::move(array);
}

ByteArray::~ByteArray() {
	release();
}

ByteArray& ByteArray::operator=(const ByteArray& array) {
	size = array.size;
	capacity = array.capacity;

	::free(data);

	data = (byte*)malloc(capacity);
	memcpy(data, array.data, size);

	return *this;
}

ByteArray& ByteArray::operator=(ByteArray&& array) {
	size = array.size;
	capacity = array.capacity;

	::free(data);

	data = array.data;

	array.data = 0;
	array.size = 0;
	array.capacity = 0;

	return *this;
}

byte* ByteArray::allocate(uint count) {
	if (!enoughSpace(count)) {
		uint newCapacity = ceilToPowerOf2(std::max(initialCapacity, size + count));

		byte* newData = (byte*)realloc(data, newCapacity);

		if (!newData)
			return 0;

		capacity = newCapacity;
		data = newData;
	}

	size += count;

	return data + size - count;
}

int ByteArray::reallocate() {
	byte* newData = (byte*)malloc(capacity);

	if (!newData)
		return 0;

	memcpy(newData, data, size);

	int delta = newData - data;

	::free(data);
	data = newData;

	return delta;
}

void ByteArray::push(const byte* data, uint size) {
	memcpy(allocate(size), data, size);
}

byte& ByteArray::operator[](int index) {
	return data[index];
}

bool ByteArray::free(uint count) {
	if (size < count)
		return false;

	size -= count;
	return true;
}

void ByteArray::release() {
	::free(data);

	size = 0;
	capacity = 0;
	data = 0;
}

bool ByteArray::enoughSpace(uint count) const {
	return size + count <= capacity;
}

byte* ByteArray::data() const {
	return data;
}

uint ByteArray::size() const {
	return size;
}

uint ByteArray::capacity() const {
	return capacity;
}

void ByteArray::write(const std::string& fileName) const {
	std::ofstream stream(fileName, std::ios::binary);
	stream.write((char*)data, size);
	stream.close();
}
