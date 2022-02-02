#pragma once

#include <cstring>
#include <memory>
#include <string>

#include "x86_64_compiler/bytearray.hpp"
#include "x86_64_compiler/function.hpp"

namespace x86_64 {

	namespace detail {

		constexpr int8_t NOREG = -1;

		enum class Size
		{
			None,
			Byte,
			Word,
			Dword,
			Qword,
		};

		enum ByteReg
		{
			AL,
			CL,
			DL,
			BL,
			AH,
			CH,
			DH,
			BH,
		};

		enum WordReg
		{
			AX,
			CX,
			DX,
			BX,
			SP,
			BP,
			SI,
			DI,
		};

		enum DwordReg
		{
			EAX,
			ECX,
			EDX,
			EBX,
			ESP,
			EBP,
			ESI,
			EDI,
		};

		enum QwordReg
		{
			RAX,
			RCX,
			RDX,
			RBX,
			RSP,
			RBP,
			RSI,
			RDI,
			R8,
			R9,
			R10,
			R11,
			R12,
			R13,
			R14,
			R15,
			RIP,
		};

		struct RegRef
		{
		public: // methods
			constexpr RegRef()
				: size{ Size::None }
				, reg{ NOREG }
			{
			}

			constexpr RegRef(ByteReg reg)
				: size{ Size::Byte }
				, reg{ static_cast<int8_t>(reg) }
			{
			}

			constexpr RegRef(WordReg reg)
				: size{ Size::Word }
				, reg{ static_cast<int8_t>(reg) }
			{
			}

			constexpr RegRef(DwordReg reg)
				: size{ Size::Dword }
				, reg{ static_cast<int8_t>(reg) }
			{
			}

			constexpr RegRef(QwordReg reg)
				: size{ Size::Qword }
				, reg{ static_cast<int8_t>(reg) }
			{
			}

			bool operator==(const RegRef& ref) const;
			bool operator!=(const RegRef& ref) const;

		public: // fields
			Size size;
			int8_t reg;
		};

	} // namespace detail

	constexpr detail::RegRef NOREG;

	constexpr detail::RegRef AL(detail::AL);
	constexpr detail::RegRef CL(detail::CL);
	constexpr detail::RegRef DL(detail::DL);
	constexpr detail::RegRef BL(detail::BL);
	constexpr detail::RegRef AH(detail::AH);
	constexpr detail::RegRef CH(detail::CH);
	constexpr detail::RegRef DH(detail::DH);
	constexpr detail::RegRef BH(detail::BH);

	constexpr detail::RegRef AX(detail::AX);
	constexpr detail::RegRef CX(detail::CX);
	constexpr detail::RegRef DX(detail::DX);
	constexpr detail::RegRef BX(detail::BX);
	constexpr detail::RegRef SP(detail::SP);
	constexpr detail::RegRef BP(detail::BP);
	constexpr detail::RegRef SI(detail::SI);
	constexpr detail::RegRef DI(detail::DI);

	constexpr detail::RegRef EAX(detail::EAX);
	constexpr detail::RegRef ECX(detail::ECX);
	constexpr detail::RegRef EDX(detail::EDX);
	constexpr detail::RegRef EBX(detail::EBX);
	constexpr detail::RegRef ESP(detail::ESP);
	constexpr detail::RegRef EBP(detail::EBP);
	constexpr detail::RegRef ESI(detail::ESI);
	constexpr detail::RegRef EDI(detail::EDI);

	constexpr detail::RegRef RAX(detail::RAX);
	constexpr detail::RegRef RCX(detail::RCX);
	constexpr detail::RegRef RDX(detail::RDX);
	constexpr detail::RegRef RBX(detail::RBX);
	constexpr detail::RegRef RSP(detail::RSP);
	constexpr detail::RegRef RBP(detail::RBP);
	constexpr detail::RegRef RSI(detail::RSI);
	constexpr detail::RegRef RDI(detail::RDI);
	constexpr detail::RegRef R8(detail::R8);
	constexpr detail::RegRef R9(detail::R9);
	constexpr detail::RegRef R10(detail::R10);
	constexpr detail::RegRef R11(detail::R11);
	constexpr detail::RegRef R12(detail::R12);
	constexpr detail::RegRef R13(detail::R13);
	constexpr detail::RegRef R14(detail::R14);
	constexpr detail::RegRef R15(detail::R15);
	constexpr detail::RegRef RIP(detail::RIP);

	class Compiler final
	{
	public: // types
		using RegRef = detail::RegRef;

		struct MemRef
		{
		public: // methods
			MemRef(int8_t scale, const RegRef& index, const RegRef& base);

			MemRef operator+(int64_t offset) const;
			MemRef operator-(int64_t offset) const;

		private: // methods
			MemRef(const MemRef& ref, int64_t disp);

		public: // fields
			int8_t scale;
			RegRef index;
			RegRef base;
			int64_t disp;
			bool disp_specified;
		};

		friend MemRef operator+(int64_t offset, const MemRef& ref);

		struct SymRef
		{
		public: // types
			enum class Type
			{
				Abs,
				Rel,
			};

		public: // methods
			SymRef(Type type, const std::string& name);
			SymRef(const SymRef& ref);
			SymRef(SymRef&& ref);

			SymRef& operator=(const SymRef& ref);
			SymRef& operator=(SymRef&& ref);

			~SymRef();

			SymRef operator+(int64_t offset) const;
			SymRef operator-(int64_t offset) const;

		private: // methods
			SymRef(const SymRef& ref, int64_t offset);

		public: // fields
			Type type;
			const char* name;
			int64_t offset;
		};

		friend SymRef operator+(int64_t offset, const SymRef& ref);

		struct Ref
		{
		public: // types
			enum class Type
			{
				Reg,
				Mem,
			};

		public: // methods
			Ref(const RegRef& ref);
			Ref(const MemRef& ref);
			Ref(const Ref& ref);
			Ref(Ref&& ref);

			Ref& operator=(const Ref& ref);
			Ref& operator=(Ref&& ref);

			Ref operator+(int64_t offset) const;

		public: // fields
			Type type;

			union
			{
				RegRef reg;
				MemRef mem;
			};
		};

		friend Ref operator+(int64_t offset, const Ref& ref);

	public: // methods
		Compiler();
		~Compiler();

		void reset();

		void rdata(const std::string& name, const uint8_t* data, std::size_t size);

		template <class T>
		void rdata(const std::string& name, T value);

		void data(const std::string& name, const uint8_t* data, std::size_t size);

		template <class T>
		void data(const std::string& name, T value);

		void bss(const std::string& name, std::size_t size);

		const ByteArray& getCode() const;

		RegRef reg(const RegRef& reg) const;

		MemRef mem(int64_t disp) const;
		MemRef mem(const RegRef& reg) const;
		MemRef mem(const RegRef& index, int8_t scale) const;
		MemRef mem(const RegRef& base, const RegRef& index, int8_t scale) const;

		SymRef abs(const std::string& name);
		SymRef rel(const std::string& name);

		void relocate(const std::string& name, int64_t value);

		void constant(uint8_t value);
		void constant(uint16_t value);
		void constant(uint32_t value);
		void constant(uint64_t value);
		void constant(double value);

		void add(const Ref& src, const Ref& dst);
		void addb(uint8_t imm, const Ref& dst);
		void addw(uint16_t imm, const Ref& dst);
		void addl(uint32_t imm, const Ref& dst);
		void addq(uint64_t imm, const Ref& dst);

		void call(int32_t disp);
		void callw(int16_t disp);
		void callq(int32_t disp);
		void call(const Ref& ref);
		void callw(const Ref& ref);
		void callq(const Ref& ref);
		void lcall(const Ref& ref);
		void lcallw(const Ref& ref);
		void lcalll(const Ref& ref);
		void call(const SymRef& ref);
		void lcall(const SymRef& ref);

		void enter(uint16_t imm16, uint8_t imm8);
		void enterw(uint16_t imm16, uint8_t imm8);
		void enterq(uint16_t imm16, uint8_t imm8);

		void lea(const MemRef& mem_ref, const RegRef& reg_ref);
		void lea(const SymRef& sym_ref, const RegRef& reg_ref);

		void leave();
		void leavew();
		void leaveq();

		void mov(const Ref& src, const Ref& dst);
		void mov(const SymRef& src, const RegRef& dst);
		void mov(const RegRef& src, const SymRef& dst);
		void movb(uint8_t imm, const Ref& dst);
		void movw(uint16_t imm, const Ref& dst);
		void movl(uint32_t imm, const Ref& dst);
		void movl(const SymRef& imm, const Ref& dst);
		void movq(uint64_t imm, const Ref& dst);

		void nop();

		void pop(const RegRef& ref);
		void popw(const MemRef& ref);
		void popq(const MemRef& ref);

		void push(uint32_t imm);
		void pushw(uint16_t imm);
		void pushq(uint32_t imm);
		void push(const RegRef& ref);
		void pushw(const MemRef& ref);
		void pushq(const MemRef& ref);
		void pushw(const SymRef& ref);
		void pushq(const SymRef& ref);

		void ret(uint16_t imm);
		void ret();
		void lret(uint16_t imm);
		void lret();

		void sub(const Ref& src, const Ref& dst);
		void subb(uint8_t imm, const Ref& dst);
		void subw(uint16_t imm, const Ref& dst);
		void subl(uint32_t imm, const Ref& dst);
		void subq(uint64_t imm, const Ref& dst);

	private: // types
		class Impl;

	private: // fields
		std::unique_ptr<Impl> m_impl;
	};

	template <class T>
	void Compiler::rdata(const std::string& name, T value)
	{
		rdata(name, reinterpret_cast<const uint8_t*>(&value), sizeof(value));
	}

	template <>
	inline void Compiler::rdata(const std::string& name, const char* value)
	{
		rdata(name, reinterpret_cast<const uint8_t*>(&value), strlen(value));
	}

	template <class T>
	void Compiler::data(const std::string& name, T value)
	{
		data(name, reinterpret_cast<const uint8_t*>(&value), sizeof(value));
	}

	template <>
	inline void Compiler::data(const std::string& name, const char* value)
	{
		data(name, reinterpret_cast<const uint8_t*>(&value), strlen(value));
	}

} // namespace x86_64
