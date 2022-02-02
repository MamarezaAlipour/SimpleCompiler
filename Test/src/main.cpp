#include <x86_64_compiler/compiler.hpp>

#include <cstring>
#include <malloc.h>
#include <stdexcept>
#include <unistd.h>
#include <sys/mman.h>

#include <fstream>
#include <iostream>
#include <sstream>

void testCommands() try
{
	using namespace x86_64;

	Compiler c;
	std::ofstream txt("dump.txt");
	std::ofstream bin("dump.bin", std::ios::binary);
	std::stringstream s;
	std::size_t correct_count = 0;

#define X(command, correct)                                              \
    {                                                                    \
        command;                                                         \
        const auto &code = c.getCode();                                  \
        code.write(bin);                                                 \
        s << code;                                                       \
        std::string space(std::max(0, 50 - int{strlen(#command)}), ' '); \
        if (s.str() == correct)                                          \
        {                                                                \
            correct_count++;                                             \
            space[space.size() - 2] = '+';                               \
        }                                                                \
        txt << #command << space << s.str()                              \
            << (s.str() == correct ? "" : std::string(" ~ ") + correct)  \
            << std::endl;                                                \
        c.reset();                                                       \
        s.str("");                                                       \
        s.clear();                                                       \
    }

	// #include "commands.txt"

	bin << std::flush;

	system("objdump -D -w -b binary -m i386:x86-64 dump.bin > disasm.txt");

	std::cout << correct_count << std::endl;
}
catch (const std::exception& e)
{
	std::cout << "error: " << e.what() << std::endl;
}

#pragma GCC push_options
#pragma GCC optimize("O0")

void puts_wrapper(const char* str)
{
	puts(str);
}

#pragma GCC pop_options

int main()
{
	// testCommands();
	// return 0;

	using namespace x86_64;

	Compiler c;

	c.push(RBP);
	c.mov(RSP, RBP);
	c.lea(c.rel("str"), RDI);
	c.call(c.rel("puts"));
	c.movl(0, EAX);
	c.pop(RBP);
	c.ret();

	c.relocate("str", reinterpret_cast<int64_t>("Hello, World!"));
	c.relocate("puts", reinterpret_cast<int64_t>(puts_wrapper));

	// std::cout << std::hex << (int64_t)c.getCode().data() << std::endl
	//           << std::flush;
	// std::cout << std::hex << (int64_t)puts << std::endl << std::flush;

	std::ofstream s("dump.bin", std::ios::binary);

	// ByteArray f_code;
	// f_code.push(reinterpret_cast<uint8_t *>(&puts_wrapper), 28);
	// f_code.write(s);

	c.getCode().write(s);
	s << std::flush;
	system("objdump -D -w -b binary -m i386:x86-64 dump.bin > disasm.txt");

	const ByteArray& code = c.getCode();

	if (mprotect(
		reinterpret_cast<void*>(
			reinterpret_cast<int64_t>(code.data()) & ~0xfff),
		0x1000,
		PROT_READ | PROT_WRITE | PROT_EXEC) == -1)
	{
		return 1;
	}

	Function<void()> f(code);
	f();

	return 0;
}
