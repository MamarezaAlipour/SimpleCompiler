#pragma once

#include <cstddef>

namespace x86_64 {

	void* allocateExecutableMemory(const void* code_ptr, size_t code_size);

} // namespace x86_64
