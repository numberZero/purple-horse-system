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
		u1 flags = 0;
		packed_struct
		{
			EReserved reserved: 5;
			ERing caller: 2;
			bool present: 1;
		};
	};
	u2 base_high;

	inline void* GetBase();
	inline void SetBase(u4 base);
	inline void SetBase(void *base);
	inline void Setup(void *base, u2 s, ERing r);
	inline void Clear();
};

static_assert(sizeof(SIDTEntry) == 8, "Unsupported compiler: bit fields must be packed");

packed_struct SIDTPointer
{
	u2 limit;
	SIDTEntry *base;
};

static_assert(sizeof(SIDTPointer) == 6, "Unsupported compiler: unexpected padding detected");

void *SIDTEntry::GetBase()
{
	return reinterpret_cast<void *>(base_high << 16 | base_low);
}

void SIDTEntry::SetBase(u4 base)
{
	base_low = (base) & 0x0000FFFF;
	base_high = (base >> 16) & 0x0000FFFF;
}

void SIDTEntry::SetBase(void *base)
{
	SetBase(reinterpret_cast<u4>(base));
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

