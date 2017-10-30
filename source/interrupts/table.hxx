#pragma once
#include "system.hxx"

packed_struct SIDTEntry
{
	enum EReserved
	{
		value = 0b01110
	};

	u2 base_low;
	u2 segment;
	u1 zero;
	packed_union
	{
		u1 flags;
		packed_struct
		{
			EReserved reserved: 5;
			ERing caller: 2;
			bool present: 1;
		};
	};
	u2 base_high;
#ifdef X86_64
	u4 base_uhigh;
	u4 zero2 = 0;
#endif

	inline void* GetBase();
	inline void SetBase(uintptr_t base);
	inline void SetBase(void *base);
	inline void Setup(void *base, u2 s, ERing r);
	inline void Clear();
};

static_assert(sizeof(SIDTEntry) == 2 * sizeof(void *), "Unsupported compiler: bit fields must be packed");

packed_struct SIDTPointer
{
	u2 limit;
	SIDTEntry *base;
};

static_assert(sizeof(SIDTPointer) == 2 + sizeof(void *), "Unsupported compiler: unexpected padding detected");

void *SIDTEntry::GetBase()
{
	return reinterpret_cast<void *>(base_high << 16 | base_low);
}

void SIDTEntry::SetBase(uintptr_t base)
{
	base_low = (base) & 0x0000FFFF;
	base_high = (base >> 16) & 0x0000FFFF;
#ifdef X86_64
	base_uhigh = (base >> 32) & 0xFFFFFFFF;
#endif
}

void SIDTEntry::SetBase(void *base)
{
	SetBase(reinterpret_cast<uintptr_t>(base));
}

void SIDTEntry::Setup(void *base, u2 s, ERing r)
{
	Clear();
	SetBase(base);
	segment = s;
	caller = r;
	reserved = value;
	present = true;
}

void SIDTEntry::Clear()
{
	*reinterpret_cast<u8 *>(this) = 0;
}

