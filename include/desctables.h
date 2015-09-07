#pragma once
#include "types.h"
#include "misc.h"

void init_descriptor_tables();

enum EZero
{
	zero = 0
};

enum ERing
{
	RingKernel = 0,
	RingUser = 3
};

struct SGDTEntry
{
	enum EDescriptorType : bool
	{
		DescriptorDefault = 1
	};

	enum ESegmentType
	{
		SegmentCode = 0b1010,
		SegmentData = 0b0010,
	};

	enum EGranularity : bool
	{
		GranularityByte = 0,
		GranularityKByte = 1
	};

	enum EOperandSize : bool
	{
		Bits16 = 0,
		Bits32 = 1
	};

	u2 length_low;		// The lower 16 bits of the limit.
	u2 base_low;		// The lower 16 bits of the base.
	u1 base_middle;		// The next 8 bits of the base.
	union
	{
		struct
		{
			u1 access_byte;		// Access flags, determine what ring this segment can be used in.
			u1 granularity_byte;
		} __attribute__((packed));
		struct
		{
			ESegmentType segtype: 4;
			EDescriptorType desctype: 1;
			ERing ring: 2;
			bool present: 1;

			int length_high: 4;
			EZero system: 1;
			EZero zero: 1;
			EOperandSize opsize: 1;
			EGranularity granularity: 1;
		} __attribute__((packed));
	} __attribute__((packed));
	u1 base_high;		// The last 8 bits of the base.

	inline void SetLength(u4 length);
	inline void SetBase(u4 base);
	inline void SetBase(void *base);
	inline void Setup(u4 base, u4 length, ERing r, EDescriptorType dt, ESegmentType st, EGranularity g = GranularityKByte, EOperandSize os = Bits32);
	inline void Clear();
} __attribute__((packed));

static_assert(sizeof(SGDTEntry) == 8, "Unsupported compiler: bit fields must be packed");

struct SGDTPointer
{
	u2 limit;
	SGDTEntry *base;
} __attribute__((packed));

static_assert(sizeof(SGDTPointer) == 6, "Unsupported compiler: unexpected padding detected");

struct SIDTEntry
{
	enum EReserved
	{
		value = 0b01110
	};

	u2 base_low;
	u2 segment;
	u1 zero;
	union
	{
		u1 flags;
		struct
		{
			EReserved reserved: 5;
			ERing caller: 2;
			bool present: 1;
		} __attribute__((packed));
	} __attribute__((packed));
	u2 base_high;

	inline void* GetBase();
	inline void SetBase(u4 base);
	inline void SetBase(void *base);
	inline void Setup(void *base, u2 s, ERing r);
	inline void Clear();
} __attribute__((packed));

static_assert(sizeof(SIDTEntry) == 8, "Unsupported compiler: bit fields must be packed");

struct SIDTPointer
{
	u2 limit;
	SIDTEntry *base;
} __attribute__((packed));

static_assert(sizeof(SIDTPointer) == 6, "Unsupported compiler: unexpected padding detected");

void SGDTEntry::SetLength(u4 length)
{
	if(length == 0xFFFFFFFF)
	{
		length_low = 0x0000FFFF;
		length_high = 0x0000000F;
		return;
	}

	if(granularity == GranularityKByte)
	{
		length >>= 10;
	}

	if(length > 0x000FFFFF)
	{
		die("Descriptor too long");
	}

	length_low = (length) & 0x0000FFFF;
	length_high = (length >> 16) & 0x0000000F;
}

void SGDTEntry::SetBase(u4 base)
{
	base_low = (base) & 0x0000FFFF;
	base_middle = (base >> 16) & 0x000000FF;
	base_high = (base >> 24) & 0x000000FF;
}

void SGDTEntry::SetBase(void *base)
{
	SetBase(reinterpret_cast<u4>(base));
}

void SGDTEntry::Setup(u4 base, u4 length, ERing r, EDescriptorType dt, ESegmentType st, EGranularity g, EOperandSize os)
{
	Clear();
	present = true;
	ring = r;
	desctype = dt;
	segtype = st;
	granularity = g;
	opsize = os;
	SetBase(base);
	SetLength(length);
}

void SGDTEntry::Clear()
{
	*reinterpret_cast<u8 *>(this) = 0;
}

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
