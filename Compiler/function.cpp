#include "function.h"

namespace x86 {

	Function::Function() {}

	Function::Function(Function&& f) : code(std::move(f.code)) {}

	Function& Function::operator=(Function&& f) {
		code = std::move(f.code);
		return *this;
	}

	int Function::invoke(int n, ...) {
		int (*f)() = (int (*)())code.data();

		for (int* i = &n + n; i > &n; i--)
			asm("push %0"
				:
		: "g"(*i));

		int r = f();

		asm("add %0, %%esp"
			:
		: "g"(n * sizeof(int)));

		return r;
	}

	int Function::invoke(const std::vector<int>& args) {
		int (*f)() = (int (*)())code.data();

		const int* argsData = args.data();

		for (int i = args.size() - 1; i >= 0; i--)
			asm("push %0"
				:
		: "g"(argsData[i]));

		int r = f();

		asm("add %0, %%esp"
			:
		: "g"(args.size() * sizeof(int)));

		return r;
	}

	byte* Function::getCode() {
		return code.data();
	}

	std::string Function::dump() {
		std::string result;

		for (uint i = 0; i < code.size(); i++)
			result += (i > 0 ? code[i] < 0x10 ? " 0" : " " : code[i] < 0x10 ? "0" : "") + toString((int)code[i], 0x10, 0);

		return result;
	}

	Function::Function(const ByteArray& code) : code(code) {}

	Function::Function(ByteArray&& code) : code(std::move(code)) {}
}
