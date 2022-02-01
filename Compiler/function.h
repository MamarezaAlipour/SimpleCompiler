#pragma once

#include <string>
#include <vector>

#include "bytearray.h"

namespace x86 {

	class Function {
		friend class Compiler;

		ByteArray code;

	public:
		Function();

		Function(Function&& f);

		Function& operator=(Function&& f);

		int invoke(int n = 0, ...);
		int invoke(const std::vector<int>& args);

		byte* getCode();

		std::string dump();

	private:
		Function(const ByteArray& code);
		Function(ByteArray&& code);
	};
}
