#pragma once
#include "types.h"
#include "misc.h"

struct SGDTEntry
{
	enum EDescriptorType: bool
	{
	};

	enum ESegmentType
	{
		SegmentCode,
		SegmentData
	};

	enum EGranularity: bool
	{
		GranularityByte = 0,
		GranularityKByte = 1
	};

	enum EOperandSize: bool
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
			ESegmentType segtype:4;
			EDescriptorType desctype:1;
			int ring:2;
			bool present:1;
			
			int length_high:4;
			int system:1;
			int zero:1;
			EOperandSize opsize:1;
			EGranularity granularity:1;
		} __attribute__((packed));
	} __attribute__((packed));
	u1 base_high;		// The last 8 bits of the base.
	
	inline void SetLength(u4 length);
	inline void SetBase(u4 base);
	inline void Setup(u4 base, u4 length, u1 r, EDescriptorType dt, ESegmentType st, EGranularity g, EOperandSize os);
	inline void Clear();
} __attribute__((packed));

static_assert(sizeof(SGDTEntry) == 8, "Unsupported compiler: bit fields must be packed");

struct SGDTPointer
{
	u2 limit;
	SGDTEntry *base;
} __attribute__((packed));

// A struct describing an interrupt gate.
struct SIDTEntry
{
	u2 base_low;             // The lower 16 bits of the address to jump to when this interrupt fires.
	u2 sel;                 // Kernel segment selector.
	u1 zero;             // This must always be zero.
	u1 flags;               // More flags. See documentation.
	u2 base_high;
} __attribute__((packed));

// A struct describing a pointer to an array of interrupt handlers.
// This is in a format suitable for giving to 'lidt'.
struct idt_ptr_struct
{
   u2 limit;
   SIDTEntry *base;                // The address of the first element in our idt_entry_t array.
} __attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;

void init_descriptor_tables();

void SGDTEntry::SetLength(u4 length)
{
	if(length == 0xFFFFFFFF)
	{
		length_low = 0x0000FFFF;
		length_high = 0x0000000F;
		return;
	}
	if(granularity == GranularityKByte)
		length >>= 10;
	if(length > 0x000FFFFF)
		die("Descriptor too long");
	length_low = (length) & 0x0000FFFF;
	length_high = (length >> 16) & 0x0000000F;
}

void SGDTEntry::SetBase(u4 base)
{
	base_low = (base) & 0x0000FFFF;
	base_middle = (base >> 16) & 0x000000FF;
	base_high = (base >> 24) & 0x000000FF;;
}

void SGDTEntry::Setup(u4 base, u4 length, u1 r, EDescriptorType dt, ESegmentType st, EGranularity g, EOperandSize os)
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
	*reinterpret_cast<u8*>(this) = 0;
}
