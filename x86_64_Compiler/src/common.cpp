#include "x86_64_compiler/common.hpp"

#include <cstring>
#include <malloc.h>
#include <stdexcept>
#include <unistd.h>
#include <sys/mman.h>

namespace x86_64 {

	void* allocateExecutableMemory(const void* code_ptr, size_t code_size)
	{
		long page_size = sysconf(_SC_PAGE_SIZE);
		if (page_size == -1)
		{
			throw std::runtime_error("error in sysconf");
		}

		size_t size = static_cast<size_t>(page_size);

		void* mem = memalign(size, size);
		if (mem == nullptr)
		{
			throw std::runtime_error("error in memalign");
		}

		memcpy(mem, code_ptr, code_size);

		if (mprotect(mem, size, PROT_READ | PROT_WRITE | PROT_EXEC) == -1)
		{
			throw std::runtime_error("error in mprotect");
		}

		return mem;
	}

} // namespace x86_64
