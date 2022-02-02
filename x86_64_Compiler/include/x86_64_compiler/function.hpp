#pragma once

#include "x86_64_compiler/bytearray.hpp"
#include "x86_64_compiler/common.hpp"

namespace x86_64 {

	template <class>
	class Function;

	template <class R, class... Args>
	class Function<R(Args...)> final
	{
	public: // methods
		explicit Function(const ByteArray& code)
			: m_code_ptr{ code.data() } ///@ hack
			, m_code_size{ code.size() }
		{
		}

		explicit Function(Function&& f)
			: m_code_ptr{ nullptr }
		{
			*this = std::move(f);
		}

		~Function()
		{
			// free(m_code_ptr);
		}

		Function& operator=(Function&& f)
		{
			// free(m_code_ptr);

			m_code_ptr = f.m_code_ptr;
			m_code_size = f.m_code_size;

			f.m_code_ptr = nullptr;
			f.m_code_size = 0;

			return *this;
		}

		R operator()(Args &&... args) const
		{
			using FuncPtrType = R(*)(Args...);
			FuncPtrType f_ptr = reinterpret_cast<FuncPtrType>(m_code_ptr);
			return f_ptr(std::forward<Args>(args)...);
		}

	private: // fields
		const void* m_code_ptr;
		size_t m_code_size;
	};

} // namespace x86_64
