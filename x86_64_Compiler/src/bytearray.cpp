#include "x86_64_compiler/bytearray.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>

namespace x86_64 {

	ByteArray::ByteArray()
	{
		///@ hack to avoid relocation
		m_data.reserve(0x1000);
	}

	ByteArray::ByteArray(const ByteArray& array)
		: m_data{ array.m_data }
	{
	}

	ByteArray::ByteArray(ByteArray&& array)
		: m_data{ std::move(array.m_data) }
	{
	}

	ByteArray& ByteArray::operator=(const ByteArray& array)
	{
		m_data = array.m_data;
		return *this;
	}

	ByteArray& ByteArray::operator=(ByteArray&& array)
	{
		m_data = std::move(array.m_data);
		return *this;
	}

	uint8_t* ByteArray::push(const uint8_t* data, std::size_t size)
	{
		if (size > 0)
		{
			if (data)
				m_data.insert(m_data.end(), data, data + size);
			else
				m_data.insert(m_data.end(), size, 0);
		}

		return back(size);
	}

	void ByteArray::pop(std::size_t size)
	{
		m_data.erase(
			m_data.end() - static_cast<int32_t>(std::min(size, m_data.size())),
			m_data.end());
	}

	uint8_t ByteArray::operator[](std::size_t index) const
	{
		return m_data[index];
	}

	uint8_t& ByteArray::operator[](std::size_t index)
	{
		return m_data[index];
	}

	const uint8_t* ByteArray::data() const
	{
		return m_data.data();
	}

	uint8_t* ByteArray::data()
	{
		return m_data.data();
	}

	std::size_t ByteArray::size() const
	{
		return m_data.size();
	}

	std::size_t ByteArray::capacity() const
	{
		return m_data.capacity();
	}

	void ByteArray::write(const std::string& file_name) const
	{
		std::ofstream stream(file_name, std::ios::binary);
		write(stream);
	}

	void ByteArray::write(std::ostream& stream) const
	{
		stream.write(
			reinterpret_cast<const char*>(m_data.data()),
			static_cast<std::streamsize>(size()));
	}

	std::ostream& operator<<(std::ostream& stream, const ByteArray& array)
	{
		bool first = true;

		std::ios state(nullptr);
		state.copyfmt(stream);

		for (const uint8_t& value : array.m_data)
		{
			if (!first)
				stream << " ";

			first = false;

			stream << std::setfill('0') << std::setw(2) << std::hex << int{ value };
		}

		stream.copyfmt(state);

		return stream;
	}

	uint8_t* ByteArray::back(std::size_t size)
	{
		return data() + this->size() - size;
	}

} // namespace x86_64
