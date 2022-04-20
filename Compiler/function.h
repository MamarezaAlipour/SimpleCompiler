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
		int invoke(std::vector<int> const& args);

		byte* getCode();

		std::string dump();

	  private:
		Function(ByteArray const& code);
		Function(ByteArray&& code);
	};
} // namespace x86
