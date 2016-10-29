#pragma once
#include "system.hxx"

packed_struct SGDTEntry
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
	packed_union
	{
		packed_struct
		{
			u1 access_byte;		// Access flags, determine what ring this segment can be used in.
			u1 granularity_byte;
		};
		packed_struct
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
		};
	};
	u1 base_high;		// The last 8 bits of the base.

	inline void SetLength(u4 length);
	inline void SetBase(u4 base);
	inline void SetBase(void *base);
	inline void Setup(u4 base, u4 length, ERing r, EDescriptorType dt, ESegmentType st, EGranularity g = GranularityKByte, EOperandSize os = Bits32);
	inline void Clear();
};

static_assert(sizeof(SGDTEntry) == 8, "Unsupported compiler: bit fields must be packed");

packed_struct SGDTPointer
{
	u2 limit;
	SGDTEntry *base;
};

static_assert(sizeof(SGDTPointer) == 6, "Unsupported compiler: unexpected padding detected");

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
