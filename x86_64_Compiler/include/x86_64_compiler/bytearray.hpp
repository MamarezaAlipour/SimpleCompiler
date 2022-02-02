#pragma once

#include <cstddef>
#include <cstdint>
#include <ostream>
#include <vector>

namespace x86_64 {

	class ByteArray final
	{
	public: // methods
		explicit ByteArray();
		explicit ByteArray(const ByteArray& array);
		explicit ByteArray(ByteArray&& array);

		ByteArray& operator=(const ByteArray& array);
		ByteArray& operator=(ByteArray&& array);

		uint8_t* push(const uint8_t* data, std::size_t size);

		template <class T>
		uint8_t* push(const T& value);

		template <class T>
		T* push();

		void pop(std::size_t size);

		template <class T>
		T pop();

		uint8_t operator[](std::size_t index) const;
		uint8_t& operator[](std::size_t index);

		const uint8_t* data() const;
		uint8_t* data();

		std::size_t size() const;
		std::size_t capacity() const;

		void write(const std::string& file_name) const;
		void write(std::ostream& stream) const;

		friend std::ostream& operator<<(
			std::ostream& stream,
			const ByteArray& array);

	private: // methods
		uint8_t* back(std::size_t size);

	private: // fields
		std::vector<uint8_t> m_data;
	};

	template <class T>
	uint8_t* ByteArray::push(const T& value)
	{
		push(reinterpret_cast<const uint8_t*>(&value), sizeof(T));
		return back(sizeof(T));
	}

	template <>
	inline uint8_t* ByteArray::push(const ByteArray& array)
	{
		push(array.data(), array.size());
		return back(array.size());
	}

	template <class T>
	T* ByteArray::push()
	{
		push(nullptr, sizeof(T));
		return reinterpret_cast<T*>(back(sizeof(T)));
	}

	template <class T>
	T ByteArray::pop()
	{
		T value = *reinterpret_cast<T*>(back(sizeof(T)));
		pop(sizeof(T));
		return value;
	}

} // namespace x86_64
