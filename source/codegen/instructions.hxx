#pragma once
#include "types.h"

inline void* operator new(size_t, void* p) { return p; }
inline void* operator new[](size_t, void* p) { return p; }
inline void operator delete(void*, void*) {}
inline void operator delete[](void*, void*) {}

template <typename _T, _T _value>
struct integral_constant
{
	typedef _T Type;
	static constexpr const Type value = _value;
};

template <bool _value>
struct bool_constant: integral_constant<bool, _value> {};

namespace codegen
{
inline namespace i686
{

template <typename T>
struct is_instruction: bool_constant<false> {};

packed_struct FlagInterrupt
{
	enum class Opcode: u1
	{
		cli = 0xFA,
		sti = 0xFB,
		disallow = cli,
		allow = sti
	} opcode = Opcode::cli;

	FlagInterrupt() = default;
	FlagInterrupt(Opcode op) :
		opcode(op)
	{
	}
};
static_assert(sizeof(FlagInterrupt) == 1, "Structure is not packed");

packed_struct Nop
{
	enum class Opcode: u1
	{
		nop = 0x90
	} opcode = Opcode::nop;
};
static_assert(sizeof(Nop) == 1, "Structure is not packed");

packed_struct PushByte
{
	enum class Opcode: u1
	{
		pushl = 0x6A
	} opcode = Opcode::pushl;
	u1 data = 0;

	PushByte() = default;
	PushByte(u1 byte) :
		data(byte)
	{
	}
};
static_assert(sizeof(PushByte) == 2, "Structure is not packed");

packed_struct JumpLong
{
	enum class Opcode: u1
	{
		jmp = 0xE9
	} opcode = Opcode::jmp;
	s4 offset = 0; // does nothing

	JumpLong() = default;
	JumpLong(JumpLong const&) = delete;
	JumpLong(JumpLong&&) = delete;
	JumpLong(void const* dest)
	{
		setDestination(dest);
	}

	void const* getDestination() const
	{
		return reinterpret_cast<void const*>(reinterpret_cast<u4>(this + 1) + offset);
	}

	void setDestination(void const* dest)
	{
		offset = reinterpret_cast<u4>(dest) - (reinterpret_cast<u4>(this + 1));
	}
};
static_assert(sizeof(JumpLong) == 5, "Structure is not packed");

template <> struct is_instruction<FlagInterrupt>: bool_constant<true> {};
template <> struct is_instruction<Nop>: bool_constant<true> {};
template <> struct is_instruction<PushByte>: bool_constant<true> {};
template <> struct is_instruction<JumpLong>: bool_constant<true> {};

template <typename Instr>
packed_struct Optional
{
	static_assert(is_instruction<Instr>::value, "Instruction class required");
	static constexpr const size_t size = sizeof(Instr);
	packed_union
	{
		Instr instr;
		Nop nop[size];
	};

	Optional() :
		nop()
	{
	}

	template <typename Arg, typename... Args>
	Optional(Arg arg, Args... args) :
		instr(arg, args...)
	{
	}

	bool is_enabled() const
	{
		return nop[0].opcode != Nop::Opcode::nop;
	}

	template <typename... Args>
	void enable(Args... args)
	{
		new(&instr) Instr(args...);
	}

	void disable()
	{
		new(&nop) Nop[size];
	}
};

}
}
